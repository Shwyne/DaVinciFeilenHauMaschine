#pragma once

#include <Arduino.h>
#include <AccelStepper.h>
#include "config.hpp"

//*Objects -> Defined in main.cpp:

extern MP6550 SLdc;         //Slider
extern MP6550 HWdc;         //Hammerwheel
extern ServoExp HSsv;       //Hammerstop
extern ServoExp COsv;       //Coupling
extern AccelStepper SGst;   //Slider Stepper
extern Sensor::Endstops WGes;       //Weight-Endstops
extern Sensor::Endstops SLes;       //Slider-Endstops
extern Sensor::HallSwitch HWha;     //Hammerwheel Hall-Switch
extern Sensor::HallSwitch SGha;     //Slider Hall-Switch
extern Sensor::Button Go;           //Go-Button

//------------------------------------------------------------

//*--------------------TestFunctions.cpp--------------------

//Testing the Components
namespace testComp {

void TestMP6550(MP6550 DC, uint8_t speed = 255);    //Test the Motor-Driver
void TestServo(ServoExp srv, uint8_t pos1 = 0, uint8_t pos2 = 180);    //Test the Servo
void TestStepper(AccelStepper stp);    //Test the Stepper
void TestEndstops(Sensor::Endstops es);     //Test the Endstops
void TestHall(Sensor::HallSwitch hall);     //Test the Hall-Switch
void TestButton();     //Test the Button

} // namespace testComp

namespace testFunc {

//*Funktionsgruppen
void IdentifyES();          //Identifies the Endstops
void WeightAndHammer();     //Test the Weight and Hammerwheel
void Slider();              //Test the Slider
void Sign(uint16_t delayTime = 1000);   //Test the Sign
void Machine();                         //Test the whole Machine

} // namespace testFunc
