//Sketch1.cpp
#include <Arduino.h>
#include "MET.h"

#define CLOCK 2 
#define DATA 9
#define SWITCH 8
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
  Serial.begin(57600);
  pinMode(CLOCK, INPUT);
  pinMode(DATA, INPUT);
  pinMode(SWITCH, INPUT_PULLUP);
  
  m = new MET();

  attachInterrupt(digitalPinToInterrupt(CLOCK), encoderISR, CHANGE);
  Serial.print("Game Mode: ");
  Serial.println(gameMode); 
  /*
  m.run(3);
  */
}



void loop() {
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
}
  