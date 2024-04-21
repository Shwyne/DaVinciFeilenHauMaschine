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

//------------------------------------------------------------

//*Error-Handling:

enum class CompStatus : uint8_t {
    NOT_CONNECTED = 0,  //Cant connect to device
    SUCCESS = 1,    //Success
    TIMEOUT = 2,    //Timeout
    ERROR = 3,      //Other Errors
    UNDEFINED = 255,    //Undefined
};

enum class FuncGroup : uint8_t {
    HW = 0,     //Hammerwheel - DC-Motor, Hall HW
    SL = 1,     //Slider - DC-Motor, Hall SG
    CO = 2,     //Coupling - Servo
    HS = 3,     //Hammerstop - Servo
    WG = 4,     //Weight - Endstops
    SG = 5,     //Sign - Stepper, Hall SG
    UNDEFINED = 255,    //Undefined - used for prototyping and testing
};

class StatusClass {
    private:
        CompStatus status;
        FuncGroup group;
    public:
        StatusClass(CompStatus status, FuncGroup group){
            this->status = CompStatus::UNDEFINED;
            this->group = FuncGroup::UNDEFINED;
        }
        //get the function group:
        FuncGroup getComps(){
            return this->group;
        }
        //get the status:
        CompStatus getStatus(){
            return this->status;
        }
};

//*-------------------- Math -----------------------

//Calculates the logarithm to the base 2
uint8_t log2(uint8_t n);


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
    StatusClass decouple();    //Decouples the coupling
    StatusClass couple();      //Couples the coupling
    StatusClass hammerstop();  //Stops the hammerwheel by engaging the hammerstop in the right position
    StatusClass hammergo();    //Releases the hammerstop
    CompStatus waitForTarget(ServoExp srv, uint16_t timeout);    //Waits for the servo to reach its target
}

//Stepper Functions
namespace step {
    void setMicroSteps(uint8_t microSteps); //Sets the microsteps for the stepper (1-32);
    StatusClass home();                            //Homes the stepper (Hall-Sensor and Magnet -> Pos1)
    void move(int steps);                   //Moves the stepper a specific amount of steps
}

//Error Functions
void ErrorState(StatusClass status);    //Handles the Error-States
void showErrorLED();    //Shows the error on the Go-Button LED and catches the Program in a while loop till the error is cleared
void printError();      //Prints the error to the Serial Monitor

//EEPROM Functions
void writeToEEPROM(StatusClass status);   //Writes the Error-Code to the EEPROM
StatusClass readFromEEPROM();             //Reads the Error-Code from the EEPROM
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
