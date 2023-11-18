#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>

namespace pin {

    //*DC-Motor Hammerrad:
    const uint8_t HR_IN1 = 12;
    const uint8_t HR_IN2 = 13;
    const uint8_t HR_SLP = 11;
    const uint8_t HR_VIS = A0;

    //*DC-Motor Schlitten:
    const uint8_t SL_IN1 = 4;
    const uint8_t SL_IN2 = 3;
    const uint8_t SL_SLP = 2;
    const uint8_t SL_VIS = A1;

    //*Servo-Motoren:
    const uint8_t HS_SRV = 9;    //Hammerstop
    const uint8_t KU_SRV = 7;    //Kupplung

    //*Stepper:
    const uint8_t STP_DIR = 32;     //!Zurückändern zu 18!!
    const uint8_t STP_STP = 17;
    const uint8_t STP_SLP = 16;
    const uint8_t STP_RST = 15;
    const uint8_t STP_FLT = 14;

    //*Sensorik:
    const uint8_t GE_ES_O = 18;  //Endschalter Gewicht oben     //!Zurückändern auf 39!!
    const uint8_t GE_ES_U = 41;  //Endschalter Gewicht unten
    const uint8_t SL_ES_L = 43;  //Endschalter Schlitten links
    const uint8_t SL_ES_R = 45;  //Endschalter Schlitten rechts

    const uint8_t HR_HALL = 49;  //Hall-Sensor Hammerrad    
    const uint8_t SH_HALL = 51;  //Hall-Sensor Schild

    const uint8_t GO_BUT = 53;   //Taster "Go"

}
    //*Serial-Connection:
    #define SERIAL_BPS 9600 //Serial Baudrate

    //*Pause-Settings:
    const short DELAY = 1000; //Default delay for testing purposes
    const short DELAY_2 = 500; //Default delay for testing purposes
    const uint8_t DELAY_4 = 250; //Default delay for testing purposes

    //*Endstop-Settings:
    const uint8_t ES_MODE = 0; //Endstop Mode (0 = Pullup, 1 = Pulldown) - default: 0

    //*Stepper-Settings:
    const uint8_t STP_INTERFACE = 1;    //Stepper Interface (1 = Driver)
    const uint8_t STP_MODE = 1;  //Stepper Mode (1 = Full Step, 2 = Half Step, 4 = Quarter Step, 8 = Eighth Step)
    const short STP_STEPS = 200;  //Stepper Steps per Revolution
    const short STP_MAXSPEED = 100;   //Stepper Speed in Steps/Second -> Translate to RPS(s^-1): Steps * Steps/s = RPS -> *60 = RPM (min^-1)
    const short STP_ACCEL = 30;   //Stepper Acceleration
    const short STP_SPEED = 50;  //Stepper Speed

    //*Servo-Settings:
    const uint8_t SRV_MIN = 0;   //Servo min Angle
    const uint8_t SRV_MAX = 180; //Servo max Angle

    //*Sensor-Settings:
    const bool HALL_TRIGGER = 0;    //Hall-Sensor Trigger State (0 = LOW, 1 = HIGH)

namespace pos{
    
    //* Servo-Positions:
    
    const uint8_t HS_1 = 0;   //Hammerstop Position 1
    const uint8_t HS_2 = 80;  //Hammerstop Position 2
    const uint8_t KU_1 = 0;   //Kupplung Position 1
    const uint8_t KU_2 = 90;  //Kupplung Position 2
    
    //*Stepper-Positions:
    const short STP_1 = 0; //Schild Position 1
    const short STP_2 = 132;   //Schild Position 2
    const short STP_3 = 264;   //Schild Position 3

}

#endif // CONFIG_HPP
