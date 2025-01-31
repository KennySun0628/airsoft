//Sketch1.cpp
#include <Arduino.h>
#include "MET.h"

MET m;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  m.run(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}
