//Sketch1.cpp
#include <Arduino.h>
#include "MET.h"

MET m;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  m.run(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}
