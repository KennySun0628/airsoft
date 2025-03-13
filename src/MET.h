//MET.h
//header file for MET and target class
//Kenny Sun
#pragma once

#include <Adafruit_NeoPixel.h>
#include <Chrono.h>
#include <pt.h>

#define VERBOSE true 

//Count down time before game
#define START_COUNTDOWN 3000.0

//Debounce time between sensor reads
#define DEBOUNCE 50

#define MILLI_IN_SECONDS 1000.0

//Number of LEDs per Target
#define TARGET_NUM_LED 4

//Number of Targets in the grid
#define NUM_TARGETS 3 

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

#define OUTPUT_PIN_COUNT 6 
#define INPUT_PIN_COUNT 16
 

enum neoPixelColors {
  RED     = 0xFF0000,
  ORANGE  = 0xFFA500,
  GREEN   = 0x00FF00,
  LGREEN  = 0x44FF44,
  BLUE    = 0x0000FF,
  YELLOW  = 0xFFFF00,
  CYAN    = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  PURPLE  = 0xBF40BF,
  WHITE   = 0xFFFFFF,
  OFF     = 0x000000
};

#define ACTIVE_TARGET_COLOR GREEN 


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
  bool timeUp = false;
  unsigned long startTime;
  unsigned long elapsedTime = 0.0;
  unsigned long countDownTime = 0.0;
  bool countMode; //True = count up; False = count down
  bool timerResetFlag = true;
  const int inputPins[INPUT_PIN_COUNT] = {2, 4, 12, 13, 14, 15, 16, 17, 25, 26, 32, 33, 34, 35, 36, 39};
  const int outputPins[OUTPUT_PIN_COUNT] = {5, 22, 23, 25, 26, 27};

  
  target_s target[NUM_TARGETS] = {};
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
  void startCountdown();
  int generateRandom(int);
};
