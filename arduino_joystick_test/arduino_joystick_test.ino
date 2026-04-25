/* Motor Control w/ Joystick
 *
 * This program uses a josytick to control a motor connected to
 * an L293D driver.
 * Horizontal thrusters (FORWARD, BACK, RIGHT, LEFT) will be controlled by a joystick.
 */

#include <stdio.h>

// MOTOR NEEDS SEPARATE VOLTAGE SUPPLY

// pins A0, A1 are connected to joystick's analog outputs
#define xPin  A0
#define yPin  A1
// pin 7 is connected to joystick's button
#define buttonPin 7
// motor pins are connected to L293D driver
// pin 3 is connected to 1,2EN; it controls PWM
// pins 12, 13 are connected to 1A, 2A; they control motor direction
#define motorEna  3
#define motorFwd  12
#define motorBck  13

void setup() {
  // set button pin to be active low
  pinMode(buttonPin, INPUT_PULLUP);
  // set motor pins as outputs
  pinMode(motorEna, OUTPUT);
  pinMode(motorFwd, OUTPUT);
  pinMode(motorBck, OUTPUT);
  // begin serial communication at 9600 bits per sec
  Serial.begin(9600);
} // end setup

void loop() {
  // read joystick values from pins
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int buttonValue = digitalRead(buttonPin);
  int motorPWM;

  // if joystick is pushed right
  if(xValue > 700) {
    motorPWM = 255;
    digitalWrite(motorFwd, HIGH);
    digitalWrite(motorBck, LOW);
  } // else if joystick is pushed left
  else if(xValue < 300) {
    motorPWM = 255;
    digitalWrite(motorFwd, LOW);
    digitalWrite(motorBck, HIGH);
  } // else don't power motor
  else motorPWM = 0;

  // write PWM to motor
  analogWrite(motorEna, motorPWM);

  Serial.print(xValue);
  Serial.print("\t");
  Serial.print(yValue);
  Serial.print("\t");
  Serial.print(buttonValue);
  Serial.print("\t");
  Serial.print(motorPWM);
  Serial.println("");

  // 1 ms delay for stability
  delay(1);
} // end loop