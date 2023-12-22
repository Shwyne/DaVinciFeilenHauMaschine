#pragma once

#include "pins.hpp"
#include <Arduino.h>

//*Serial-Connection:
#define SERIAL_BPS 9600 // Serial Baudrate

//*Pause-Settings:
const int INTERVAL = 30; // Interval time for running in seconds

const int INTER_MS = INTERVAL * 1000; // Transformation to milliseconds

const short DELAY = 1000;    // Default delay for testing purposes
const short DELAY_2 = 500;   // Default delay for testing purposes
const uint8_t DELAY_4 = 250; // Default delay for testing purposes

//*Endstop-Settings:
const uint8_t ES_TRIGGER =
    0; // Endstop Mode (0 = Pullup, 1 = Pulldown) - default: 0

namespace Weight { // Endstop Weight
enum esState {
  UNTRIGGERED = 0, // No Endstops reached
  TOP = 2,         // Top Endstop triggered
  BOTTOM = 1,      // Bottom Endstop triggered
};
}

namespace Slider { // Endstop Slider
enum esState {
  UNTRIGGERED = 0, // No Endstops reached
  LEFT = 1,        // Left Endstop triggered
  RIGHT = 2,       // Right Endstop triggered
};
}

//*Hall-Settings:
const bool HALL_TRIGGER = 0; // Hall-Sensor Trigger State (0 = LOW, 1 = HIGH)
const uint16_t HALL_TIMEOUT = 4000; // Hall-Sensor Timeout in ms
//*Go-Button-Settings:

const uint8_t GO_TRIGGER =
    0; // Go-Button Trigger (0 = Pullup, 1 = Pulldown) - default: 0

//*DC-Motor-Settings:

const uint8_t DC_MAXSPEED =
    255; // DC-Motor Max Speed for both motors (PWM-Value)

namespace HR {            //*DC-Motor Hammerwheel
const bool reversed = 1;  // DC-Motor Direction (0 = Forward, 1 = Backward)
const int speed = 155;    // DC-Motor Speed while running (PWM-Value)
const int rs_speed = 255; // DC-Motor Speed while reseting (PWM-Value)

const short SPEED =
    (reversed) ? -speed : speed; // Changing direction according to reversed
const short RS_SPEED =
    (reversed) ? rs_speed
               : -rs_speed; // Changing direction according to reversed
} // namespace HR
namespace SL {            //*DC-Motor Slider
const bool reversed = 1;  // DC-Motor Direction (0 = Forward, 1 = Backward)
const int speed = 200;    // DC-Motor Speed while running
const int rs_speed = 255; // DC-Motor Speed while reseting

const short SPEED =
    (reversed) ? -speed : speed; // Changing direction according to reversed
const short RS_SPEED =
    (reversed) ? rs_speed
               : -rs_speed; // Changing direction according to reversed
} // namespace SL

//*Stepper-Settings:
const uint8_t STP_INTERFACE = 1; // Stepper Interface (1 = Driver)
const uint8_t STP_MODE = 16; // Stepper Mode (1 = Full Step, 2 = Half Step, 4 =
                             // Quarter Step, 8 = Eighth Step, ...)
const short STP_STEPS = 200; // Stepper Steps per Revolution
const float STP_i = 1.2;     // Stepper to Schild Gear Ratio
const short STP_POS =
    short(STP_STEPS / 3 * STP_i) * STP_MODE; // Steps per Position change
const short STP_RPM = 120; // Stepper Speed in Revolutions/Minute

//*Servo-Settings:
const uint8_t SRV_MIN = 0;   // Servo min Angle
const uint8_t SRV_MAX = 180; // Servo max Angle

//* Servo-Positions:

namespace HS {
const uint8_t OFF = 0;              // Hammerstop off -> not engaged
const uint8_t ON = 80;              // Hammerstop on -> engaged and blocking
const uint8_t RANGE = 5;            // Range for position check
const uint8_t TriesBeforeError = 5; // Trys before error
} // namespace HS
namespace COUP {

const uint8_t DIS = 0;              // Coupling disengaged -> No "transmission"
const uint8_t EN = 90;              // Coupling engaged -> "transmission"
const uint8_t RANGE = 3;            // Range for position check
const uint8_t TriesBeforeError = 5; // Trys before error
} // namespace COUP
