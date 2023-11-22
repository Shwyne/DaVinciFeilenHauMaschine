#ifndef Testfunctions_hpp
#define Testfunctions_hpp

#include <Arduino.h>
#include "Sensors.hpp"
#include "MP6550_driver.hpp"
#include "ServoExp.hpp"
#include <AccelStepper.h>
#include "config.hpp"



namespace testing{

    //*Einzeltests
    void TestDC(MP6550 mdc);
    void TestServo(ServoExp srv, Endstop esGe);
    void TestServo(ServoExp srv);
    void TestStepper();
    void TestEndstop(Endstop es);
    void TestHall(Hall hall);
    void TestButton(uint8_t pin);
    
    //*Funktionsgruppen
    void TestHammer(MP6550 mdc, ServoExp srvHs, Endstop esGe, Hall hallHr);
    void TestSchlitten(MP6550 moSl, Servo srvKu, Endstop esSl);
    void TestSchild(AccelStepper stp, Hall hallSh);

}

#endif