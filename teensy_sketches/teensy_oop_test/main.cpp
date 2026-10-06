#include <stdio.h>
#include <iostream>
#include <thread>

#include "ESC.h"

void setup()
{
    // BLOCK TO TEST MULTITHREADING FOR SIMULTANEOUS ESC CALIBRATION
    //thread t(task); // create new thread the executes task() function [task IS A PLACEHOLDER]
    //t.join(); // makes main thread wait until 't' finishes [t IS A PLACEHOLDER]

    Serial.begin(9600); // begin serial communication at 9600 baud rate
} // end setup

void loop()
{

} // end main