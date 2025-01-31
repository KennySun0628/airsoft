//MET.cpp
//Implementation for MET class
//Kenny Sun

#include "MET.h"
using namespace std;

//constructor
MET::MET(): isRunning(false), strip(TARGET_NUM_LED * NUM_TARGETS, TARGET_LED_PIN, NEO_GRB + NEO_KHZ800){

  for(int i = 0; i < NUM_TARGETS; i++){
    target[i].SENSOR_PIN = (i);
    target[i].currentStatus = OFF;
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

void MET::setTargetColor(int targetNum, neoPixelColors color){
  //Serial.println(color);
  int startingLED = (targetNum - 1) * TARGET_NUM_LED;
  int endingLED = startingLED + (TARGET_NUM_LED - 1);
  for(int i = startingLED; i <= endingLED; i++){
    strip.setPixelColor(i, color);
  }
}

void MET::displayTargets(){
  strip.show();
}

void MET::displayTargets(unsigned long time){
  strip.show();
  delay(time);
  strip.clear();
}

void MET::turnOffTargets(){
  strip.clear();
}


void MET::quickDraw(){
  Serial.println("\nQuickdraw");
  turnOffTargets();
	for(int i = 1; i <= NUM_TARGETS; i++){
    setTargetColor(i, GREEN);
  }
  displayTargets();
  
  pinMode(3, INPUT);
  int sensorRead = digitalRead(3);
  timer.restart();

  while(sensorRead != HIGH){
    sensorRead = digitalRead(3);
  }
  
  float elapsedTime = timer.elapsed() / (float) 1000;
  for(int i = 1; i <= NUM_TARGETS; i++){
    setTargetColor(i, RED);
  }
  displayTargets();
  Serial.print(elapsedTime, 3);
  Serial.println("s");
}
	

void MET::SD(){
	displayTargets();
	
}

void MET::practice(){
	displayTargets();

}

void MET::random(){
	displayTargets();

}

void MET::twin(){
	displayTargets();

}


