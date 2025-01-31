//MET.h
//header file for MET and target class
//Kenny Sun

#include <Adafruit_NeoPixel.h>
#include <Chrono.h>

#define TARGET_LED_PIN 13
//Number of LEDs per Target
#define TARGET_NUM_LED 1
//Number of Targets in the grid
#define NUM_TARGETS 4

enum neoPixelColors {
  RED   = 0xFF0000,
  GREEN = 0x00FF00,
  BLUE  = 0x0000FF,
  YELLOW = 0xFFFF00,
  CYAN  = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  WHITE = 0xFFFFFF,
  LED_OFF   = 0x000000
};

enum status{
  OFF,
  ON,
  HIT
};

struct target_s {
  int LED_INDEX[TARGET_NUM_LED];
  int SENSOR_PIN;
  status currentStatus;
};

class MET {
public:
  MET();
  ~MET();
  void run(int gameMode);
  bool isRunning;

private:
  Chrono timer;
  target_s target[NUM_TARGETS];
  Adafruit_NeoPixel strip;
  void quickDraw();
  void SD();
  void practice();
  void random();
  void twin();
  void setTargetColor(int, neoPixelColors);
  void turnOffTargets();
  void displayTargets() const;
  void displayTargets(unsigned long) const;
  
};
