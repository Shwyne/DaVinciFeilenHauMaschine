#include "Error_Handling.hpp"

void printError(ErrCode erCode) {
  switch (erCode) {
  case ErrCode::NO_ERROR:
    Serial.println("No Error.");
    break;
  case ErrCode::HW_TIMEOUT:
    Serial.println("TIMEOUT - Hammerwheel: Occurs if the Hall cant detect the "
                   "magnets for engaging the hammerstop. Hardware to check: "
                   "Hall-Sensor, DC-Motor, DC-Driver, Arduino, Connections");
    break;
  case ErrCode::SG_TIMEOUT:
    Serial.println(
        "TIMEOUT - Sign: Occurs if the Hall cant detect the magnets for "
        "nulling the sign-stepper. Hardware to check: Hall-Sensor, "
        "Stepper-Driver, Stepper, Arduino, Connections");
    break;
  case ErrCode::WG_TIMEOUT:
    Serial.println("TIMEOUT - Weight: Occurs if one of the Endstops should've "
                   "been triggered but is not. Hardware to check: Endstops, "
                   "DC-Motor, DC-Driver, Arduino, Connections");
    break;
  case ErrCode::SL_TIMEOUT:
    Serial.println("TIMEOUT - Slider: Occurs if one of the Endstops should've "
                   "been triggered but is not. Hardware to check: Endstops, "
                   "DC-Motor, DC-Driver, Arduino, Connections");
    break;
  case ErrCode::STPDRV_NOT_CONNECTED:
    Serial.println(
        "NOT CONNECTED - Stepper-Driver: The DRV8825 is pulling the ?-Pin to "
        "high. If the driver isnt present the arduino reads low. Hardware to "
        "check: Stepper-Driver, Arduino, Connections");
    break;
  case ErrCode::WG_OVERCURRENT:
    Serial.println("OVERCURRENT - Weight-Motor: Occurs if the current of the "
                   "DC-Motor is too high. (Only implemented, if a VISEN-Pin is "
                   "given): Hardware to check: DC-Motor, DC-Driver");
    break;
  case ErrCode::SL_OVERCURRENT:
    Serial.println("OVERCURRENT - Slider-Motor: Occurs if the current of the "
                   "DC-Motor is too high. (Only implemented, if a VISEN-Pin is "
                   "given): Hardware to check: DC-Motor, DC-Driver");
    break;
  case ErrCode::COUP_NOT_ATTACHED:
    Serial.println(
        "NOT ATTACHED - Servo for coupling : Occurs if the servo is not "
        "attached. Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::COUP_NOT_IN_POS:
    Serial.println("NOT IN POS - Servo for coupling: not in expected position "
                   "- Occurs if the servo is not in the position it should be. "
                   "Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::HS_NOT_ATTACHED:
    Serial.println(
        "NOT ATTACHED - Servo for hammerstop: Occurs if the servo is not "
        "attached. Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::HS_NOT_IN_POS:
    Serial.println("NOT IN POS - Servo for hammerstop: not in expected "
                   "position - Occurs if the servo is not in the position it "
                   "should be. Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::UNDEFINED:
    Serial.println("UNDEFINED ERROR. Check manual for more help.");
    break;
  }
}