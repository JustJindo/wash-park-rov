/* Motor Control w/ Joystick
 *
 * This program uses two joysticks to control 3 DC motors driven by
 * an Adafruit Motor Shield v3.
 * Left joystick controls horizontal (FORWARD, BACK, RIGHT, LEFT) thrusters.
 * Right joystick controls vertical (UP, DOWN) thruster.
 */

#include <stdio.h>
#include <Adafruit_MotorShield.h>

// MOTOR NEEDS SEPARATE VOLTAGE SUPPLY

// create motor shield object w/ default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// set left motor to port M1, right motor to port M2
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *vertMotor = AFMS.getMotor(3);

// pins A0, A1 are connected to left joystick's analog outputs
#define leftXPin       A0
#define leftYPin       A1
// pin 7 is connected to left joystick's button
#define leftButtonPin  7
// pins A2, A3 are connected to right joystick's analog outputs
#define rightXPin      A2
#define rightYPin      A3
// pin 4 is connected to right joystick's button
#define rightButtonPin 4

void setup() {

  // set button pins to be active low
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  // begin serial communication at 9600 bits per sec
  Serial.begin(9600);
  Serial.println("Starting program...");

  // begin motor shield operation w/ default frequency (1.6kHz)
  if(!AFMS.begin()) {
    Serial.println("Could not find motor shield. Check wiring...");
    while(1);
  } // end if
  Serial.println("Motor shield found.");

} // end setup

void loop() {

  // read joystick values from pins
  int leftXValue = analogRead(leftXPin);
  int leftYValue = analogRead(leftYPin);
  int leftButtonValue = digitalRead(leftButtonPin);
  int rightXValue = analogRead(rightXPin);
  int rightYValue = analogRead(rightYPin);
  int rightButtonValue = digitalRead(rightButtonPin);
  // save max PWM to pass when running motors
  int motorPWM =  255;

  // NOTE: Horizontal and vertical thrusters should be controlled 
  //       independently if you want to run them simultaneously.

  // if right joystick is pushed forward
  if(rightYValue < 300) {
    vertMotor->run(BACKWARD);
    vertMotor->setSpeed(motorPWM);
  } // else if right joystick is pushed backward
  else if(rightYValue > 700) {
    vertMotor->run(FORWARD);
    vertMotor->setSpeed(motorPWM);
  } // else don't run vertical motor
  else {
    vertMotor->run(RELEASE);
  } // end else

  // if left joystick is pushed forward
  if(leftYValue < 300) {
    leftMotor->run(FORWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(motorPWM);
  } // else if left joystick pushed backward
  else if(leftYValue > 700) {
    leftMotor->run(BACKWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(motorPWM);
  } // else if left joystick is pushed right
  else if(leftXValue > 700) {
    leftMotor->run(FORWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(motorPWM);
  } // else if left joystick is pushed left
  else if(leftXValue < 300) {
    leftMotor->run(BACKWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(motorPWM);
  }  // else don't run horizontal motors
  else {
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
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
