// header file is for definitions and method declarations
#include <stdio.h>
#include <PWMServo.h> // Teensy servo library

/*POTENTIOMETER AXES ORIENTATIONS IN CONTROLLER
  LEFT: Y->1024
  RIGHT: Y->0
  UP: X->0
  DOWN: X->1024*/
#define Port_Horz 2
#define Port_Vert 3
#define Star_Horz 4
#define Star_Vert 5

class ESC
{
    public:
    // you should know...
    // which motor the ESC is connected to
    // what the ESC's current throttle magnitude is
    // whether or not the ESC is on
    int position;

    void throttle(int PWM); // send a PWM throttle signal to the ESC
    void getTelemetry(); // printout telemetry from ESC
    void calibrate(); // calibrate ESCs

    private:

};