//MET.h
//header file for MET and target class
//Kenny Sun
#pragma once

#include <Adafruit_NeoPixel.h>
#include <Chrono.h>
#include <pt.h>

#define VERBOSE false 

#define MILLI_IN_SECONDS 1000.0

//Largets pin for target leds strips **decremented for successive strips**
#define TARGET_LED_PIN 13

//Number of LEDs per Target
#define TARGET_NUM_LED 4

//Number of Targets in the grid
#define NUM_TARGETS 2 

//Number of Rows of LEDs
#define NUM_ROWS 2 

//LED brightness
#define LED_BRIGHTNESS 150

//Timer for the Game Modes (in milliseconds)
#define RANDOM_TIME 8000.0
#define TWIN_TIME 8000.0

//Score count for Search and Destroy
#define SD_SCORE 5

//Time between automatic target change for Random and Twin gamemodes
#define TIMEOUT 1.5f

//Calibration time used for synchronizing timeout time calculations (to account for internal function calls)
#define RANDOM_CALIBRATION 0.109f
#define TWIN_CALIBRATION 0.2f

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
  bool timeUp;
  unsigned long startTime;
  unsigned long elapsedTime = 0.0;
  unsigned long countDownTime = 0.0;
  bool countMode; //True = count up; False = count down

  
  target_s target[NUM_TARGETS];
  Adafruit_NeoPixel* strip[NUM_ROWS];
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

  void printColor(neoPixelColors);

  int readSensors(bool);
  bool allTargetsHit();
  void displaySpecificTarget(unsigned long, int);
  void resetMET();
  int updateTimerThread(pt* pt1);
  void delayTimer(unsigned long);
};
