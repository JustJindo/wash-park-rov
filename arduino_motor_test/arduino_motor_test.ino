#include <stdio.h>
// include the servo library compatible with Teensy's hardware
#include <PWMServo.h>

#define xPin A0
#define yPin A1
#define buttonPin 7
#define signalPin 3

// declare servo object
PWMServo servo;

void setup() {
  // attach signal pin to servo object with control pulse width ranging from 1000-2000 microseconds
  servo.attach(signalPin, 1000, 2000);
  // set button pin to be active low
  pinMode(buttonPin, INPUT_PULLUP);
  // begin serial communication at 9600 bits per sec
  Serial.begin(9600);
  Serial.println("Starting test...");

  // 7 sec delay
  delay(7000);
} // end setup

void loop() {
  //read joystick values from pins
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int buttonValue = digitalRead(buttonPin);

  // map the y-axis potentiometer ranging from 0-1023 to a valid PWM signal range from 1100-1900 microseconds
  int pwmVal = map(yValue, 0, 1023, 1100, 1900);
  // write PWM signal value to PWMServo object
  servo.write(pwmVal);

  Serial.print(xValue);
  Serial.print("\t");
  Serial.print(yValue);
  Serial.print("\t");
  Serial.print(buttonValue);
  Serial.println("");

  // 10 ms delay for stability
  delay(10);
} // end loop
