#include "USBHost_t36.h"

#include <stdio.h>
#include <PWMServo.h>

/*M1 = Port Side Horizontal
  M2 = Port Side Vertical
  M3 = Starboard Side Horizontal
  M4 = Starboard Side Vertical*/

#define M1_signalPin 2
#define M2_signalPin 3
#define M3_signalPin 4
#define M4_signalPin 5

// 'USBHost' is the base class for USB objects
USBHost myusb;
USBHub hub1(myusb);
USBHIDParser hid1(myusb);

// 'JoystickController' is a class for controllers connected to your USB host
// make an index of 'JoystickController' objects, quantity defined by 'COUNT_JOYSTICKS' (you only need one for your Logitech F310)
#define COUNT_JOYSTICKS 1
JoystickController joysticks[COUNT_JOYSTICKS] = {
  JoystickController(myusb)
};

int user_axis[64];
int psAxis[64];

USBDriver *drivers[] = {&hub1, &joysticks[0], &hid1};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
const char * driver_names[CNT_DEVICES] = {"Hub1", "joystick[0D]", "HID1"};
bool driver_active[CNT_DEVICES] = {false};

// Lets also look at HID Input devices
USBHIDInput *hiddrivers[] = {&joysticks[0]};
#define CNT_HIDDEVICES (sizeof(hiddrivers)/sizeof(hiddrivers[0]))
const char * hid_driver_names[CNT_DEVICES] = {"joystick[0H]"};
bool hid_driver_active[CNT_DEVICES] = {false};

bool show_changed_only = false; // boolean to limit displayed fields from 'JoystickController' objects based on changed values

uint8_t joystick_left_trigger_value[COUNT_JOYSTICKS] = {0};
uint8_t joystick_right_trigger_value[COUNT_JOYSTICKS] = {0};
uint64_t joystick_full_notify_mask = (uint64_t) - 1;

void setup()
{
  
  /*APISQUEEN 12-24V 45A Bi-directional ESC
    >500Hz Refresh
    1-2ms PWM Signal
    1.5-2ms Forward
    1-1.5ms Backward
    1.5ms Zero Throttle*/

  // set PWM frequency to 500 Hz
  analogWriteFrequency(M1_signalPin, 500);
  analogWriteFrequency(M2_signalPin, 500);
  analogWriteFrequency(M3_signalPin, 500);
  analogWriteFrequency(M4_signalPin, 500);
  // set pulse width resolution to 8 bits
  analogWriteResolution(8);

  Serial1.begin(2000000);
  while (!Serial) ; // wait for Arduino Serial Monitor

  // // Calibrate ESCs with 3 second intervals for stability
  // CalibrateESC(M1_signalPin);
  // CalibrateESC(M2_signalPin);
  // CalibrateESC(M3_signalPin);
  // CalibrateESC(M4_signalPin);

  Serial.println("\n\nUSB Host Joystick Testing");
  myusb.begin();
} // end setup

void loop()
{
  myusb.Task();
  PrintDeviceListChanges();

  if (Serial.available()) {
    int ch = Serial.read(); // get the first char.
    while (Serial.read() != -1) ;
    if ((ch == 'b') || (ch == 'B')) {
      Serial.println("Only notify on Basic Axis changes");
      joysticks[0].axisChangeNotifyMask(0x3ff); // axisChangeNotifyMask() returns a bit mask showing which axes have changed since last read
    } else if ((ch == 'f') || (ch == 'F')) {
      Serial.println("Only notify on Full Axis changes");
      joysticks[0].axisChangeNotifyMask(joystick_full_notify_mask);
    } else {
      if (show_changed_only) {
        show_changed_only = false;
        Serial.println("\n*** Show All fields mode ***");
      } else {
        show_changed_only = true;
        Serial.println("\n*** Show only changed fields mode ***");
      }
    }
  }

  if (joysticks[0].available()) {
    uint64_t axis_mask = joysticks[0].axisMask(); // return bit mask showing which axes are defined for joystick
    uint64_t axis_changed_mask = joysticks[0].axisChangedMask(); // returns bit mask showing which axes have changed since last read
    uint32_t buttons = joysticks[0].getButtons(); // returns all buttons associated with 'JoystickController' object
    Serial.printf("Joystick(%d): buttons = %x", 0, buttons); // print all buttons associated with 'JoystickController' object
    
    //Serial.printf(" AMasks: %x %x:%x", axis_mask, (uint32_t)(user_axis_mask >> 32), (uint32_t)(user_axis_mask & 0xffffffff));
    //Serial.printf(" M: %lx %lx", axis_mask, joysticks[0].axisChangedMask());
    
    if (show_changed_only) {
      for (uint8_t i = 0; axis_changed_mask != 0; i++, axis_changed_mask >>= 1) {
        if (axis_changed_mask & 1) {
          Serial.printf(" %d:%d", i, joysticks[0].getAxis(i));
        }
      }
    } else {
      for (uint8_t i = 0; axis_mask != 0; i++, axis_mask >>= 1) {
        if (axis_mask & 1) {
          Serial.printf(" %d:%d", i, joysticks[0].getAxis(i));
          Serial.printf(" %d:%d", i, joysticks[0].getAxis(0)); // Testing output of first value in getAxis();
        }
      }
    }

    // get each axis value from the controller and write to the 'psAxis' index
    for (uint8_t i = 0; i<64; i++) {
        psAxis[i] = joysticks[0].getAxis(i);
    }

    Serial.println();
    joysticks[0].joystickDataClear();
  }
} // end loop

//=============================================================================
// Calibrate ESC by sending zero, forward, then backward throttle
//=============================================================================
void CalibrateESC(int signalPin) {
  Serial.println("Calibrating ESCs...");

  // begin at zero throttle for three seconds
  analogWrite(signalPin, 192);
  delay(3000);
  // send forward throttle for one second
  Serial.println("Forwards...");
  analogWrite(signalPin, 255);
  delay(1000);
  // return to zero throttle for one second
  Serial.println("Pause...");
  analogWrite(signalPin, 192);
  delay(1000);
  // send backward throttle for one second
  Serial.println("Backwards...");
  analogWrite(signalPin, 128);
  delay(1000);
  // return to zero throttle
  analogWrite(signalPin, 192);

  Serial.println("Calibration complete.");
  delay(3000);
} //end CalibrateESC

//=============================================================================
// Show when devices are added or removed
//=============================================================================
void PrintDeviceListChanges() {

  if (*drivers[0] != driver_active[0]) {
    if (driver_active[0]) {
      Serial.printf("*** Device %s - disconnected ***\n", driver_names[0]);
      driver_active[0] = false;
    } else {
      Serial.printf("*** Device %s %x:%x - connected ***\n", driver_names[0], drivers[0]->idVendor(), drivers[0]->idProduct());
      driver_active[0] = true;

      // print device attributes from 'USBDriver' object
      const uint8_t *psz = drivers[0]->manufacturer(); // local variable 'psz' stores a device attribute, initially set to manufacturer
      if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
      psz = drivers[0]->product(); // set 'psz' to product
      if (psz && *psz) Serial.printf("  product: %s\n", psz);
      psz = drivers[0]->serialNumber(); // set 'psz' to serial number
      if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
    }
  }

  if (*hiddrivers[0] != hid_driver_active[0]) {
    if (hid_driver_active[0]) {
      Serial.printf("*** HID Device %s - disconnected ***\n", hid_driver_names[0]);
      hid_driver_active[0] = false;
    } else {
      Serial.printf("*** HID Device %s %x:%x - connected ***\n", hid_driver_names[0], hiddrivers[0]->idVendor(), hiddrivers[0]->idProduct());
      hid_driver_active[0] = true;

      const uint8_t *psz = hiddrivers[0]->manufacturer();
      if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
      psz = hiddrivers[0]->product();
      if (psz && *psz) Serial.printf("  product: %s\n", psz);
      psz = hiddrivers[0]->serialNumber();
      if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
    }
  }
} // end PrintDeviceListChanges