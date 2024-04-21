#pragma once
#include <Arduino.h>

namespace pin {

//? Change if pins are connected differently

//*DC-Motor Hammerwheel:
constexpr uint8_t HW_IN1 = 12;  // IN1 Pin MP6550 - Motor Control
constexpr uint8_t HW_IN2 = 13;  // IN2 Pin MP6550 - Motor Control
constexpr uint8_t HW_SLP = 11;  // SLP Pin MP6550 - Sleep Mode
constexpr uint8_t HW_VIS = A0;  // VIS Pin MP6550 - Voltage Indicator //TODO: Implement

//*DC-Motor Schlitten:
constexpr uint8_t SL_IN1 = 4;   // IN1 Pin MP6550 - Motor Control
constexpr uint8_t SL_IN2 = 3;   // IN2 Pin MP6550 - Motor Control
constexpr uint8_t SL_SLP = 2;   // SLP Pin MP6550 - Sleep Mode
constexpr uint8_t SL_VIS = A1;  // VIS Pin MP6550 - Voltage Indicator //TODO: Implement

//*Servo-Motoren:
constexpr uint8_t CO_SRV = 10;  // Coupling PWM-Control (0-255 = 0-180°)
constexpr uint8_t HS_SRV = 8;   // Hammerstop PWM-Control (0-255 = 0-180°)

//*Stepper:
constexpr uint8_t STP_DIR = 18; // Direction Pin DRV8825 - Direction Control
constexpr uint8_t STP_STP = 17; // Step Pin DRV8825 - Pulsing this (High and then low) -> steps the motor in the direction of the DIR-Pin
constexpr uint8_t STP_SLP = 16; // Sleep Pin DRV8825 - Sleep Mode
constexpr uint8_t STP_RST = 15; // Reset Pin DRV8825 - Reset the Driver
constexpr uint8_t STP_FLT = 14; // Fault Pin DRV8825 - Error Output

constexpr uint8_t STP_EN = A12; // Enable Pin DRV8825 - Enables the Driver
constexpr uint8_t STP_M0 = A11; // Microstep Resolution Pin DRV8825 - Resolution of the Microsteps (Datasheet)
constexpr uint8_t STP_M1 = A10; // Microstep Resolution Pin DRV8825 - Resolution of the Microsteps (Datasheet)
constexpr uint8_t STP_M2 = A9;  // Microstep Resolution Pin DRV8825 - Resolution of the Microsteps (Datasheet)

//*Sensorik:
constexpr uint8_t WG_ES_T = 43; // Endstop Weight Top
constexpr uint8_t WG_ES_B = 45; // Endstop Weight Bottom 
constexpr uint8_t SL_ES_L = 41; // Endstop Slider Left    
constexpr uint8_t SL_ES_R = 39; // Endstop Slider Right   

constexpr uint8_t HR_HALL = 27; // Hall-Sensor Hammerwheel
constexpr uint8_t SG_HALL = 25; // Hall-Sensor Sign        

constexpr uint8_t GO_BUT = 33; // Taster "Go"
constexpr uint8_t GO_LED_R = 49; // Red LED "Go"   
constexpr uint8_t GO_LED_G = 51; // Blue LED "Go"   
constexpr uint8_t GO_LED_B =53; // Green LED "Go"   


//*Sonstiges:
constexpr uint8_t CLEAR_ERROR = 29; // Button "Clear Error" -> press and hold to clear the error
constexpr uint8_t FAN = 31; // Fan -> cooling the electronics whenever the machine runs

} // namespace pin