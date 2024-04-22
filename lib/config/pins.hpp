#pragma once
#include <Arduino.h>

namespace pin {

//? Change if pins are connected differently

//*DC-Motor Hammerwheel (IN1, IN2, SLP, VIS - MP6550 driver):
//IN1, IN2 = Motor Control, SLP = Sleep Mode, VIS = Voltage Indicator
constexpr uint8_t HW_IN1 = 12;  
constexpr uint8_t HW_IN2 = 13;  
constexpr uint8_t HW_SLP = 11;  
constexpr uint8_t HW_VIS = A0;  

//*DC-Motor Schlitten (IN1, IN2, SLP, VIS - MP6550 driver):
//IN1, IN2 = Motor Control, SLP = Sleep Mode, VIS = Voltage Indicator
constexpr uint8_t SL_IN1 = 4;   
constexpr uint8_t SL_IN2 = 3;  
constexpr uint8_t SL_SLP = 2;  
constexpr uint8_t SL_VIS = A1;  

//*Servo-Motoren (PWM):
//CO = Coupling, HS = Hammerstop, 0-255 PWM = 0-180°
constexpr uint8_t CO_SRV = 10;
constexpr uint8_t HS_SRV = 8;

//*Stepper (DIR, STP, SLP, RST, FLT, EN, M0, M1, M2 - DRV8825 driver):
//DIR = Direction Control, STP = Step Control, SLP = Sleep Mode, RST = Reset, FLT = Error Output
constexpr uint8_t STP_DIR = 18;
constexpr uint8_t STP_STP = 17;
constexpr uint8_t STP_SLP = 16;
constexpr uint8_t STP_RST = 15;
constexpr uint8_t STP_FLT = 14; 
//EN = Enable, M0, M1, M2 = Microstep Resolution
constexpr uint8_t STP_EN = A12;
constexpr uint8_t STP_M0 = A11; 
constexpr uint8_t STP_M1 = A10;
constexpr uint8_t STP_M2 = A9;

//*Endstops (digital):
//Endstops WG = Weight, SL = Slider
//Normally open, pull-up resistor -> HIGH = no signal, LOW = signal
constexpr uint8_t WG_ES_T = 43;
constexpr uint8_t WG_ES_B = 45;
constexpr uint8_t SL_ES_L = 41;  
constexpr uint8_t SL_ES_R = 39;

//*Hall-Sensors (digital):
//Hall-Sensors HR = Hammerwheel, SG = Sign
//Normally open, pull-up resistor -> HIGH = no signal, LOW = signal
constexpr uint8_t HR_HALL = 27; 
constexpr uint8_t SG_HALL = 25;

//*RGB-Button (digital):
//Go-Button with RGB-LEDs
//Normally open, pull-up resistor -> HIGH = no signal, LOW = signal
constexpr uint8_t GO_BUT = 33; // Taster "Go"
//LEDS: LOW = Off, HIGH = On, R = Red, G = Green, B = Blue
//LEDS (optional): Connect to PWM -> analogWrite(pin, pwm_value) -> 8-bit Colors
constexpr uint8_t GO_LED_R = 49; 
constexpr uint8_t GO_LED_G = 51;    
constexpr uint8_t GO_LED_B =53;  


//*Others:
// Button "Clear Error" -> press and hold to clear the error
constexpr uint8_t CLEAR_ERROR = 29;
// Fan -> cooling the electronics whenever the machine runs
// LOW = Off, HIGH = On
// optional: Add temperature sensor to control the fan 
constexpr uint8_t FAN = 31; 

} // namespace pin