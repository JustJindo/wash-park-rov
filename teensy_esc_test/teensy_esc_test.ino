#include <stdio.h>
// include Teensy servo library
#include <PWMServo.h>

// A16 = 40, A17 = 41
#define xPin A16
#define yPin A17
#define buttonPin 36
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
  // int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  // int buttonValue = digitalRead(buttonPin);

  // map the y-axis potentiometer ranging from 0-1023 to a valid pulse width range from 128-255
  int pwmVal = map(yValue, 0, 1023, 128, 255);
  // write pulse width to signal pin
  analogWrite(signalPin, pwmVal);

  //Serial.print(xValue);
  //Serial.print("\t");
  Serial.print(yValue);
  //Serial.print("\t");
  //Serial.print(buttonValue);
  Serial.println("");

  // 10 ms delay for stability
  delay(10);
} // end loop

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