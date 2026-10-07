#include <Arduino.h>

#include "ESC.h"


void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 baud rate
}

void loop() {
  ESC esc(Port_Horz); // create an instance of the ESC class with Port_Horz as the signal pin

  esc.throttle(255); // send a full PWM throttle signal to the ESC
  esc.getTelemetry(); // printout telemetry from ESC
}