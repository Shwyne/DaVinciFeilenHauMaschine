#include <Arduino.h>    //Arduino Library
#include "MP6550_driver.hpp"     //Driver MP6550 Library
#include "CustomServo.hpp"      //Custom Servo commands
#include "Sensors.hpp"          //Sensor Library
#include "config.hpp"           //Config file -> Pins, settings, etc.
#include "Testfunctions.hpp"    //Testfunctions
#include <AccelStepper.h>       //Stepper Library

//* functions:

//* objects:
motor moHr(pin::HR_IN1, pin::HR_IN2, pin::HR_SLP);  //Motor Hammerrad (In1, In2, Sleep)
motor moSl(pin::SL_IN1, pin::SL_IN2, pin::SL_SLP);  //Motor Schlitten (In1, In2, Sleep)
CustomServo srvHs(pin::HS_SRV, SRV_MIN, SRV_MAX, pos::HS_1, pos::HS_2);  //Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
CustomServo srvKu(pin::KU_SRV, SRV_MIN, SRV_MAX, pos::KU_1, pos::KU_2);  //Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
AccelStepper stp(STP_INTERFACE, pin::STP_STP, pin::STP_DIR);    //Stepper Schild (Interface, Step, Direction)
Endstop esGe(pin::GE_ES_O, pin::GE_ES_U, ES_MODE);   //Endschalter Gewicht: State 1 = Oben, State 2 = Unten
Endstop esSl(pin::SL_ES_L, pin::SL_ES_R, ES_MODE);    //Endschalter Schlitten: State 1 = Links, State 2 = Rechts
Hall hallHr(pin::HR_HALL);                      //Hall-Sensor Hammerrad
Hall hallSh(pin::SH_HALL);                      //Hall-Sensor Schild


void setup() {

    //pinMode(pin::GO_BUT, INPUT_PULLUP);    //Start-Button

    //* Serial-Setup:
    Serial.begin(SERIAL_BPS);

    //* Servo-Setup:
    srvHs.attach();
    srvKu.attach();
    //*Servo-Data to Serial:
    //srvHs.printData();
    //srvKu.printData();

    //*Stepper-Setup:
    stp.setMaxSpeed(STP_SPEED);
    stp.setAcceleration(STP_ACCEL);
}

void loop() {
    //testing::TestHammer(moHr, srvHs, esGe, hallHr);
    testing::TestServo(srvHs);
    //testing::TestSchlitten(moSl, srvKu, esSl);
    //testing::TestSchild(stp, hallSh);

}