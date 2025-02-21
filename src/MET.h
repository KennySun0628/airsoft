//MET.h
//header file for MET and target class
//Kenny Sun
#pragma once

#include <Adafruit_NeoPixel.h>
#include <Chrono.h>
#include <pt.h>

#define VERBOSE false

#define TARGET_LED_PIN 13

//Number of LEDs per Target
#define TARGET_NUM_LED 1

//LED brightness
#define LED_BRIGHTNESS 150

//Number of Targets in the grid
#define NUM_TARGETS 2 

//Timer for the Game Modes (in milliseconds)
#define SD_TIME 8000.0
#define RANDOM_TIME 8000.0
#define TWIN_TIME 8000.0



enum neoPixelColors {
  RED   = 0xFF0000,
  GREEN = 0x00FF00,
  BLUE  = 0x0000FF,
  YELLOW = 0xFFFF00,
  CYAN  = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  WHITE = 0xFFFFFF,
  OFF   = 0x000000
};


struct target_s {
  //The Index of the first led in the target
  int startingLedIndex;

  //The Index of the last led in the target
  int endingLedIndex;

  //pin number of the sensor
  int SENSOR_PIN;

  //Status of the piezosensor
  int currentStatus;
};

class MET {
public:
  MET();
  ~MET();
  void run(int gameMode);
  

private:
  struct pt timerThread;
  bool timeUp;
  unsigned long startTime;
  unsigned long elapsedTime = 0.0;
  unsigned long countDownTime = 0.0;
  bool countMode; //True = count up; False = count down

  
  target_s target[NUM_TARGETS];
  Adafruit_NeoPixel strip;
  void quickDraw();
  void SD();
  void blackout();
  void random();
  void twin();

  void setTargetColor(int, neoPixelColors, bool);
  void setAllTargetColor(neoPixelColors);

  void turnOffTargets();
  void displayTargets();
  void displayTargets(unsigned long);
  int readSensors(bool);
  bool allTargetsHit();
  void displaySpecificTarget(unsigned long, int);
  void resetMET();
  int updateTimerThread(pt* pt1);
};
