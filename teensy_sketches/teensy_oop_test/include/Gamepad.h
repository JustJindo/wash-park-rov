// This header file declares a Gamepad class to handle USB connection and signals from a Logitech F310 gamepad.
#include <Arduino.h>
#include <stdio.h>

// Most of the code is cannibalized from the 'USBHost_t36' library's 'Joystick' example.
#include "USBHost_t36.h"

// Gamepad will handle Logitech F310 gamepad inputs
class Gamepad
{

public:

    int USB_pins; // USB-associated pins will be used as arguments in constructor

    Gamepad(int USB_pins); // Only one instance of Gamepad is needed, it will be constructed with appropriate pins

private:

};