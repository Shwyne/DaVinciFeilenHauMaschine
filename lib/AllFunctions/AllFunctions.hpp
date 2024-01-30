#pragma once

#include <Arduino.h>
#include "config.hpp"
#include "Error_Handling.hpp"

#include "DC_Motor_Driver.hpp"
#include "Sensor.hpp"
#include "ServoExp.hpp"
#include <DRV8825.h>
#include <A4988.h>

extern DC_Motor_Driver::MP6550 SLdc;
extern DC_Motor_Driver::MP6550 HWdc;
extern ServoExp HSsv;
extern ServoExp COsv;
//extern DRV8825 SGst;
extern A4988 SGst;
extern Sensor::Endstops WGes;
extern Sensor::Endstops SLes;
extern Sensor::HallSwitch HRha;
extern Sensor::HallSwitch SGha;
extern Sensor::Button Go;

extern ErrCode erCode;
extern uint32_t ctime;

//--------------------StateFunctions.cpp--------------------

//*DEFAULT SETUP:

namespace states {
void idle(bool firstExec);  // Idle state: Waiting for start button
void run(bool firstExec);   // Run state: Running motors till interval is or endstops are reached
void reset(); // Reset state: Resetting the system if an endstop is reached
void error(ErrCode erCode); // Error state: Throwing an error and trapping the system in an endless loop while blinking the go-led (or  changing it to red, if RGB-LED is used)
}// namespace states

//--------------------Functions.cpp--------------------
namespace serv {
    void decouple();
    void couple();
    void hammerstop();
    void hammergo();
}

namespace step {
    void pos1();
    void pos2();
    void pos3();
}

void IdentifyES();

//--------------------TestFunctions.cpp--------------------

namespace testComp {

//*Einzeltests
void TestDC(DC_Motor_Driver::MP6550 mdc);
void TestServo(ServoExp srv, uint8_t pos1 = 0, uint8_t pos2 = 180);
void TestServo(ServoExp srv, uint8_t Butpin, uint8_t pos1 = 0, uint8_t pos2 = 180);
void TestStepper(DRV8825 stp, bool hallInUse = false);

} // namespace testComp

namespace testFunc {

//*Funktionsgruppen
void TestHammer(DC_Motor_Driver::MP6550 HRdc, ServoExp HSsv, Sensor::Endstops WGes, Sensor::HallSwitch hallHr);
void TestSchlitten(DC_Motor_Driver::MP6550 moSl, ServoExp KUsv, Sensor::Endstops esSl);
void TestSchild(DRV8825 stp, Sensor::HallSwitch hall, bool hallInUse = false);
void TestWeight(DC_Motor_Driver::MP6550 HWdc, Sensor::Endstops WGes, bool infinite = true);
void TestWeightError(DC_Motor_Driver::MP6550 HWdc, Sensor::Endstops WGes,Sensor::HallSwitch HWhall, bool infinite = true);

} // namespace testFunc

namespace measure {
    //Going around three times with constanst speed to calculate the times inbetween every magnet in ms and print the average of the three times at the end.
void MagnetsHR(DC_Motor_Driver::MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns = 3, uint8_t speed = 255);
void TestWeight(DC_Motor_Driver::MP6550 HWdc, Sensor::Endstops WGes, Sensor::HallSwitch HWhall, bool infinite = true);

} // namespace measure

//--------------------ExporFunctios.cpp--------------------

namespace exportData{

void MagnetTimings(DC_Motor_Driver::MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns, uint8_t speed = 255);
void WeightCountMagnets(DC_Motor_Driver::MP6550 mdc, Sensor::HallSwitch hall, Sensor::Endstops es, uint8_t speed = 255);
void ServoPositions(ServoExp servo, uint8_t angle1, uint8_t angle2, uint16_t iteration = 10, uint16_t delayTime = 500);
void SliderTiming(DC_Motor_Driver::MP6550 mdc, Sensor::Endstops es, uint8_t speed = 255, uint8_t iterations = 10);

} // namespace export