//MET.cpp
//Implementation for MET class
//Kenny Sun

#include <TrueRandom.h>
#include "MET.h"
using namespace std;

//constructor
MET::MET():  strip(TARGET_NUM_LED * NUM_TARGETS, TARGET_LED_PIN, NEO_GRB + NEO_KHZ800){
  strip.clear();
  strip.setBrightness(LED_BRIGHTNESS);
  PT_INIT(&timerThread);
  for(int i = 0; i < NUM_TARGETS; i++){
    target[i].SENSOR_PIN = (i + 3);
    pinMode(target[i].SENSOR_PIN, INPUT);
    target[i].currentStatus = LOW;
    target[i].startingLedIndex = i * (TARGET_NUM_LED);
    target[i].endingLedIndex =  target[i].startingLedIndex + (TARGET_NUM_LED - 1);
  }

  strip.begin();
}

//destructor
MET::~MET(){}

//Main function to facilitate modes changes
void MET::run(int gameMode){
  
	switch (gameMode){
		case 1:  
			quickDraw();
			break;
		case 2: 
			SD();
			break;
		case 3:
			blackout();
			break;
		case 4:
      countMode = false;
      countDownTime = RANDOM_TIME;
			random();
			break;
		case 5:
      countMode = false;
      countDownTime = TWIN_TIME;
			twin();
			break;
	}
}
/*
****************************************************************************************
Helper functions for setting and controlling individual or all target LEDs
All function have no Return Values (void)

neoPixelColors enum:
  RED   = 0xFF0000,
  GREEN = 0x00FF00,
  BLUE  = 0x0000FF,
  YELLOW = 0xFFFF00,
  CYAN  = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  WHITE = 0xFFFFFF,
  OFF   = 0x000000
****************************************************************************************
*/

/*
void setTargetColor (int targetNum, neoPixelColors color, bool clearStrip)
sets the color of a single target, with the option to clear all other leds or retain their original colors

Parameters: 
int targetNum         - target number to change color (Indexed at 1 to match the physical target number)
neoPixelColors color  - Color to set the target to (from the neoPixelColors enum list)
bool clearStrip       - true:   Sets other target LEDs to OFF 
                        false:  Other target LEDs retains their original states
*/
void MET::setTargetColor(int targetNum, neoPixelColors color, bool clearStrip){
  if(VERBOSE){
    Serial.print("Target: ");
    Serial.print(targetNum);
    switch(color){
      case RED:
        Serial.println(" RED");
        break;

      case GREEN:
        Serial.println(" GREEN");
        break;

      case BLUE: 
        Serial.println(" BLUE");
        break;
      
      case YELLOW:
        Serial.println(" YELLOW");
        break;
      
      case CYAN:
        Serial.println(" CYAN");
        break;
      
      case MAGENTA:
        Serial.println(" MAGENTA");
        break;

      case WHITE:
        Serial.println(" WHITE");
        break;

      case OFF:
        Serial.println(" OFF");
        break;

      default:
        Serial.println(" Unknown Color Type");
        break;
    }
  }

  //Clears buffer if true; resets the status of all other leds
  if(clearStrip) 
    strip.clear();

  //sets all of the LEDs in the specified target to the same color
  for(int i = target[targetNum - 1].startingLedIndex; 
      i <= target[targetNum - 1].endingLedIndex; i++){
    strip.setPixelColor(i, color);
  }
}

/*
void setAllTargetColor (neoPixelColors color)
sets the color of ALL targets

Parameters: 
neoPixelColors color  - Color to set the targets to (from the neoPixelColors enum list)
*/
void MET::setAllTargetColor(neoPixelColors color){
  if(VERBOSE){
    Serial.print("All Targets:");
    switch(color){
      case RED:
        Serial.println(" RED");
        break;

      case GREEN:
        Serial.println(" GREEN");
        break;

      case BLUE: 
        Serial.println(" BLUE");
        break;
      
      case YELLOW:
        Serial.println(" YELLOW");
        break;
      
      case CYAN:
        Serial.println(" CYAN");
        break;
      
      case MAGENTA:
        Serial.println(" MAGENTA");
        break;

      case WHITE:
        Serial.println(" WHITE");
        break;

      case OFF:
        Serial.println(" OFF");
        break;

      default:
        Serial.println(" Unknown Color Type");
        break;
    }
  }
  strip.clear();
  for(int i = 1; i <= NUM_TARGETS; i++){
      setTargetColor(i, color, false);
  } 
}

/*
void displayTargets()
Physically displays the target as the set color in buffer

Parameters: 
None
*/
void MET::displayTargets(){
  delay(100);
  strip.show();
}

/*
void displayTargets(unsigned long time)
Physically displays the target for a set time
**RESETS BUFFER FOR ALL LEDS**

Parameters: 
unsigned long time    - How long to display the targets for (in Milliseconds)
*/
void MET::displayTargets(unsigned long time){
  delay(100);
  strip.show();
  delay(time);
  strip.clear();
  strip.show();
}
 
/*
void displaySpecificTarget(unsigned long time, int target)
Physically displays a specific target for a set time
**RESETS BUFFER FOR THE SPECIFIC TARGET LEDS**

Parameters: 
unsigned long time    - How long to display the target for (in Milliseconds)
int target            - Target to display (Indexed at 1 to match physical target number)
*/
 void MET::displaySpecificTarget(unsigned long time, int target){
  delay(100);
  strip.show();
  delay(time);
  setTargetColor(target, OFF, false);
  strip.show();
 }

/*
void turnOffTargets()
Physically turns off all targets
**RESETS BUFFER FOR ALL LEDS**

Parameters: 
NONE
*/
void MET::turnOffTargets(){
  strip.clear();
  strip.show();
}

/*
****************************************************************************************
Functions in charge of logic for the game modes
All function have no Return Values (void)

Game Modes:
1. QuickDraw
2. Search and Destroy
3. Blackout
4. Random
5. Twin Shot
****************************************************************************************
*/

/*
Game Mode 1: Quick Draw
All targets light green, first target hit turns red, others turn off. 
Timed till first target is hit.
*/
void MET::quickDraw(){
  resetMET();
  countMode = true;
  setAllTargetColor(GREEN);
  displayTargets();
  
  int targetHit = -1;
  while(!timeUp){
    updateTimerThread(&timerThread);
    targetHit = readSensors(false);
    if(targetHit != -1){
      timeUp = true;
    }
    delay(10);
  }
  setTargetColor(targetHit, RED, true);
  Serial.print("Target Hit: ");
  Serial.println(targetHit); 
  
  displayTargets();

  Serial.print("Final Time: ");
  Serial.print((elapsedTime / 1000.0), 3);
  Serial.println("s");

}
	
/*
Game Mode 2: Search and Destroy
One  random target lights green at a time, turns red briefly when hit. 
Next target lights up after first one is hit. 
Timed till x targets hit.
*/
void MET::SD(){
  resetMET();
  countMode = false;
  countDownTime = 10000.0;
  int scoreCount = 0;

  while(!timeUp){
    randomSeed(analogRead(0));
    int bullseye = TrueRandom.random(1, 3);
    strip.clear();
    setTargetColor(bullseye, GREEN, true);
    displayTargets();

    while(readSensors(true) != bullseye){
      updateTimerThread(&timerThread);
      if(timeUp)
        break;
    }

    if(!timeUp){
      setTargetColor(bullseye, RED, true);
      displayTargets(800);
      scoreCount++;
    }
  }
  turnOffTargets();
  Serial.print("Score: ");
  Serial.println(scoreCount);
}

/*
Gamemode 3: Blackout
All targets light green, turns red briefly when hit, then turns off. 
Timed until all targets are hit.
*/
void MET::blackout(){
 resetMET();
 countMode = true;
 setAllTargetColor(GREEN);
 displayTargets();

 while(!allTargetsHit()){
  updateTimerThread(&timerThread);
  int targetHit = readSensors(false);
  if(targetHit != -1){
    setTargetColor(targetHit, RED, false);
    displaySpecificTarget(800, targetHit);
  }
 }

Serial.print("Final Time: ");
Serial.print(elapsedTime / 1000, 3);
Serial.println("s");
}

/*
Gamemode 4: Random
One random target lights up green, turns red when hit. 
Next target lights up after 1.5 seconds or when the first is hit, whichever comes first. 
Scored after x seconds.
*/
void MET::random(){
}

/*
Gamemode 5: Twin Shot 
Two random targets light up green in pairs, turns red briefly when hit. 
Both targets must be hit for next pair to light up. 
Scored after x seconds.
*/
void MET::twin(){

}

/*
****************************************************************************************
Helper functions to facilitate reading input from sensors and checking if all sensors are hit
****************************************************************************************
*/

/*
int readSensors(bool reset)
Reads if sensors detect any unique inputs.

Return type:
int    - the number of the target that was hit

Parameters: 
bool reset - true:   resets the "currentStatus" for the sensor to allow reuse
             false:  sets "currentStatus" sensor so further readings do not occur 
*/
int MET::readSensors(bool reset){
  int targetNum = -1;

  for(int i = 0; i < NUM_TARGETS; i++){
   int tempStatus = digitalRead(target[i].SENSOR_PIN);
    if(target[i].currentStatus == LOW && tempStatus == HIGH){
      if(!reset){
        target[i].currentStatus = HIGH;
      }
      targetNum = i + 1;
      return targetNum;
    }
  }

  return -1;
}


/*
bool allTargetsHit()
Checks if all targets have been hit

Return type:
bool    - Returns true if all targets are hit

Parameters: 
NONE
*/
bool MET::allTargetsHit(){
  for(int i = 0; i < NUM_TARGETS; i++){
    if(target[i].currentStatus != HIGH){
      return false;
    }
  }
  return true;
}


/*
****************************************************************************************
Miscelaneous Helper functions 
****************************************************************************************
*/

/*
void resetMet()
resets variables to default values for the next game.

Parameters:
NONE
*/
void MET::resetMET(){
  for(int i = 0; i < NUM_TARGETS; i++){
    target[i].currentStatus = LOW;
  }
  strip.clear();
  timeUp = false;
}

/*
int updateTimerThread(struct pt* pt1)
Protothread function to count up for time trial gamemodes, or count down for timed game modes

Return Type:
int     -To check function status; UNUSED

Parameters:
struct pt* pt1 - The address of a Protothread object
*/
int MET::updateTimerThread(struct pt* pt1){
  PT_BEGIN(pt1);
  
  //Track the last update time
  static unsigned long lastMillis = millis();
  static unsigned long deltaTime = 0;
  static unsigned long timeRemain = countDownTime;
  while(!timeUp){
    unsigned long currentMillis = millis();

    if (currentMillis >= lastMillis) {
    deltaTime = currentMillis - lastMillis;
    } 
    else {
    // handle overflow scenario
    deltaTime = (4294967295 - lastMillis) + currentMillis + 1;
    }


    if(deltaTime > 0.0){
      lastMillis = currentMillis;

      //Check if we are counting up or down
      if(countMode){
        elapsedTime += deltaTime;
      }
      else{
        timeRemain -= deltaTime;
        if(timeRemain <= 0.0 || timeRemain > countDownTime ){
          timeRemain = 0.0;
           timeUp = true;
        }
      }
    }
    Serial.print("Time: ");
    Serial.print(countMode ? (elapsedTime / 1000.0) : (timeRemain / 1000.0), 3);
    Serial.println("s");
    
    PT_YIELD(pt1);
  }
  PT_END(pt1);
}