#ifndef Testfunctions_hpp
#define Testfunctions_hpp

#include <Arduino.h>
#include "Sensors.hpp"
#include "MP6550_driver.hpp"
#include "CustomServo.hpp"
#include <AccelStepper.h>
#include "config.hpp"

//*Einzeltests

namespace testing{

    void TestDC(motor mdc);
    void ServoData(CustomServo srv);
    void TestServo(CustomServo srv, Endstop esGe);
    void TestServo(CustomServo srv);
    void TestStepper();
    void TestES(Endstop es);
    void TestHall(Hall hall);
    void TestButton(uint8_t pin);

    void TestHammer(motor mdc, CustomServo srvHs, Endstop esGe, Hall hallHr);
    void TestSchlitten(motor moSl, Servo srvKu, Endstop esSl);
    void TestSchild(AccelStepper stp, Hall hallSh);

}

#endif