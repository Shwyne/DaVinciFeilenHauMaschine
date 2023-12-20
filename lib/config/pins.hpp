#pragma once

namespace pin {

    //*DC-Motor Hammerwheel:
    constexpr uint8_t HR_IN1 = 12;
    constexpr uint8_t HR_IN2 = 13;
    constexpr uint8_t HR_SLP = 11;
    constexpr uint8_t HR_VIS = A0;

    //*DC-Motor Schlitten:
    constexpr uint8_t SL_IN1 = 4;
    constexpr uint8_t SL_IN2 = 3;
    constexpr uint8_t SL_SLP = 2;
    constexpr uint8_t SL_VIS = A1;

    //*Servo-Motoren:
    constexpr uint8_t CO_SRV = 6;    //Kupplung
    constexpr uint8_t HS_SRV = 7;    //Hammerstop

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
    constexpr uint8_t WG_ES_T = 39;  //Endstop Weight Top    
    constexpr uint8_t WG_ES_B = 41;  //Endstop Weight unten    
    constexpr uint8_t SL_ES_L = 43;  //Endstop Slide left    //*brown   
    constexpr uint8_t SL_ES_R = 45;  //Endstop Slide right   //*green

    constexpr uint8_t HR_HALL = 49;  //Hall-Sensor Hammerwheel    
    constexpr uint8_t SG_HALL = 51;  //Hall-Sensor Sign

    constexpr uint8_t GO_BUT = 53;   //Taster "Go"
    constexpr uint8_t GO_LED = 52;   //LED "Go"   //! Change to another pin, 52 cant be used in final product.      

}