//Sketch1.cpp
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

#include "log.h"
#include "index.h"
#include "MET.h"

#define CLOCK 18 
#define DATA 19
#define SWITCH 21

#define AP false
#define ROUTER true

const char* apSSID = "ESP32";
const char* apPassword = "Airsoft123";

const char* routerSSID = "";
const char* routerPassword = "";

const bool requireClient = true;

const bool serverMode = AP;


char* ssid = nullptr;
char* password = nullptr;


WebServer server (80);
WebSocketsServer webSocket = WebSocketsServer(81);

String webpage = HTML_CONTENT_GAMEMODE;

int interval = 1000;
unsigned long previousMillis = 0;

JsonDocument doc_tx = StaticJsonDocument<200>();
JsonDocument doc_rx = StaticJsonDocument<200>();


/*
****************************************************************************************
Game Modes:
1. QuickDraw
2. Search and Destroy
3. Blackout
4. Random
5. Twin Shot
****************************************************************************************
*/


volatile int gameMode = 1;   // Stores the current game mode (1-5)
volatile bool stateChanged = false;
int lastState;
bool clientConnected = false;
bool buttonPressed = false;
bool gameRunning = false;
QueueHandle_t gameModeQueue;

int lastClockState = LOW;  // Track the last state of the CLOCK pin
unsigned long lastDebounceTime = 0;  // Last time the state changed
unsigned long debounceDelay = 100;  // Debounce delay (in milliseconds)

void TaskGameMode(void* pvParameters);
void TaskWebServer(void* pvParameters);
void webSocketEvent(byte num, WStype_t type, uint8_t* payload, size_t length);
void sendLog(String message);
void encoderISR();

void setup() {
  Serial.begin(115200);
  
  gameModeQueue = xQueueCreate(1, sizeof(int));

  xTaskCreatePinnedToCore(
    TaskGameMode,
    "GameModeTask",
    4096,
    NULL,
    1,
    NULL,
    1 
  );

  xTaskCreatePinnedToCore(
    TaskWebServer,
    "WebServerTask",
    8192,
    NULL,
    1,
    NULL,
    0 
  );

}

//Game Mode Thread
//Runs controls logic and peripherals for playing games
void TaskGameMode(void* pvParameters){
  pinMode(CLOCK, INPUT);
  pinMode(DATA, INPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  sendLog("Pins initialized");

  attachInterrupt(digitalPinToInterrupt(CLOCK), encoderISR, CHANGE);
  sendLog("Interrupt Attached");

  MET m;
  sendLog("MET created");

  if(requireClient){
    while(!clientConnected){
      sendLog("Waiting for Client...");
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
  int receivedGameMode = gameMode;

  sendLog("Game Mode: " + String(gameMode));

  while(true){
    if(xQueueReceive(gameModeQueue, &receivedGameMode, 0) == pdTRUE){
      gameMode = receivedGameMode;
      sendLog("Game Mode Started from Web: " + String(gameMode));
      m.run(gameMode);
      gameRunning = false;
    }

    if(stateChanged){
      sendLog("Game Mode: " + String(gameMode));
      stateChanged = false;
    }
    
    if(digitalRead(SWITCH) == LOW && !buttonPressed){
      gameRunning = true;
      sendLog("Starting Game Mode: " + String(gameMode));
      m.run(gameMode);
      buttonPressed = true;
      gameRunning = false;
    }

    if(digitalRead(SWITCH) == HIGH){
      buttonPressed = false;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

//ISR for rotary encoder detection including bounce delay
void encoderISR() {
  int currentClockState = digitalRead(CLOCK);  // Read the current state of the CLOCK pin
  
  // Debounce the signal: Only respond after a debounce delay has passed
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Check if the clock state has changed (from LOW to HIGH or vice versa)
    if (currentClockState != lastClockState) {
      int currentDataState = digitalRead(DATA);  // Read the DATA pin to determine direction
      
      if (currentDataState != currentClockState) {
        gameMode++;  // Clockwise rotation
        if (gameMode > 5) gameMode = 1;
      } else {
        gameMode--;  // Counter-clockwise rotation
        if (gameMode < 1) gameMode = 5;
      }
      stateChanged = true;  // Mark that the gameMode has changed
      lastDebounceTime = millis();  // Update debounce timer
    }
  }
  lastClockState = currentClockState;  // Update the last clock state for the next comparison
}

//Web Server Thread
//Starts web server on startup, handles and maintains client connections while game is run
//Also sends game mode selection data to game mode thread
void TaskWebServer(void* pvParameters){
  if(serverMode == AP){
   
    IPAddress local_IP(192,168,1,22);
    IPAddress gateway(192,168,1,5);
    IPAddress subnet(255,255,255,0);

    Serial.print("Setting up Access Point");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? " Ready" : " Failed!");
    
    Serial.print("Setting AP...");
    Serial.println(WiFi.softAP(apSSID, apPassword, 6) ? " Ready" : " Failed!");

    Serial.print("IP address = ");
    Serial.println(WiFi.softAPIP());
  }
  else if(serverMode == ROUTER){
    WiFi.begin(routerSSID, routerPassword);
    Serial.print("Establishing connection to WiFi with SSID: ");
    Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      sendLog(".");
    }

    Serial.print("Connected to network with IP address: ");
    Serial.println(WiFi.localIP());
  }
 
  server.on("/", [] () {
      server.send(200, "text/html", webpage);
  });

  server.begin();
  webSocket.begin();

  webSocket.onEvent(webSocketEvent);
  Serial.println("Web Server Started");

  while(true){
    server.handleClient();
    webSocket.loop();

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void webSocketEvent(byte num, WStype_t type, uint8_t *payload, size_t length){
  switch(type){
    case WStype_DISCONNECTED:
      sendLog("Client Disconnected");
      break;
    case WStype_CONNECTED:
      clientConnected = true;
      sendLog("Client Connected");
      break;
    case WStype_TEXT:
      DeserializationError error = deserializeJson(doc_rx, payload);
      if(error){
          sendLog("Deserialize Json() failed");
          return;
      }
      const int selectedGamemode = doc_rx["game_mode"];
      if(!gameRunning){
        gameRunning = true;
        xQueueSend(gameModeQueue, &selectedGamemode, 0);
      }
      else{
        sendLog("Game is in progress");
      }
      break;
  }
  
}

void loop() {
}
  