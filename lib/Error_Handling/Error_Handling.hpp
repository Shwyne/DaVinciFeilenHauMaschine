#pragma once

#include <Arduino.h> // Add include statement for uint8_t

enum class ErrCode : uint8_t {
    NO_ERROR,   // No Error
    HW_TIMEOUT, // Timeout Hammerwheel: Occurs if the Hall cant detect the magnets
                            // for engaging the hammerstop. Hardware to check: Hall-Sensor,
                            // DC-Motor, DC-Driver, Arduino, Connections
    SG_TIMEOUT, // Timeout Sign: Occurs if the Hall cant detect the magnets for
                            // nulling the sign-stepper. Hardware to check: Hall-Sensor,
                            // Stepper-Driver, Stepper, Arduino, Connections
    WG_TIMEOUT, // Timeout Weight: Occurs if one of the Endstops should've been
                            // triggered but is not. Hardware to check: Endstops, DC-Motor,
                            // DC-Driver, Arduino, Connections
    SL_TIMEOUT, // Timeout Slider: Occurs if one of the Endstops should've been
                            // triggered but is not. Hardware to check: Endstops, DC-Motor,
                            // DC-Driver, Arduino, Connections
    STPDRV_NOT_CONNECTED, // Stepper-Driver not connected: The DRV8825 is pulling
                                                // the ?-Pin to high. If the driver isnt present the
                                                // arduino reads low. Hardware to check: Stepper-Driver,
                                                // Arduino, Connections
    WG_OVERCURRENT, // Overcurrent Weight-Motor: Occurs if the current of the
                                    // DC-Motor is too high. (Only implemented, if a VISEN-Pin is
                                    // given): Hardware to check: DC-Motor, DC-Driver
    SL_OVERCURRENT, // Overcurrent Slider-Motor: Occurs if the current of the
                                    // DC-Motor is too high. (Only implemented, if a VISEN-Pin is
                                    // given): Hardware to check: DC-Motor, DC-Driver
    COUP_NOT_ATTACHED, // Servo for coupling not attached: Occurs if the servo is
                                         // not attached. Hardware to check: Servo, Arduino,
                                         // Connections
    COUP_NOT_IN_POS, // Servo for coupling not in expected position: Occurs if the
                                     // servo is not in the position it should be. Hardware to
                                     // check: Servo, Arduino, Connections
    HS_NOT_ATTACHED, // Servo for hammerstop not attached: Occurs if the servo is
                                     // not attached. Hardware to check: Servo, Arduino,
                                     // Connections
    HS_NOT_IN_POS, // Servo for hammerstop not in expected position: Occurs if the
                                 // servo is not in the position it should be. Hardware to
                                 // check: Servo, Arduino, Connections
    UNDEFINED // Undefined Error: Occurs if an error is thrown but not defined in
                        // the enum class.
};

void printError(ErrCode erCode);