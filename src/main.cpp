#include <Arduino.h>    //Arduino Library
#include "MP6550_driver.hpp"     //Driver MP6550 Library
#include "CustomServo.hpp"      //Custom Servo Library -> depends on Servo.h
#include "Sensors.hpp"          //Sensor Library
#include "config.hpp"           //Config file -> Pins, settings, etc.
#include "Testfunctions.hpp"    //Testfunctions
#include <AccelStepper.h>       //Stepper Library
#include "CustomStepper.hpp"    //Custom Stepper Library -> depends on AccelStepper.h

//* functions:

//* objects:
motor moHr(pin::HR_IN1, pin::HR_IN2, pin::HR_SLP);  //Motor Hammerrad (In1, In2, Sleep)
motor moSl(pin::SL_IN1, pin::SL_IN2, pin::SL_SLP);  //Motor Schlitten (In1, In2, Sleep)
CustomServo srvHs(pin::HS_SRV, SRV_MIN, SRV_MAX, pos::HS_1, pos::HS_2);  //Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
CustomServo srvKu(pin::KU_SRV, SRV_MIN, SRV_MAX, pos::KU_1, pos::KU_2);  //Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
CustomStepper stp(STP_INTERFACE, pin::STP_STP, pin::STP_DIR, pin::STP_SLP, STP_MAXSPEED, STP_ACCEL, pin::SH_HALL, HALL_TRIGGER);    //Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin)
Endstop esGe(pin::GE_ES_O, pin::GE_ES_U, ES_MODE);   //Endschalter Gewicht: State 1 = Oben, State 2 = Unten
Endstop esSl(pin::SL_ES_L, pin::SL_ES_R, ES_MODE);    //Endschalter Schlitten: State 1 = Links, State 2 = Rechts
Hall hallHr(pin::HR_HALL);                      //Hall-Sensor Hammerrad 


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
}

void loop() {
    //testing::TestHammer(moHr, srvHs, esGe, hallHr);
    testing::TestServo(srvHs);
    //testing::TestSchlitten(moSl, srvKu, esSl);
    //testing::TestSchild(stp, hallSh);

}