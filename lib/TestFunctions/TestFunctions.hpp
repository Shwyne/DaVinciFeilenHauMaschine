#pragma once

#include "Drivers.hpp"
#include "Sensors.hpp"
#include "config.hpp"

//*Objects -> Defined in main.cpp:

extern MP6550 SLdc;         //Slider
extern MP6550 HWdc;         //Hammerwheel
extern ServoExp HSsv;       //Hammerstop
extern ServoExp COsv;       //Coupling
extern StepperExp SGst;   //Slider Stepper
extern Sensor::Endstops WGes;       //Weight-Endstops
extern Sensor::Endstops SLes;       //Slider-Endstops
extern Sensor::HallSwitch HWha;     //Hammerwheel Hall-Switch
extern Sensor::HallSwitch SGha;     //Slider Hall-Switch
extern Sensor::Button Go;           //Go-Button

#define useButton TEST_BUTTON   //Settins for the Test-Button (Enabled/Disabled)
#define useButtonLED TEST_LED   //Settings for the Test-LED (Enabled/Disabled)

//------------------------------------------------------------

//*--------------------TestFunctions.cpp--------------------

void printEndstops(uint8_t status); //Prints the status of the Endstops

namespace testComp {
    void DC(MP6550 DC, uint8_t speed = 255);    //Tests a Motor-Driver
    void Servo(ServoExp srv, uint8_t pos1 = 0, uint8_t pos2 = 180);    //Tests a Servo
    void Stepper(StepperExp stp);    //Tests a Stepper
    void Endstops(Sensor::Endstops es);     //Tests a pair of Endstops
    void Hall(Sensor::HallSwitch hall);     //Tests a Hall-Switch
    void Button();     //Tests a Button
    void ButtonLED();  //Tests a Button with LED
}

namespace testFunc {
    void findEndstops();   //Identifies the Endstops
    void WeightHammer();   //Test the Weight and Hammerwheel
    void Slider();      //Test the Slider
    void Coupling();    //Test the Coupling
    void hammergo();    //Function for hammergo
    void hammerstop();  //Function for hammerstop
}
