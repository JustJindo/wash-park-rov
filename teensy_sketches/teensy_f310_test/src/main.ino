#include "USBHost_t36.h"

// 'USBHost' is the base class for USB objects
USBHost myusb;
USBHub hub1(myusb);
USBHIDParser hid1(myusb);

// 'JoystickController' is a class for controllers connected to your USB host
// make an index of 'JoystickController' objects, quantity defined by 'COUNT_JOYSTICKS' (you only need one for your Logitech F310)
#define COUNT_JOYSTICKS 4
JoystickController joysticks[COUNT_JOYSTICKS] = {
  JoystickController(myusb), JoystickController(myusb),
  JoystickController(myusb), JoystickController(myusb)
};

// NO IDEA WHAT THESE ARE FOR, DO NOT DELETE UNLESS UNDERSTOOD
int user_axis[64];
int psAxis[64];

USBDriver *drivers[] = {&hub1, &joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3], &hid1};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))
const char * driver_names[CNT_DEVICES] = {"Hub1", "joystick[0D]", "joystick[1D]", "joystick[2D]", "joystick[3D]",  "HID1"};
bool driver_active[CNT_DEVICES] = {false, false, false, false};

// Lets also look at HID Input devices
USBHIDInput *hiddrivers[] = {&joysticks[0], &joysticks[1], &joysticks[2], &joysticks[3]};
#define CNT_HIDDEVICES (sizeof(hiddrivers)/sizeof(hiddrivers[0]))
const char * hid_driver_names[CNT_DEVICES] = {"joystick[0H]", "joystick[1H]", "joystick[2H]", "joystick[3H]"};
bool hid_driver_active[CNT_DEVICES] = {false};

bool show_changed_only = false; // boolean to limit displayed fields from 'JoystickController' objects based on changed values

uint8_t joystick_left_trigger_value[COUNT_JOYSTICKS] = {0};
uint8_t joystick_right_trigger_value[COUNT_JOYSTICKS] = {0};
uint64_t joystick_full_notify_mask = (uint64_t) - 1;

//=============================================================================
// Setup
//=============================================================================
void setup()
{
  Serial1.begin(2000000);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("\n\nUSB Host Joystick Testing");
  myusb.begin();
}

//=============================================================================
// loop
//=============================================================================
void loop()
{
  myusb.Task();
  PrintDeviceListChanges();

  if (Serial.available()) {
    int ch = Serial.read(); // get the first char.
    while (Serial.read() != -1) ;
    if ((ch == 'b') || (ch == 'B')) {
      Serial.println("Only notify on Basic Axis changes");
      for (int joystick_index = 0; joystick_index < COUNT_JOYSTICKS; joystick_index++) // iterate through each controller
        joysticks[joystick_index].axisChangeNotifyMask(0x3ff); // axisChangeNotifyMask() returns a bit mask showing which axes have changed since last read
    } else if ((ch == 'f') || (ch == 'F')) {
      Serial.println("Only notify on Full Axis changes");
      for (int joystick_index = 0; joystick_index < COUNT_JOYSTICKS; joystick_index++)
        joysticks[joystick_index].axisChangeNotifyMask(joystick_full_notify_mask);

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

  for (int joystick_index = 0; joystick_index < COUNT_JOYSTICKS; joystick_index++) {
    if (joysticks[joystick_index].available()) {
      uint64_t axis_mask = joysticks[joystick_index].axisMask(); // return bit mask showing which axes are defined for current joystick
      uint64_t axis_changed_mask = joysticks[joystick_index].axisChangedMask(); // returns bit mask showing which axes have changed since last read
      uint32_t buttons = joysticks[joystick_index].getButtons(); // returns all buttons associated with 'JoystickController' object
      Serial.printf("Joystick(%d): buttons = %x", joystick_index, buttons); // print all buttons associated with 'JoystickController' object
      
      //Serial.printf(" AMasks: %x %x:%x", axis_mask, (uint32_t)(user_axis_mask >> 32), (uint32_t)(user_axis_mask & 0xffffffff));
      //Serial.printf(" M: %lx %lx", axis_mask, joysticks[joystick_index].axisChangedMask());
      
      if (show_changed_only) {
        for (uint8_t i = 0; axis_changed_mask != 0; i++, axis_changed_mask >>= 1) {
          if (axis_changed_mask & 1) {
            Serial.printf(" %d:%d", i, joysticks[joystick_index].getAxis(i));
          }
        }

      } else {
        for (uint8_t i = 0; axis_mask != 0; i++, axis_mask >>= 1) {
          if (axis_mask & 1) {
            Serial.printf(" %d:%d", i, joysticks[joystick_index].getAxis(i));
          }
        }
      }

      // get each axis value from the current controller and write them to the 'psAxis' index
      for (uint8_t i = 0; i<64; i++) {
          psAxis[i] = joysticks[joystick_index].getAxis(i);
      }

      Serial.println();
      joysticks[joystick_index].joystickDataClear();
    }
  }
}

//=============================================================================
// Show when devices are added or removed
//=============================================================================
void PrintDeviceListChanges() {
  for (uint8_t i = 0; i < CNT_DEVICES; i++) { // iterate through each device
    if (*drivers[i] != driver_active[i]) {
      if (driver_active[i]) {
        Serial.printf("*** Device %s - disconnected ***\n", driver_names[i]);
        driver_active[i] = false;
      } else {
        Serial.printf("*** Device %s %x:%x - connected ***\n", driver_names[i], drivers[i]->idVendor(), drivers[i]->idProduct());
        driver_active[i] = true;

        // print device attributes from 'USBDriver' object
        const uint8_t *psz = drivers[i]->manufacturer(); // local variable 'psz' stores a device attribute, initially set to manufacturer
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = drivers[i]->product(); // set 'psz' to product
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = drivers[i]->serialNumber(); // set 'psz' to serial number
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
      }
    }
  }

  for (uint8_t i = 0; i < CNT_HIDDEVICES; i++) {
    if (*hiddrivers[i] != hid_driver_active[i]) {
      if (hid_driver_active[i]) {
        Serial.printf("*** HID Device %s - disconnected ***\n", hid_driver_names[i]);
        hid_driver_active[i] = false;
      } else {
        Serial.printf("*** HID Device %s %x:%x - connected ***\n", hid_driver_names[i], hiddrivers[i]->idVendor(), hiddrivers[i]->idProduct());
        hid_driver_active[i] = true;

        const uint8_t *psz = hiddrivers[i]->manufacturer();
        if (psz && *psz) Serial.printf("  manufacturer: %s\n", psz);
        psz = hiddrivers[i]->product();
        if (psz && *psz) Serial.printf("  product: %s\n", psz);
        psz = hiddrivers[i]->serialNumber();
        if (psz && *psz) Serial.printf("  Serial: %s\n", psz);
      }
    }
  }
}