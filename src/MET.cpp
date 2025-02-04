//MET.cpp
//Implementation for MET class
//Kenny Sun

#include <TrueRandom.h>
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

//Sets specific target to specified color
void MET::setTargetColor(int targetNum, neoPixelColors color, bool clearStrip){
  //Serial.println(color);
  //Clears buffer if true; resets the status of all other leds
  if(clearStrip) 
    strip.clear();

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
      setTargetColor(i, color, false);
  } 
}

void MET::displayTargets(){
  delay(100);
  strip.show();
}

void MET::displayTargets(unsigned long time){
  delay(100);
  strip.show();
  delay(time);
  strip.clear();
  strip.show();
}

void MET::turnOffTargets(){
  strip.clear();
  strip.show();
}

//Gamemode 1: Quick Draw
void MET::quickDraw(){

  setAllTargetColor(GREEN);
  displayTargets();
  
  timer.restart();

  int targetHit = readSensors(); 
  Serial.print("Target Hit: ");
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
	
//Gamemode 2: Search and Destroy
void MET::SD(){
  timer.restart();
  int scoreCount = 0;

  while((timer.elapsed() / 1000 ) <= 5){
    randomSeed(analogRead(0));
    int bullseye = TrueRandom.random(1, 3);
    strip.clear();
    setTargetColor(bullseye, GREEN, true);
    displayTargets();

    while(readSensors() != bullseye){}

    setTargetColor(bullseye, RED, true);
    displayTargets(1000);
    scoreCount++;
  }
  turnOffTargets();
  Serial.print("Score: ");
  Serial.println(scoreCount);
}

//Gamemode 3: Blackout 
void MET::blackout(){
  strip.clear();
  strip.setPixelColor(2, OFF);
  strip.show();
  
}

void MET::random(){
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
