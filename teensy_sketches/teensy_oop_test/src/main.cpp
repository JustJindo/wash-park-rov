#include "ESC.h"
#include "Gamepad.h"

void setup() {
  analogWriteResolution(8); // set pulse width resolution to 8 bits (0-255)

  Serial.begin(9600); // initialize serial communication at 9600 baud rate
} // end setup

void loop() {
  ESC esc(Port_Horz); // create an instance of the ESC class with Port_Horz as the signal pin

  esc.throttle(255); // send a full PWM throttle signal to the ESC
  esc.getTelemetry(); // printout telemetry from ESC
} // end loop