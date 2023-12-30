#pragma once

#include "DC_Motor_Driver.hpp"
#include "Sensor.hpp"
#include "ServoExp.hpp"
#include "config.hpp"
#include <Arduino.h>
#include <DRV8825.h>
#include "Functions.hpp"

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

} // namespace testFunc

namespace measure {
    //Going around three times with constanst speed to calculate the times inbetween every magnet in ms and print the average of the three times at the end.
void MagnetsHR(DC_Motor_Driver::MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns = 3, uint8_t speed = 255);

} // namespace measure