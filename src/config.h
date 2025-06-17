#pragma once
#ifndef CONFIG_H 
#define CONFIG_H 

#define VERBOSE false 

//Count down time before game
#define START_COUNTDOWN 3000.0

//Debounce time between sensor reads
#define DEBOUNCE 50

#define MILLI_IN_SECONDS 1000.0

//Number of LEDs per Target
#define TARGET_NUM_LED 4

//Number of Targets in the grid
#define NUM_TARGETS 1 

//Number of Rows of LEDs
#define NUM_ROWS 1 

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

//Initial Threshold Value for Sensors
#define SENSOR_INITIAL_VALUE 4000
//Average Threshold Value for Sensors
#define SENSOR_AVERAGE_VALUE 1800
//Number of Sensor Polls to Average
#define SENSOR_POLL_AMOUNT 5
//Delay Between Sensor Polls
#define SENSOR_POLL_DELAY 5

//Color for Active Target
#define ACTIVE_TARGET_COLOR PURPLE 

#endif