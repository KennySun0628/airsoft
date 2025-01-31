//MET.cpp
//Implementation for MET class
//Kenny Sun

#include "MET.h"
using namespace std;

//constructor
MET::MET(): isRunning(false), strip(TARGET_NUM_LED * NUM_TARGETS, TARGET_LED_PIN, NEO_GRB + NEO_KHZ800){

  for(int i = 0; i < NUM_TARGETS; i++){
    target[i].SENSOR_PIN = (i + 2);
    pinMode(i + 2, INPUT);
    target[i].startingLedIndex = i * (TARGET_NUM_LED);
    target[i].endingLedIndex =  target[i].startingLedIndex + (TARGET_NUM_LED - 1);
  }

  strip.begin();
}

//destructor
MET::~MET(){}

void MET::run(int gameMode){
	switch (gameMode){
		case 1: 
			quickDraw();
			break;
		case 2:
			SD();
			break;
		case 3:
			practice();
			break;
		case 4:
			random();
			break;
		case 5:
			twin();
			break;
	}
}

//Sets specific target to specified color
void MET::setTargetColor(int targetNum, neoPixelColors color){
  //Serial.println(color);
  
  //sets all of the leds in the specified target to the same color
  Serial.println(target[targetNum - 1].startingLedIndex);
  //Serial.println(target[targetNum - 1].endingLedIndex);
  for(int i = target[targetNum - 1].startingLedIndex; 
      i <= target[targetNum - 1].endingLedIndex; i++){
    strip.setPixelColor(i, color);
  }
}

//Sets ALL targets to specified color
void MET::setAllTargetColor(neoPixelColors color){
  Serial.println(color);
  strip.clear();
  for(int i = 1; i <= NUM_TARGETS; i++){
      setTargetColor(i, color);
    }
  strip.show();
  delay(100);
}

void MET::displayTargets(){
  strip.show();
}

void MET::displayTargets(unsigned long time){
  strip.show();
  delay(time);
  strip.clear();
  strip.show();
}

void MET::turnOffTargets(){
  strip.clear();
  strip.show();
}


void MET::quickDraw(){
  Serial.println("\nQuickdraw");
  turnOffTargets();
  delay(5000);
	for(int i = 1; i <= NUM_TARGETS; i++){
      setTargetColor(i, GREEN);
    }
  displayTargets();
  

  timer.restart();

  //int targetHit = readSensors();
  delay(2345);
  float elapsedTime = timer.elapsed() / (float) 1000;
  /*
  for(int i = 1; i <= NUM_TARGETS; i++){
    if(i != targetHit)
      setTargetColor(i, RED);
  }
  */
 setAllTargetColor(RED);
  displayTargets();

  Serial.print(elapsedTime, 3);
  Serial.println("s");
}
	

void MET::SD(){
  Serial.println("SD");
  turnOffTargets();
	setAllTargetColor(GREEN);
  displayTargets();
  delay(2500);
  turnOffTargets();
  delay(2500);
  setAllTargetColor(RED);
	displayTargets();
}

void MET::practice(){
	turnOffTargets();
}

void MET::random(){
	displayTargets();

}

void MET::twin(){
	displayTargets();

}

int MET::readSensors(){
  int targetNum ;

  while(true){
    for(int i = 0; i < NUM_TARGETS; i++){
      targetNum = i + 1;
      target[i].currentStatus = digitalRead(target[i].SENSOR_PIN);

      if(target[i].currentStatus == HIGH)
        break;
    }
  }
  return targetNum;
}
