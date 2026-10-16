// This header file declares an ESC class to control each ESC in translating PWM signals to motor commands.
#include <Arduino.h>
#include <stdio.h>

#define Port_Horz 2
#define Port_Vert 3
#define Star_Horz 4
#define Star_Vert 5

// An instance of ESC will be created for each ESC to handle signal interpretation and telemetry for each motor.
class ESC
{

public: // access specifier - determines whether members can be accessed/modified outside the code
        // members are private unless specified otherwise
        
    int position; // motor position (i.e. Port Horizontal)
    int PWM; // motor PWM value (i.e. 0-255)

    ESC(int position); // declare constructor with position's associated signal pin as argument

    void throttle(int PWM); // send a PWM throttle signal to the ESC
    void getTelemetry(); // printout telemetry from ESC

private: // private - members cannot be accessed/modified outside of class declaration/definition

};