#pragma once

#include <Arduino.h>
#include "config.hpp"
#include "Sensors.hpp"
#include "Drivers.hpp"

extern MP6550 SLdc;
extern MP6550 HWdc;
extern ServoExp HSsv;
extern ServoExp COsv;
extern StepExp SGst;
//extern A4988 SGst;
extern Sensor::Endstops WGes;
extern Sensor::Endstops SLes;
extern Sensor::HallSwitch HWha;
extern Sensor::HallSwitch SGha;
extern Sensor::Button Go;

//Error-Handling:

enum class ErrCode : uint8_t {
    NO_ERROR,
    HW_TIMEOUT,
    SG_TIMEOUT,
    WG_TIMEOUT,
    SL_TIMEOUT,
    STPDRV_NOT_CONNECTED,
    WG_OVERCURRENT,
    SL_OVERCURRENT,
    COUP_NOT_ATTACHED,
    COUP_NOT_IN_POS,
    HS_NOT_ATTACHED,
    HS_NOT_IN_POS,
    UNDEFINED
};

extern ErrCode erCode;
extern uint32_t ctime;

//--------------------Enumerator-----------------------
//Endstops Weight:
namespace Weight {
    enum State {
        UNTRIGGERED = 0,
        TOP = 1,
        BOTTOM = 2,
};}

//Endstops Slider:
namespace Slider {
    enum State {
        UNTRIGGERED = 0,
        RIGHT = 1,
        LEFT = 2,
    };
}


//Positions of the Servo (OFF = pos1, ON = pos2):
namespace SERVO {
    enum State {
        OFF = 0,
        ON = 1,
    };
}



//============================================================
//*Hardware-Settings:

//Go-Button:
namespace LED{
    enum color{
        OFF = 0,
        RED = 1,
        GREEN = 2,
        BLUE = 3,
        YELLOW = 4,
        CYAN = 5,
        MAGENTA = 6,
        WHITE = 7,
    };
}

//--------------------Functions.cpp--------------------
namespace serv {
    void decouple();
    void couple();
    void hammerstop();
    void hammergo();
}

void check();
void IdentifyES();

void printError(ErrCode erCode);

//--------------------TestFunctions.cpp--------------------

//*Testing the Components
namespace testComp {

void TestMP6550(MP6550 DC, uint8_t ButtonPin = 255);
void TestServo(ServoExp srv, uint8_t ButtonPin = 255, uint8_t pos1 = 0, uint8_t pos2 = 180);
void TestStepper(StepExp stp, uint8_t ButtonPin = 255);
void TestEndstops(Sensor::Endstops es, Sensor::Button button, bool UseLED = false);
void TestHall(Sensor::HallSwitch hall, Sensor::Button button, bool UseLED = false);
void TestButton(Sensor::Button button, bool UseLED = true);

} // namespace testComp

namespace testFunc {

//*Funktionsgruppen
void WeightAndHammer();
void Slider();
void Sign(uint16_t delayTime = 1000);

} // namespace testFunc

namespace measure {
    //Going around three times with constanst speed to calculate the times inbetween every magnet in ms and print the average of the three times at the end.
void SliderTiming();
void MagnetsHR(MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns = 3, uint8_t speed = 255);
void TestWeight(MP6550 HWdc, Sensor::Endstops WGes, Sensor::HallSwitch HWhall, bool infinite = true);

} // namespace measure

//--------------------ExporFunctios.cpp--------------------

namespace exportData{

void MagnetTimings(MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns, uint8_t speed = 255);
void WeightCountMagnets(MP6550 mdc, Sensor::HallSwitch hall, Sensor::Endstops es, uint8_t speed = 255);
void ServoPositions(ServoExp servo, uint8_t angle1, uint8_t angle2, uint16_t iteration = 10, uint16_t delayTime = 500);
void SliderTiming(MP6550 mdc, Sensor::Endstops es, uint8_t speed = 255, uint8_t iterations = 10);

} // namespace export