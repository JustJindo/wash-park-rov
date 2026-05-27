#include <stdio.h>
// include Teensy servo library
#include <PWMServo.h>

// A14 = 38, A15 = 39, A16 = 40, A17 = 41
#define leftXPin 40
#define leftYPin 41
#define leftButtonPin 36
#define rightXPin 38
#define rightYPin 39
#define rightButtonPin 37

/*M1 = Port Side Horizontal
  M2 = Port Side Vertical
  M3 = Starboard Side Horizontal
  M4 = Starboard Side Vertical*/
#define M1_signalPin 2
#define M2_signalPin 3
#define M3_signalPin 4
#define M4_signalPin 5

/*APISQUEEN 12-24V 45A Bi-directional ESC
  >500Hz Refresh
  1-2ms PWM Signal
  1.5-2ms Forward
  1-1.5ms Backward
  1.5ms Zero Throttle*/

void setup() {
  // set PWM frequency to 500 Hz
  analogWriteFrequency(M1_signalPin, 500);
  analogWriteFrequency(M2_signalPin, 500);
  analogWriteFrequency(M3_signalPin, 500);
  analogWriteFrequency(M4_signalPin, 500);
  // set pulse width resolution to 8 bits
  analogWriteResolution(8);
  // begin serial communication at 9600 bits/sec
  Serial.begin(9600);
  // 3 second delay written into calibration for stability
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
  int leftXValue = analogRead(leftXPin);
  int leftYValue = analogRead(leftYPin);
  int leftButtonValue = digitalRead(leftButtonPin);
  int rightXValue = analogRead(rightXPin);
  int rightYValue = analogRead(rightYPin);
  int rightButtonValue = digitalRead(rightButtonPin);

  // map potentiometer axes ranging from 0-1023 to a valid pulse width range from 128-255
  int leftXPWM = map(leftXValue, 0, 1023, 128, 255);
  int leftYPWM = map(leftYValue, 0, 1023, 128, 255);
  int rightXPWM = map(rightXValue, 0, 1023, 128, 255);
  int rightYPWM = map(rightYValue, 0, 1023, 128, 255);

  // if axis value passes threshold, write pulse width to respective signal pin(s)
  if(leftYValue > 800 || leftYValue < 224) {
    analogWrite(M1_signalPin, leftYPWM);
    analogWrite(M3_signalPin, leftYPWM - 2*(leftYPWM - 191));
  } // end if
  // else, set thruster(s) to neutral
  else {
    analogWrite(M1_signalPin, 191);
    analogWrite(M3_signalPin, 191);
  } // end else

  // left and right joystick controls are processed independently
  if(rightYValue > 800 || rightYValue < 224) {
    analogWrite(M2_signalPin, rightYPWM);
    analogWrite(M4_signalPin, rightYPWM - 2*(rightYPWM - 191));
  } // end if
  else{
    analogWrite(M2_signalPin, 191);
    analogWrite(M4_signalPin, 191);
  } // end else

  Serial.print(leftXValue);
  Serial.print("\t");
  Serial.print(leftYValue);
  Serial.print("\t");
  Serial.print(leftButtonValue);
  Serial.print("\t");
  Serial.print(rightXValue);
  Serial.print("\t");
  Serial.print(rightYValue);
  Serial.print("\t");
  Serial.print(rightButtonValue);
  Serial.println("");

  // 10 ms delay for stability
  delay(10);
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
} //end calibrate_esc