#pragma once

#include <Arduino.h>
#include <AccelStepper.h>
#include "config.hpp"
#include "Sensors.hpp"
#include "Drivers.hpp"
#include "EEPROM.h"

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

//*Error-Codes:

enum class ErrCode : uint8_t {
    NO_ERROR = 0,       //No Error
    HW_TIMEOUT = 1,     //Hammerwheel Timeout
    SG_TIMEOUT = 2,     //Slider Timeout
    WG_TIMEOUT = 3,     //Weight Timeout
    SL_TIMEOUT = 4,     //Slider Timeout
    COUP_NOT_ATTACHED = 5,  //Coupling not attached
    COUP_NOT_IN_POS = 6,    //Coupling not in position
    HS_NOT_ATTACHED = 7,    //Hammerstop not attached
    HS_NOT_IN_POS = 8,      //Hammerstop not in position
    UNDEFINED = 255,        //Undefined Error
};


//* Global Variables -> Defined in main.cpp:
extern ErrCode erCode;   //Error-Code
extern uint32_t ctime;  //Current Time

//*--------------------Enumerator-----------------------

//*Endstops Weight:
namespace Weight {
//Used for the Weight-Endstops to make code more readable.
//Example: if(WGes.read() == Weight::TOP) -> If the weight is at the top.
enum State {
    UNTRIGGERED = 0,    //Untriggered
    TOP = 1,            //Weight triggered at the top
    BOTTOM = 2,         //Weight triggered at the bottom
};}

//*Endstops Slider:
namespace Slider {
//Used for the Slider-Endstops to make code more readable.
//Example: if(SLes.read() == Slider::RIGHT) -> If the slider is at the right endstop.
enum State {
    UNTRIGGERED = 0,    //Untriggered
    RIGHT = 1,          //Slider reached the right endstop
    LEFT = 2,           //Slider reached the left endstop
};}


//*Positions of the Servo (OFF = pos1, ON = pos2):
//Can be used for functions that should work with both servos. e.g. TestServo()
//Its important that pos1 and pos2 got set with myServo.setPositions(pos1, pos2) before using this enum.
//Otherwise the positions are not defined and the servo will not move.
//Example: myServo.runToPos(SERVO::OFF); -> Moves the servo to position 1.
namespace SERVO {
    enum State {
        OFF = 0,    //Position 1
        ON = 1,     //Position 2
    };
}

//============================================================
//*Hardware-Settings:

//*Go-Button:
//If the Go-Button has RGB-LEDs, it can be updated using this enum.
//Example: Go.updateLED(LED::RED); -> Updates the LED to red.
namespace LED{
    enum color{
        OFF = 0,        //LED off
        RED = 1,        //LED red
        GREEN = 2,      //LED green
        BLUE = 3,       //LED blue
        YELLOW = 4,     //LED yellow
        CYAN = 5,       //LED cyan
        MAGENTA = 6,    //LED magenta
        WHITE = 7,      //LED white
    };
}

//*--------------------Functions.cpp--------------------

//Servo Functions
namespace serv {
    void decouple();    //Decouples the coupling
    void couple();      //Couples the coupling
    void hammerstop();  //Stops the hammerwheel by engaging the hammerstop in the right position
    void hammergo();    //Releases the hammerstop
}

//Stepper Functions
namespace step {
    void setMicroSteps(uint8_t microSteps); //Sets the microsteps for the stepper (1-32);
    void home();                            //Homes the stepper (Hall-Sensor and Magnet -> Pos1)
    void move(int steps);                   //Moves the stepper a specific amount of steps
}

//Error Functions
void check();           //Checks the hardware for errors
void showErrorLED();    //Shows the error on the Go-Button LED and catches the Program in a while loop till the error is cleared
void printError();      //Prints the error to the Serial Monitor

//EEPROM Functions
void writeToEEPROM();   //Writes the Error-Code to the EEPROM
bool hasErrorEEPROM();  //Checks if an Error is stored in the EEPROM
void clearEEPROM();     //Clears the Error-Code from the EEPROM

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

namespace measure {
void SliderTiming();    //Measures the Slider-Timing
void MagnetsHR(MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns = 3, uint8_t speed = 255);    //Measures the Hammerwheel-Timing
void TestWeight(MP6550 HWdc, Sensor::Endstops WGes, Sensor::HallSwitch HWhall, bool infinite = true);   //Measures the Weight-Timing

} // namespace measure

//--------------------ExporFunctios.cpp--------------------

namespace exportData{

void MagnetTimings(MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns, uint8_t speed = 255);    //Measures the Hammerwheel-Timing and can export them to e.g. excel using serial
void WeightCountMagnets(MP6550 mdc, Sensor::HallSwitch hall, Sensor::Endstops es, uint8_t speed = 255);   //Counts the magnets of the weight and can export them to e.g. excel using serial
void ServoPositions(ServoExp servo, uint8_t angle1, uint8_t angle2, uint16_t iteration = 10, uint16_t delayTime = 500);   //Measures the Servo-Positions and can export them to e.g. excel using serial
void SliderTiming(MP6550 mdc, Sensor::Endstops es, uint8_t speed = 255, uint8_t iterations = 10);   

} // namespace export