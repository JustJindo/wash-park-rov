// constructor file is for implementation of its associated class
#include "ESC.h"

void ESC::throttle(int PWM){
    analogWrite(ESC.position, PWM);
}