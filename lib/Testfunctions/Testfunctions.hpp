#pragma once

#include <Arduino.h>
#include "Sensor.hpp"
#include "DC_Motor_Driver.hpp"
#include "ServoExp.hpp"
#include "config.hpp"
#include <DRV8825.h>



namespace testComp{

    //*Einzeltests
    void TestDC(MP6550 mdc);
    void TestServo(ServoExp srv, Endstop esGe);
    void TestServo(ServoExp srv);
    void TestStepper();
    void TestEndstop(Endstop es);
    void TestHall(Hall hall);
    void TestButton(uint8_t pin);

}
namespace testFunc{
    
    //*Funktionsgruppen
    void TestHammer(MP6550 mdc, ServoExp srvHs, Endstop esGe, Hall hallHr);
    void TestSchlitten(MP6550 moSl, Servo srvKu, Endstop esSl);
    void TestSchild(DRV8825 stp, bool hallInUse = false);

}