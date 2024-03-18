#pragma once
#include <Arduino.h>

namespace pin {

//*DC-Motor Hammerwheel:
constexpr uint8_t HW_IN1 = 12;
constexpr uint8_t HW_IN2 = 13;
constexpr uint8_t HW_SLP = 11;
constexpr uint8_t HW_VIS = A0;

//*DC-Motor Schlitten:
constexpr uint8_t SL_IN1 = 4;
constexpr uint8_t SL_IN2 = 3;
constexpr uint8_t SL_SLP = 2;
constexpr uint8_t SL_VIS = A1;

//*Servo-Motoren:
constexpr uint8_t CO_SRV = 10; // Kupplung
constexpr uint8_t HS_SRV = 8; // Hammerstop

//*Stepper:
constexpr uint8_t STP_DIR = 18;
constexpr uint8_t STP_STP = 17;
constexpr uint8_t STP_SLP = 16;
constexpr uint8_t STP_RST = 15;
constexpr uint8_t STP_FLT = 14;

constexpr uint8_t STP_EN = A12;
constexpr uint8_t STP_M0 = A11;
constexpr uint8_t STP_M1 = A10;
constexpr uint8_t STP_M2 = A9;

//*Sensorik:
constexpr uint8_t WG_ES_T = 43; // Endstop Weight Top 
constexpr uint8_t WG_ES_B = 45; // Endstop Weight unten 
constexpr uint8_t SL_ES_L = 41; // Endstop Slide left    
constexpr uint8_t SL_ES_R = 39; // Endstop Slide right   

constexpr uint8_t HR_HALL = 27; // Hall-Sensor Hammerwheel
constexpr uint8_t SG_HALL = 25; // Hall-Sensor Sign        

constexpr uint8_t GO_BUT = 33; // Taster "Go"
constexpr uint8_t GO_LED_R = 49; // LED "Go"   
constexpr uint8_t GO_LED_G = 51; // LED "Go"   
constexpr uint8_t GO_LED_B =53; // LED "Go"   


//*Sonstiges:
constexpr uint8_t CLEAR_ERROR = 29; // Taster "Clear Error"
constexpr uint8_t FAN = 31; // Lüfter

} // namespace pin