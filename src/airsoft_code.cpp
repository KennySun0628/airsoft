//Sketch1.cpp
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "MET.h"

#define CLOCK 18 
#define DATA 19
#define SWITCH 21
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
bool buttonPressed = false;
MET* m;

int lastClockState = LOW;  // Track the last state of the CLOCK pin
unsigned long lastDebounceTime = 0;  // Last time the state changed
unsigned long debounceDelay = 100;  // Debounce delay (in milliseconds)


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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  Serial.println("Setup start");
  delay(100);
// Check the high water mark for stack usage
  //Serial.print("FreeRTOS Stack High Water Mark: ");
  //Serial.println(uxTaskGetStackHighWaterMark(NULL));

  pinMode(CLOCK, INPUT);
  pinMode(DATA, INPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  Serial.println("Pins initialized");
  delay(100);

  m = new MET();
  Serial.println("MET created");

  /*  
  attachInterrupt(digitalPinToInterrupt(CLOCK), encoderISR, CHANGE);
  Serial.println("Interrupt Attached");

  Serial.print("Game Mode: ");
  Serial.println(gameMode); 
 */ 
}



void loop() {
  /*
  if (stateChanged) {  // If encoder changed state
    Serial.print("Game Mode: ");
    Serial.println(gameMode);
    stateChanged = false;  // Reset flag after printing
  }

  if (digitalRead(SWITCH) == LOW && !buttonPressed) {
    Serial.print("Starting Game Mode: ");
    Serial.println(gameMode);
    m -> run(gameMode);  // Call function to start selected mode
    Serial.print("Game Mode: ");
    Serial.println(gameMode);
    buttonPressed = true;  // Prevents multiple triggers while held
  }

  if (digitalRead(SWITCH) == HIGH) {
    buttonPressed = false;  // Reset when button is released
  }
  */

  delay(1000);
  Serial.println("Loop running");
}
  