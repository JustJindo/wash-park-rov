// This header file declares a Gamepad class to translate HID inputs from a Logitech F310 gamepad to ROV command signals.
#include <Arduino.h>
#include <stdio.h>

#define USB_associated_pins_and_such

// Gamepad will handle Logitech F310 gamepad inputs
class Gamepad
{

public:

    int USB_pins; // USB-associated pins will be used as arguments in constructor

    Gamepad(int USB_pins); // Only one instance of Gamepad is needed, it will be constructed with appropriate pins

private:

};