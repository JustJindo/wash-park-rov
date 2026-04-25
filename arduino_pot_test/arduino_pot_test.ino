/* Motor Control w/ Potentiometer
 *
 * This program uses a single-turn potentiometer to control a motor connected to
 * an L293D driver.
 * Vertical thrusters (UP, DOWN) will be controlled by a potentiometer.
 */

#include <stdio.h>

// MOTOR NEEDS SEPARATE VOLTAGE SUPPLY

// pin A2 is connected to the potentiometer output
#define potentPin A2
// motor pins are connected to L293D driver
// pin 3 is connected to 1,2EN; it controls PWM
// pins 12, 13 are connected to 1A, 2A; they control motor direction
#define motorEna  3
#define motorFwd  12
#define motorBck  13

void setup() {
  // set motor pins as outputs
  pinMode(motorEna, OUTPUT);
  pinMode(motorFwd, OUTPUT);
  pinMode(motorBck, OUTPUT);
  // begin serial communication at 9600 bits per sec
  Serial.begin(9600);
} // end setup

void loop() {
  // record value from potentiometer (0-1024)
  int potentValue = analogRead(potentPin);
  int motorPWM;

  // if potentiometer is turned to the right
  if(potentValue > 700){
    // power motor to propel upwards
    motorPWM = 255;
    digitalWrite(motorFwd, HIGH);
    digitalWrite(motorBck, LOW);
    Serial.println("UP");
  } // else if potentiometer is turned to the left
  else if(potentValue < 300){
    // power motor to propel downwards
    motorPWM = 255;
    digitalWrite(motorFwd, LOW);
    digitalWrite(motorBck, HIGH);
    Serial.println("DOWN");
  } // else if potentiometer is in the middle
  else{
    // don't power motor
    motorPWM = 0;
    Serial.println("STAY");
  } // end else

  // write PWM to motor
  analogWrite(motorEna, motorPWM);

  // 1 ms delay for stability
  delay(1);
} // end loop