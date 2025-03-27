//MET.cpp
//Implementation for MET class
//Kenny Sun

#include "MET.h"
#include "log.h"
using namespace std;

//Constructor
MET::MET(){
  
  PT_INIT(&timerThread);
  int targetsRemaining = NUM_TARGETS;
  const int targetsPerStrip = (NUM_TARGETS % NUM_ROWS) == 0 ? NUM_TARGETS / NUM_ROWS : (NUM_TARGETS / NUM_ROWS) + 1;
  int pin = -1;

  if(NUM_ROWS > OUTPUT_PIN_COUNT){
    sendLog("ERROR - NOT ENOUGH OUTPUT PINS");
    return;
  }
  //initialize each row of LEDs
  for(int i = 0; i < NUM_ROWS; i++){
    //Ternary statements check if there are more total targets remaining than the amount of targets there should be in each strip
    //If there true, use the number of targest per strip
    //If false, uses number of targets remaining. This accounts for remainders if NUM_TARGETS is not evenly divisible by NUM_ROWS
    if (i >= 0 && i < OUTPUT_PIN_COUNT) {
      // Safe to access pinArray[i]
      pin = outputPins[i];
      if(VERBOSE){
        sendLog("Pin: " + (String)pin );
      }
    } 
    else {
      sendLog("Pin Array Out of Bounds Error");
      return;
    }

    if(pin == -1){
      sendLog("Invalid Output Pin Number");
      return;
    }
    else{
      strip[i] = new Adafruit_NeoPixel( (targetsRemaining > targetsPerStrip) ? (targetsPerStrip * TARGET_NUM_LED) : (targetsRemaining * TARGET_NUM_LED), pin, NEO_GRB + NEO_KHZ800);
      if(VERBOSE){
        sendLog("Neopixel Strip Created");
        
        sendLog("LED Pin: " + String(pin));
      }

      strip[i] -> setBrightness(LED_BRIGHTNESS);
      if(VERBOSE){
        sendLog("Neopixel Brightness Set: " + String(LED_BRIGHTNESS));
      }
      strip[i] -> begin();
      if(VERBOSE){
        sendLog("Neopixel Strip Began");
      }
    }

    if(VERBOSE){
      sendLog("Row Index: " + String(i));
    }

    //initialize every target that belongs in this row
    for(int j = 0; j < ((targetsRemaining > targetsPerStrip) ? targetsPerStrip : targetsRemaining); j++){
      int targetNumber = (targetsPerStrip * i) + j;
      
      if (targetNumber >= NUM_TARGETS || targetNumber >= INPUT_PIN_COUNT) {
        sendLog("ERROR: Target number out of bounds!");
        return; 
      }

      target[targetNumber].SENSOR_PIN = inputPins[targetNumber];
      if(VERBOSE){
        sendLog("Target Index: " + String(j));
        sendLog("Target Number: " + String(targetNumber + 1));
        sendLog("Sensor Pinout: " + String(target[targetNumber].SENSOR_PIN));
      }
      //pinMode(target[targetNumber].SENSOR_PIN, INPUT);
      
      target[targetNumber].currentStatus = LOW;
      target[targetNumber].rowIndex = i;
      target[targetNumber].startingLedIndex = j * TARGET_NUM_LED;
      target[targetNumber].endingLedIndex =  target[targetNumber].startingLedIndex + (TARGET_NUM_LED - 1);
      

    }
    //Number of targets remaining gets decremented after every loop by the amount per strip   
    targetsRemaining -= targetsPerStrip;
  }
  resetMET();
}

//destructor
MET::~MET(){
  resetMET();
  for(int i = 0; i < NUM_ROWS; i++){
    delete strip[i];
    strip[i] = nullptr;
  }
}

//Main function to facilitate modes changes
void MET::run(int gameMode){
  startCountdown(); 
  sendLog("Start!");
  
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
			random();
			break;
		case 5:
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
    Serial.print("Target: " + String(targetNum));
    printColor(color);
  }
  
  //check bounds
  if(targetNum < 1 || targetNum > NUM_TARGETS)
    return;

  //Clears buffer if true; resets the status of all other leds
  if(clearStrip){ 
    for(int i = 0; i < NUM_ROWS; i++){
      strip[i] -> clear();

    }
  }
 
  //sets all of the LEDs in the specified target to the same color
  for(int i = target[targetNum - 1].startingLedIndex; i <= target[targetNum - 1].endingLedIndex; i++){
    strip[target[targetNum - 1].rowIndex] -> setPixelColor(i, color);
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
    sendLog("Set All Targets");
  }

  for(int i = 0; i < NUM_ROWS; i++){
    strip[i] -> clear();
  }

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
  delayTimer(10);
  for(int i = 0; i < NUM_ROWS; i++){
    strip[i] -> show();
  }
}

/*
void displayTargets(unsigned long time)
Physically displays the target for a set time
**RESETS BUFFER FOR ALL LEDS**

Parameters: 
unsigned long time    - How long to display the targets for (in Milliseconds)
*/
void MET::displayTargets(unsigned long time){
  displayTargets();
  delayTimer(time);
  turnOffTargets();
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
  for(int i = 0; i < NUM_ROWS; i++){
    strip[i] -> show();
  }
  delayTimer(time);
  setTargetColor(target, OFF, false);
  for(int i = 0; i < NUM_ROWS; i++){
    strip[i] -> show();
  }
 }

/*
void turnOffTargets()
Physically turns off all targets
**RESETS BUFFER FOR ALL LEDS**

Parameters: 
NONE
*/
void MET::turnOffTargets(){
  for(int i = 0; i < NUM_ROWS; i++){
    strip[i] -> clear();
    strip[i] -> show();
  }
}

/*
void printColor(neoPixelColors color)
Helper function to display which color is used in plain-text

Parameters:
neoPixelColors color - Color to print
*/
void MET::printColor(neoPixelColors color){

  switch(color){
    case RED:
      sendLog(" RED");
      break;

    case GREEN:
      sendLog(" GREEN");
      break;

    case BLUE: 
      sendLog(" BLUE");
      break;
    
    case YELLOW:
      sendLog(" YELLOW");
      break;
    
    case CYAN:
      sendLog(" CYAN");
      break;
    
    case MAGENTA:
      sendLog(" MAGENTA");
      break;

    case WHITE:
      sendLog(" WHITE");
      break;

    case OFF:
      sendLog(" OFF");
      break;

    default:
      sendLog(" Unknown Color Type");
      break;
  }
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
***ADD LEFT AND RIGHT HALF RANDOMIZED TARGET***
*/
void MET::quickDraw(){
  resetMET();
  countMode = true;
  if(VERBOSE){
    sendLog("Gamemode: #1 - Quick Draw");
  }
 
  int bullseye = generateRandom(NUM_TARGETS);
  setTargetColor(bullseye, ACTIVE_TARGET_COLOR, true);
  displayTargets();
  
  int targetHit = -1;
  while(!timeUp){
    updateTimerThread(&timerThread);
    targetHit = readSensors(false);
    if(targetHit == bullseye){
      break;
    }
  }
  setTargetColor(targetHit, RED, true);
  if(VERBOSE){
    sendLog("Target Hit: " + String(targetHit)); 
  }
  
  displayTargets();

  sendLog("Final Time: " + String((elapsedTime / MILLI_IN_SECONDS)) + "s");
  resetMET(); 
}
	
/*
Game Mode 2: Search and Destroy
One  random target lights green at a time, turns red briefly when hit. 
Next target lights up after first one is hit. 
Timed till x targets hit.
***ADD INCREMENTAL LIT TARGET NUMBER***
*/
void MET::SD(){
  resetMET();
  countMode = true;

  if(VERBOSE){
    sendLog("Gamemode: #2 - Search and Destroy");
    sendLog("Time Limit: " + String(countDownTime / MILLI_IN_SECONDS) + "sec");
  }
  int scoreCount = 0;

  while(scoreCount < SD_SCORE){
    int bullseye = generateRandom(NUM_TARGETS);
    setTargetColor(bullseye, ACTIVE_TARGET_COLOR, true);
    displayTargets();

    while(readSensors(true) != bullseye){
      updateTimerThread(&timerThread);
    }

      setTargetColor(bullseye, RED, true);
      displayTargets(200);
      scoreCount++;
  }
  turnOffTargets();
  sendLog("Final Time: " + String((elapsedTime / MILLI_IN_SECONDS)) + "s");
  resetMET(); 
}

/*
Gamemode 3: Blackout
All targets light green, turns red briefly when hit, then turns off. 
Timed until all targets are hit.
*/
void MET::blackout(){
 resetMET();
 if(VERBOSE){
  sendLog("Gamemode: #3 - Blackout");
 }
 countMode = true;
 setAllTargetColor(ACTIVE_TARGET_COLOR);
 displayTargets();

 while(!allTargetsHit()){
  updateTimerThread(&timerThread);
  int targetHit = readSensors(false);
  if(targetHit != -1){
    setTargetColor(targetHit, RED, false);
    displaySpecificTarget(200, targetHit);
  }
 }

  sendLog("Final Time: " + String((elapsedTime / MILLI_IN_SECONDS)) + "s");
  resetMET(); 
}

/*
Gamemode 4: Random
One random target lights up green, turns red when hit. 
Next target lights up after 1.5 seconds or when the first is hit, whichever comes first. 
Scored after x seconds.
*/
void MET::random(){
  resetMET();
  countMode = false;
  countDownTime = RANDOM_TIME;
  if(VERBOSE){
    sendLog("Gamemode: #4 - Random");
    sendLog("Time Limit: " + String(countDownTime / MILLI_IN_SECONDS) + "sec");
  }
  
  static unsigned long prevTime = 0;
  static unsigned long delta = 0;
  int scoreCount = 0;
  float timeoutSeconds = VERBOSE ? TIMEOUT - RANDOM_CALIBRATION: TIMEOUT;
  

  while(!timeUp){
    int bullseye = generateRandom(NUM_TARGETS);
    setTargetColor(bullseye, ACTIVE_TARGET_COLOR, true);
    displayTargets();

    bool timeout = false;

    if(VERBOSE){
      sendLog("Bullseye: " + String(bullseye));
      sendLog("Score: " + String(scoreCount));
    }

    prevTime = millis();
    delta = 0;
    while(readSensors(true) != bullseye){
      unsigned long currentTime = millis();

      if(currentTime >= prevTime){
        delta = (currentTime - prevTime);
      }
      else{
        //Handel overflow scenario
        delta = ((4294967295 - prevTime) + currentTime + 1);
      }
      if(delta > (timeoutSeconds * MILLI_IN_SECONDS)){
        timeout = true;
        break;
      }

      updateTimerThread(&timerThread);
      if(timeUp)
        break;
    }
    if(VERBOSE){
      if(timeout){
        sendLog("Timed Out: True");
      }
      else{
        sendLog("Timed Out: False");
      }
    }

    if(!timeUp && !timeout){
      scoreCount++;
    }
    setTargetColor(bullseye, RED,  true);
    displayTargets(200);
  }
  turnOffTargets();
  sendLog("Score: " + String(scoreCount));
  resetMET(); 
}

/*
Gamemode 5: Twin Shot 
Two random targets light up green in pairs, turns red briefly when hit. 
Both targets must be hit for next pair to light up. 
Scored after x seconds.
*/
void MET::twin(){
resetMET();
  countMode = false;
  countDownTime = TWIN_TIME;
  if(VERBOSE){
    sendLog("Gamemode: #5 - Twin Shot");
    sendLog("Time Limit: " + String(countDownTime / MILLI_IN_SECONDS) + "sec");
  }
  
  static unsigned long prevTime = 0;
  static unsigned long delta = 0;
  int scoreCount = 0;
  float timeoutSeconds = VERBOSE ? (TIMEOUT - TWIN_CALIBRATION) * 2: TIMEOUT * 2;
  int counter = 0; 
  int targetHit = -1;
  bool hit1 = false;
  bool hit2 = false;

  while(!timeUp){
    int bullseye1 = generateRandom(NUM_TARGETS);
    setTargetColor(bullseye1, ACTIVE_TARGET_COLOR, true);
    int bullseye2 = -1;
    do{
      bullseye2 = generateRandom(NUM_TARGETS);
    } while(bullseye1 == bullseye2);
    setTargetColor(bullseye2, ACTIVE_TARGET_COLOR, false);
    displayTargets();

    bool timeout = false;
    

    if(VERBOSE){
      sendLog("Bullseye 1: " + String(bullseye1));
      sendLog("Bullseye 2: " + String(bullseye1));
    }

    prevTime = millis();
    delta = 0;
    targetHit = -1;
    counter = 0;
    hit1 = hit2 = false;

    while((targetHit = readSensors(true))){
      //Checks which targets are hit
      if((targetHit == bullseye1 && hit1 == false) || (targetHit == bullseye2 && hit2 == false)){
        if(targetHit == bullseye1){
          hit1 = true;
        }
        else{
          hit2 = true;
        }

        if(++counter == 2){
          setTargetColor(targetHit, RED, true);
          displayTargets(100);
          break;
        }        
        setTargetColor(targetHit, RED, false);
        displayTargets();
      }

      unsigned long currentTime = millis();
      //Time calculations for timeouts
      if(currentTime >= prevTime){
        delta = (currentTime - prevTime);
      }
      else{
        //Handel overflow scenario
        delta = ((4294967295 - prevTime) + currentTime + 1);
      }
      if(delta > (timeoutSeconds * MILLI_IN_SECONDS)){
        timeout = true;
        break;
      }

      //runs timer thread
      updateTimerThread(&timerThread);
      if(timeUp)
        break;
    }
    if(VERBOSE){
      if(timeout){
        sendLog("Timed Out: True");
      }
      else{
        sendLog("Timed Out: False");
      }
    }

    if(!timeUp && !timeout){
      scoreCount++;
    }
  }
  turnOffTargets();
  sendLog("Score: " + String(scoreCount));
  resetMET(); 
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
      delayTimer(DEBOUNCE);
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
  turnOffTargets();
  for(int i = 0; i < NUM_TARGETS; i++){
    target[i].currentStatus = LOW;
  }
  for(int i = 0; i < NUM_ROWS; i++)
  strip[i] -> clear();
  timeUp = false;
  timerResetFlag = true;
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
  static unsigned long lastMillis;
  
  static unsigned long deltaTime = 0;
  static unsigned long timeRemain = countDownTime;
  while(!timeUp){
    unsigned long currentMillis = millis();
    if(timerResetFlag == true){
        lastMillis = millis();
        elapsedTime = 0;
        timeRemain = countDownTime;
        timeUp = false;
        timerResetFlag = false;
      }
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
    sendLog("Time: " + String(countMode ? (elapsedTime / MILLI_IN_SECONDS) : (timeRemain / MILLI_IN_SECONDS)) + "s");
    
    PT_YIELD(pt1);
  }
  PT_END(pt1);
}

/*
void delayTimer(unsigned long delayTime)
same as the arduino delay() function, but allows the timer protothread to continue in the background.
*Doesn't stall displaying the timer*

Return Type:
void

Parameters:
unsigned long delayTime - time in milliseconds to delay for
*/
void MET::delayTimer(unsigned long delayTime){
  unsigned long prevTime = millis();
  unsigned long delta = 0;
  
  while(delta < delayTime){
    
    unsigned long currentTime = millis();

    if (currentTime >= prevTime) {
    delta = currentTime - prevTime;
    } 
    else {
    // handle overflow scenario
    delta = (4294967295 - prevTime) + currentTime + 1;
    }
    
    updateTimerThread(&timerThread);
  }
}

/*
void startCountdown()
Sets a count down for (START_COUNTDOWN) milliseconds.
All LEDs Displays Red -> Yellow -> Green to match timer.

Return Type:
void

Parameters:
NONE
*/
void MET::startCountdown(){
  resetMET();
  countDownTime = START_COUNTDOWN;
  if(VERBOSE){
    sendLog("COUNTDOWN" + String(countDownTime));
  }
  countMode = false;
  while(!timeUp){
    setAllTargetColor(RED);
    displayTargets((countDownTime / 3));
    setAllTargetColor(YELLOW);
    displayTargets((countDownTime / 3));
    setAllTargetColor(GREEN);
    displayTargets((countDownTime / 3));
  }
  turnOffTargets();
  delay(30);
  resetMET();
}

/*
int generateRandom(int max)
Generates a pseudo-random number from 1 - max, reseeded every time.

Return Type:
int

Parameters:
int max - upper bound for the random generation
*/
int MET::generateRandom(int max){
  randomSeed(esp_random());
  return ::random(1, max + 1);
}