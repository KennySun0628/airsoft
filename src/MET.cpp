//MET.cpp
//Implementation for MET class
//Kenny Sun

#include "MET.h"
using namespace std;

//constructor
MET::MET(): isRunning(false), strip(TARGET_NUM_LED * NUM_TARGETS, TARGET_LED_PIN, NEO_GRB + NEO_KHZ800){

  for(int i = 0; i < NUM_TARGETS; i++){
    target[i].SENSOR_PIN = (i + 3);
    pinMode(target[i].SENSOR_PIN, INPUT);
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
  for(int i = target[targetNum - 1].startingLedIndex; 
      i <= target[targetNum - 1].endingLedIndex; i++){
    strip.setPixelColor(i, color);
  }
}

//Sets ALL targets to specified color
void MET::setAllTargetColor(neoPixelColors color){
  //Serial.println(color);
  strip.clear();
  for(int i = 1; i <= NUM_TARGETS; i++){
      setTargetColor(i, color);
  } 
}

void MET::displayTargets(){
  delay(100);
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

  setAllTargetColor(GREEN);
  displayTargets();
  
  timer.restart();

  int hitSensor; 
  while(hitSensor != HIGH){
    hitSensor = digitalRead(2);
  }
  float elapsedTime = timer.elapsed() / (float) 1000;
 
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
  strip.clear();
  strip.setPixelColor(2, OFF);
  strip.show();
  
}

void MET::random(){

  setAllTargetColor(GREEN);
  displayTargets();
  
  timer.restart();

  int targetHit = readSensors(); 
  Serial.println(targetHit); 
  float elapsedTime = timer.elapsed() / (float) 1000;
  if(targetHit ==  1){
    strip.setPixelColor(0, RED);
    strip.setPixelColor(1, RED);
  }
  else{
    strip.setPixelColor(2, RED);
    strip.setPixelColor(3, RED);
  }
  
  displayTargets();

  Serial.print(elapsedTime, 3);
  Serial.println("s");

}

void MET::twin(){
	displayTargets();

}

int MET::readSensors(){
  int targetNum ;
  bool hit = false;

  while(!hit){
    for(int i = 0; i < 3; i++){
      target[i].currentStatus = digitalRead(target[i].SENSOR_PIN);
      if(target[i].currentStatus == HIGH){
        targetNum = i + 1;
        hit = true;
      }
    }
  }
  return targetNum;
}
