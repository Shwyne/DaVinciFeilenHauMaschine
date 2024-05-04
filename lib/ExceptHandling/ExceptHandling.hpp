#pragma once

#include <Arduino.h>
#include "config.hpp"
#include <EEPROM.h>
#include "Drivers.hpp"
#include "Sensors.hpp"

extern Sensor::Button Go;
extern MP6550 SLdc;
extern MP6550 HWdc;

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
    WG = 2,     //Weight - Endstops
    SG = 3,     //Sign - Stepper, Hall SG
    UNDEFINED = 255,    //Undefined - used for prototyping and testing
};

class StatusClass {
    private:
        CompStatus status;
        FuncGroup group;
    public:
        StatusClass(CompStatus status, FuncGroup group){
            this->status = status;
            this->group = group;
        }
        StatusClass(uint8_t status, uint8_t group){
            switch(status){
                case 0:
                    this->status = CompStatus::NOT_CONNECTED;
                    break;
                case 1:
                    this->status = CompStatus::SUCCESS;
                    break;
                case 2:
                    this->status = CompStatus::TIMEOUT;
                    break;
                case 3:
                    this->status = CompStatus::ERROR;
                    break;
                default:
                    this->status = CompStatus::UNDEFINED;
                    break;
            }
            switch(group){
                case 0:
                    this->group = FuncGroup::HW;
                    break;
                case 1:
                    this->group = FuncGroup::SL;
                    break;
                case 2:
                    this->group = FuncGroup::WG;
                    break;
                case 3:
                    this->group = FuncGroup::SG;
                    break;
                default:
                    this->group = FuncGroup::UNDEFINED;
                    break;
            }
            return;
        }
        //get the function group:
        FuncGroup getComps(){
            return this->group;
        }
        //get the status:
        CompStatus getStatus(){
            return this->status;
        }
        uint8_t getCompsInt(){
            return static_cast<uint8_t>(this->group);
        }
        uint8_t getStatusInt(){
            return static_cast<uint8_t>(this->status);
        }
};

//ExceptionHandling
void printError(StatusClass status);      //Prints the error to the Serial Monitor

//EEPROM Functions
void writeToEEPROM(StatusClass status);   //Writes the Error-Code to the EEPROM
bool hasErrorEEPROM();  //Checks if an Error is stored in the EEPROM
void clearEEPROM();     //Clears the Error-Code from the EEPROM