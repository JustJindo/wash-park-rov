#include <stdio.h>
// include Teensy servo library
#include <PWMServo.h>

#define signalPin 37

/*
APISQUEEN 12-24V 45A Bi-directional ESC
>500Hz Refresh
1-2ms PWM Signal
1.5-2ms Forward
1-1.5ms Backward
1.5ms Zero Throttle
*/

void setup() {
  // set PWM frequency to 500 Hz
  analogWriteFrequency(signalPin, 500);
  // set pulse width resolution to 8 bits
  analogWriteResolution(8);
  // begin serial communication at 9600 bits/sec
  Serial.begin(9600);
  // 3 second delay written into calibration for stability
  calibrate_esc();
}

/*
Relationship between value sent to signal pin and resultduty cycle is described below:

W Pulse Width
C Control Signal
R Resolution
F Frequency
W = C / R*F
W = C / 256*500
*/

void loop() {
  // put your main code here, to run repeatedly:

}

// calibrate ESC by sending 1.5ms signal for zero throttle at power on, then forward throttle, then backward.
void calibrate_esc() {
  Serial.println("Calibrating ESCs...");

  // begin at zero throttle for three seconds
  analogWrite(signalPin, 192);
  delay(3000);
  // send forward throttle for one second
  Serial.println("Forwards...");
  analogWrite(signalPin, 255);
  delay(1000);
  // return to zero throttle for one second
  Serial.println("Pause...");
  analogWrite(signalPin, 192);
  delay(1000);
  // send backward throttle for one second
  Serial.println("Backwards...");
  analogWrite(signalPin, 128);
  delay(1000);
  // return to zero throttle
  analogWrite(signalPin, 192);

  Serial.println("Calibration complete.");
  delay(3000);
}