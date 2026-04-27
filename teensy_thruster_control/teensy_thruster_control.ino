#include <stdio.h>
// include Teensy servo library
#include <PWMServo.h>

/*
Variables governing motor control are labeled according to the order described below:
Motor 1: Starboard Side Vertical
Motor 2: Port Side Vertical
Motor 3: Starboard Side Horizontal
Motor 4: Port Side Horizontal
*/

#define left_xPin A14 // 38
#define left_yPin A15 // 39
#define right_xPin A16 // 40
#define right_yPin A17 // 41
#define M1_signalPin 33
#define M2_signalPin 34
#define M3_signalPin 35
#define M4_signalPin 36

/*
APISQUEEN 12-24V 45A Bi-directional ESC
>500Hz Refresh
1-2ms PWM Signal
1.5-2ms Forward
1-1.5ms Backward
1.5ms Zero Throttle
*/

void setup() {
  // set PWM frequencies to 500 Hz
  analogWriteFrequency(M1_signalPin, 500);
  analogWriteFrequency(M2_signalPin, 500);
  analogWriteFrequency(M3_signalPin, 500);
  analogWriteFrequency(M4_signalPin, 500);
  // set pulse width resolution to 8 bits
  analogWriteResolution(8);
  // begin serial communication at 9600 bits/sec
  Serial.begin(9600);
  // 3 second delays written into calibrations for stability
  calibrate_esc(M1_signalPin);
  calibrate_esc(M2_signalPin);
  calibrate_esc(M3_signalPin);
  calibrate_esc(M4_signalPin);
} // end setup

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
  // read joystick values from pins
  int left_xValue = analogRead(left_xPin);
  int left_yValue = analogRead(left_yPin);
  int right_xValue = analogRead(right_xPin);
  int right_yValue = analogRead(right_yPin);
  // map the potentiometer axes' values ranging from 0-1023 to a valid pulse width range from 128-255
  int left_xPWM = map(left_xValue, 0, 1023, 128, 255);
  int left_yPWM = map(left_yValue, 0, 1023, 128, 255);
  int right_xPWM = map(right_xValue, 0, 1023, 128, 255);
  int right_yPWM = map(right_yValue, 0, 1023, 128, 255);

  // print returned joystick values
  Serial.printf("Left X: %i", left_xPWM);
  Serial.print("\t");
  Serial.printf("Left Y: %i", left_yPWM);
  Serial.print("\t");
  Serial.printf("Right X: %i", right_xPWM);
  Serial.print("\t");
  Serial.printf("Right Y: %i", right_yPWM);
  Serial.println("");
} // end loop

// calibrate ESC by sending 1.5ms signal for zero throttle at power on, then forward throttle, then backward.
void calibrate_esc(int signalPin) {
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
} // end calibrate_esc