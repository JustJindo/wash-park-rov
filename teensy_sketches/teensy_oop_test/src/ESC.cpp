// constructor file is for implementation of its associated class
#include "ESC.h"

ESC::ESC(const int position)
{
    this->position = position;
    this->PWM = 128; // initialize PWM to 128 (50% duty cycle)
}

void ESC::throttle(int PWM){
    analogWrite(position, PWM);
}

void ESC::getTelemetry(){
    Serial.println(position);
}