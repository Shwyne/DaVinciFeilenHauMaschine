#pragma once
#include <Arduino.h>
#include "pins.hpp"
#include "Enumerator.hpp"



//---------------------------------------------------------------------------------------------
//? Settings for the user to change:
//---------------------------------------------------------------------------------------------

//* General Settings:

//Fan -> cooling the electronics whenever the machine runs
constexpr bool FAN = 1;
//Temperature Measurement for controlling the Fan (not implemented yet)
constexpr bool TEMP_MEAS = 0; 


//---------------------------------------------------------------------------------------------
//*Serial-Settings:
namespace SERIALSET{

//Baudrate in Bits per Second (Bps)
//Check Device-Manager -> COM -> Arduino -> Settings -> Properties -> Port Settings
//Standard: 9600
//The Arduino might be called differently depending on the manufacturer
constexpr int BAUTRATE = 9600;  
}

//---------------------------------------------------------------------------------------------
//*Debugging and Error-Management:
//0 = off, 1 = basic info, 2 = detailed info
constexpr uint8_t DEBUG = 2;
//If Error-Management is enabled, an error will lead to the error state
//If not enabled, error will be ignored   
constexpr bool ERROR_MANAGEMENT = 1;
//If EEPROM is enabled, the error-flag will be stored in the EEPROM
//This allows for the error to be handled, even after the device loses power 
constexpr bool EEPROM_ENABLED = 1;
//Adress where the error-flag is stored (Atmega328P has 1024 Bytes of EEPROM)       
constexpr uint16_t EEPROM_ADDRESS = 0;
//Blinking Interval of the Error-LED in ms  
constexpr uint16_t ERROR_LED_DELAY = 500;
//Duration for the reset button to be pressed to clear the error in ms
constexpr uint16_t CLEAR_ERROR_TIME = 2000; 

//---------------------------------------------------------------------------------------------
//*Hammerwheel (DC-Motor):
namespace HW {
//Maximal RPM of the Motor (datasheet)
constexpr float RPM_MAX = 10;
//If the motor turns in the wrong direction, change this to 1
constexpr bool REVERSED = 1;
//This enables the Motor to automatically go to sleep after the motor has stopped
constexpr bool AUTO_SLEEP = 1;
//Speed of the Motor (0-255) -> 0 = 0%, 64 = 25%, 128 = 50%, 192 = 75%, 255 = 100%
constexpr int SPEED = 255;
//Reset-Speed of the Motor (0-255) -> 0 = 0%, 64 = 25%, 128 = 50%, 192 = 75%, 255 = 100%
constexpr int RS_SPEED = 255;
//The Gear Ratio inbetween the Motor and the Hammerwheel shaft
constexpr float i = 0.5;
//Number of Magnets on the Hammerwheel
constexpr uint8_t nMAGNETS = 6;
//Height of the weight lifted in mm
constexpr uint16_t HEIGHT_mm = 700;
//Radius of the Weightspool in mm
constexpr uint16_t RADIUS_mm = 40;
}

//---------------------------------------------------------------------------------------------
//*Slider (DC-Motor):
namespace SL {
//Maximal RPM of the Motor (datasheet)
constexpr float RPM_MAX = 19;
//Length of the Slider path in mm
constexpr uint16_t L_mm = 108;
//Pitch of the Screw in mm 
constexpr uint8_t PITCH = 5;
//If the motor turns in the wrong direction, change this to 1
constexpr bool REVERSED = 0;
//This enables the Motor to automatically go to sleep after the motor has stopped
constexpr bool AUTO_SLEEP = 1;
//Synchronizes the Slider with the Hammerwheel
constexpr bool SYNCHRONIZED = 1;
//Transmission Factor inbetween the Slider and the Hammerwheel
constexpr float SYNC_FAC = 0.5;
//Speed of the Motor (0-255) -> 0 = 0%, 64 = 25%, 128 = 50%, 192 = 75%, 255 = 100%
//Used only, if synchronized = 0
constexpr uint8_t speed_ = 180;     
//Reset-Speed of the Motor (0-255) -> 0 = 0%, 64 = 25%, 128 = 50%, 192 = 75%, 255 = 100%
constexpr int RS_SPEED = 255;
//The Gear Ratio inbetween the Motor and the Slider shaft
constexpr float i = 0.5;
}

//---------------------------------------------------------------------------------------------
//*Sign (Stepper):
namespace STP{
//The Stepper turns the sign and can be disabled if needed
constexpr bool ENABLED = 1;
//Microsteps of the Stepper 
//-> more microsteps -> less force, more precision, smoother
// Possible with this driver: 1,2,4,8,16,32
constexpr uint8_t MICRO_STEPS = 32;
// Default steps per revolution: 200 -> for most stepper-motors (Datasheet)   
constexpr uint8_t SPR = 200;            //Steps per Revolution
//Gear Ratio inbetween the Stepper and the Sign shaft
constexpr float i = 1.2;
//Speed of the Stepper in RPM
//Can be max RPM or lowered if slower turnings speeds are desired
constexpr uint8_t RPM = 10;
//Angle till the next surface of the sign in degrees (°)
constexpr uint16_t ANGLE = 120;
//Timeout
constexpr uint32_t TIMEOUT = 5000;
}

//---------------------------------------------------------------------------------------------
//*Hammerstop (Servo):
namespace HS {
//Defines the angle of the "Off" position in degrees (°)
constexpr uint8_t OFF = 0;
//Defines the angle of the "On" position in degrees (°)
constexpr uint8_t ON = 80;
//Minimum angle of the Servo in degrees (°) (Datasheet)
//Default: 0°
constexpr uint8_t MIN = 0;
//Maximum angle of the Servo in degrees (°) (Datasheet)
//Default: 180°
constexpr uint8_t MAX = 180;
//Time per degree in ms (Datasheet or estimations/measurements)
//Used for error-management and timeout-calculations
constexpr float TPD = 0.14;

}

//---------------------------------------------------------------------------------------------
//*Coupling (Servo):
namespace COUP {
//Defines the angle of the "Disengaged" position in degrees (°)
constexpr uint8_t DIS = 0;
//Defines the angle of the "Engaged" position in degrees (°)
constexpr uint8_t EN = 110;
//Minimum angle of the Servo in degrees (°) (Datasheet)
constexpr uint8_t MIN = 0;
//Maximum angle of the Servo in degrees (°) (Datasheet)
constexpr uint8_t MAX = 180;
//Time per 180 degrees (Datasheet or estimations/measurements)
//Used for error-management and timeout-calculations
constexpr uint8_t TPR = 250;
}

//---------------------------------------------------------------------------------------------
//*Hall-Sensor:
namespace HALL{
//Defines if the Hall-Sensor triggers LOW or HIGH (Datasheet)
//0 = LOW, 1 = HIGH
//Can be tested as well, if the Hall-Sensor is hooked up correctly
    //and then measured with and without a magnet
constexpr bool TRIGGERED_IF = 0; 
}









//===========================================================================
//! Dont change anything after! It's for the compiler to calculate the values.
//===========================================================================

//*Calculations Hammerwheel:
namespace HW{
//Calculation of the actual RPM according to the given speed in PWM and the gear ratio i
constexpr float RPM = (SPEED / 255.0) * RPM_MAX * i;        //RPM
//Calculation of the timeout used to detect an error if either the motor doesnt move or the hall-sensor is not triggered
// 1 min/ (RPM * nMagnets) -> 60000ms / (RPM * nMagnets):
constexpr uint32_t TIMEOUT = (RPM>0 && nMAGNETS>0) ? 60000/(RPM*nMAGNETS) * 2: 0;
//Calculation if the hammerwheel isnt spinning (while resetting to the starting position)
//(Height * 60000)/(RPM * 2 * PI * Radius)
constexpr uint32_t wtime_calc = (RPM>0 && HEIGHT_mm>0 && RADIUS_mm>0) ? (HEIGHT_mm * 60000)/(RPM * 2 * PI * RADIUS_mm) * 1.5 : 0;
//Factor speed/rs_speed -> Used for Timeout-Calculations while resetting the motor
constexpr float RS_TO_FACTOR = abs(float(SPEED)/float(RS_SPEED));
}

//*Calculations Slider:
namespace SL{

//If synchronized, the Speed is calculated according to the RPM of the Hammerwheel and the synchronization factor
constexpr int SPEED = (SYNCHRONIZED) ? HW::RPM/(RPM_MAX*i) * 255 * SYNC_FAC : speed_;
//Calculation of the actual RPM according to the given speed in PWM and the gear ratio i
//If synchronized, the RPM is calculated according to the RPM of the Hammerwheel and the synchronization factor
constexpr float RPM = (SYNCHRONIZED) ? HW::RPM * i : (SPEED / 255.0) * RPM_MAX * i;
//Calculates the time in minutes for the slider to move from one end to the other
constexpr float time_min = (RPM>0 && PITCH>0) ? L_mm/(RPM*PITCH) : 0;
//Calculates the timeout in ms
//If the slider didnt reach its destination in time, an error will be triggered
constexpr uint32_t TIMEOUT = time_min * 60000 * 1.2;
constexpr uint32_t stime_calc = TIMEOUT;
//Factor speed/rs_speed -> Used for Timeout-Calculations while resetting the motor
constexpr float RS_TO_FACTOR = abs(float(SPEED)/float(RS_SPEED));
}

//*Calculations Stepper:
namespace STP{
// Calculates the steps needed to reach the next surface of the sign
constexpr uint16_t POS = (STP::SPR * STP::MICRO_STEPS * (STP::ANGLE/360.0)) * STP::i;
}

//*Calculations Servo Hammerstop:
namespace HS{
//Calculates the timeout in ms
// timeout (ms) = abs(Max_Angle - Min_Angle) * Time_Per_Degree * 2 (for safety reasons)
//e.g. abs(180-0) * 0.14 * 2 = 50.4
constexpr uint16_t BLOCKTIME = abs(MAX - MIN) * TPD * 2;
}

//*Calculations Servo Coupling:
namespace COUP{
//Calculates the timeout in ms
// timeout (ms) = abs(Max_Angle - Min_Angle)/180° * Time_Per_180Degrees * 1.5 (for safety reasons)
//e.g. abs(180-0) * 0.14 * 2 = 50.4
constexpr uint16_t BLOCKTIME = abs(MAX - MIN)/180 * TPD * 1.5;
}
