#include <stdio.h>
#include <Servo.h>

#define left_XPin A0
#define left_YPin A1
#define left_ButtonPin 13
#define right_XPin A2
#define right_YPin A3
#define right_ButtonPin 12

/*
Looking from stern towards bow...
motor 1 is top,
motor 2 is bottom,
motor 3 is left,
and motor 4 is right.
Signal pins are defined accordingly.
*/
#define signal1Pin 8
#define signal2Pin 7
#define signal3Pin 4
#define signal4Pin 2

// declare servo objects for each motor's esc
Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

void setup() {
  // attach PWM pins to corresponding objects
  esc1.attach(signal1Pin);
  esc2.attach(signal2Pin);
  esc3.attach(signal3Pin);
  esc4.attach(signal4Pin);
  // initialize speed controllers at 1500ms
  esc1.writeMicroseconds(1500);
  esc2.writeMicroseconds(1500);
  esc3.writeMicroseconds(1500);
  esc4.writeMicroseconds(1500);
  // set button pins to be active low
  pinMode(left_ButtonPin, INPUT_PULLUP);
  pinMode(right_ButtonPin, INPUT_PULLUP);
  // begin serial communication at 9600 bits per sec
  Serial.begin(9600);
  Serial.println("Starting thrusters...");

  // 7 sec delay
  delay(7000);
} // end setup

void loop() {
  //read joystick values from pins
  int left_XValue = analogRead(left_XPin);
  int left_YValue = analogRead(left_YPin);
  int left_ButtonValue = digitalRead(left_ButtonPin);
  int right_XValue = analogRead(right_XPin);
  int right_YValue = analogRead(right_YPin);
  int right_ButtonValue = digitalRead(right_ButtonPin);

  // if joystick values correspond to left, right, up, or down, then map the relevant potentiometer(s)
  // map the potentiometer axes ranging from 0-1023 to a valid pwm range for servo objects from 1100-1900
  int left_Xpwm = map(left_XValue, 0, 1023, 1100, 1900);
  int left_Ypwm = map(left_YValue, 0, 1023, 1100, 1900);
  int right_Ypwm = map(right_YValue, 0, 1023, 1100, 1900);

  /*
  LEFT JOYSTICK
  forward: horizontals forward
  backward: horizontals backward
  right: left forward and right backward
  left: left backward and right forward
  RIGHT JOYSTICK
  forward: verticals forward
  backward: verticals backward
  */
  esc1.writeMicroseconds(pwmVal);
  // if left joystick is pushed right
  if(left_XValue > 600){

  }

  Serial.print(left_XValue);
  Serial.print("\t");
  Serial.print(left_YValue);
  Serial.print("\t");
  Serial.print(left_ButtonValue);
  Serial.print("\t");
  Serial.print(right_XValue);
  Serial.print("\t");
  Serial.print(right_YValue);
  Serial.print("\t");
  Serial.print(right_ButtonValue);
  Serial.println("");

  // 10 ms delay for stability
  delay(10);
} // end loop
