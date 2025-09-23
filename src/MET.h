//MET.h
//header file for MET and target class
//Kenny Sun
#pragma once
#ifndef MET_H
#define MET_h

#include <Adafruit_NeoPixel.h>
#include <Chrono.h>
#include <pt.h>

#include "config.h"

enum neoPixelColors {
  RED     = 0xFF0000,
  ORANGE  = 0xFFA500,
  GREEN   = 0x00FF00,
  LGREEN  = 0x64FF3C,
  BLUE    = 0x0000FF,
  YELLOW  = 0xFFFF00,
  CYAN    = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  PURPLE  = 0xBF40BF,
  WHITE   = 0xFFFFFF,
  OFF     = 0x000000
};

struct target_s {
  //The Index of the first led in the target
  int startingLedIndex;

  //The Index of the last led in the target
  int endingLedIndex;

  //pin number of the sensor
  int SENSOR_PIN;

  //Status of the sensor
  bool hit = false;

  //Index of which row target is in
  int rowIndex;

};

class MET {
public:
  MET();
  ~MET();
  void run(int gameMode);
  

private:
  struct pt timerThread;
  bool timeUp = false;
  unsigned long startTime;
  unsigned long elapsedTime = 0.0;
  unsigned long countDownTime = 0.0;
  bool countMode; //True = count up; False = count down
  bool timerResetFlag = true;

  #define OUTPUT_PIN_COUNT 6 
  #define INPUT_PIN_COUNT 16
  //const int inputPins[INPUT_PIN_COUNT] = {2, 4, 12, 13, 14, 15, 16, 17, 25, 26, 32, 33, 34, 35, 36, 39};
  const int inputPins[INPUT_PIN_COUNT] = {36, 39, 34, 35, 32, 33};
  const int outputPins[OUTPUT_PIN_COUNT] = {5, 22, 23, 25, 26, 27};

  
  target_s target[NUM_TARGETS] = {};
  Adafruit_NeoPixel* strip[NUM_ROWS];
  void quickDraw();
  void SD();
  void blackout();
  void random();
  void twin();

  void calibrateTargets();
  void testAnalogInput();

  void setTargetColor(int, neoPixelColors, bool);
  void setAllTargetColor(neoPixelColors);

  void turnOffTargets();
  void displayTargets();
  void displayTargets(unsigned long);

  void printColor(neoPixelColors);

  int readSensors(bool);
  bool allTargetsHit();
  void displaySpecificTarget(unsigned long, int);
  void resetMET();
  int updateTimerThread(pt* pt1);
  void delayTimer(unsigned long);
  void startCountdown();
  int generateRandom(int);
};

#endif