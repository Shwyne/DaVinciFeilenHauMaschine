#pragma once

#include <Arduino.h>
#include "config.hpp"
#include <EEPROM.h>
#include "Drivers.hpp"
#include "Sensors.hpp"

extern Sensor::Button Go; // External declaration of Sensor::Button Go
extern MP6550 SLdc; // External declaration of MP6550 SLdc
extern MP6550 HWdc; // External declaration of MP6550 HWdc

enum class CompStatus : uint8_t {
    NOT_CONNECTED = 0,  // Can't connect to device
    SUCCESS = 1,    // Success
    TIMEOUT = 2,    // Timeout
    ERROR = 3,      // Other Errors
    UNDEFINED = 255,    // Undefined
};

enum class FuncGroup : uint8_t {
    HW = 0,     // Hammerwheel - DC-Motor, Hall HW
    SL = 1,     // Slider - DC-Motor, Hall SG
    WG = 2,     // Weight - Endstops
    SG = 3,     // Sign - Stepper, Hall SG
    UNDEFINED = 255,    // Undefined - used for prototyping and testing
};

class StatusClass {
private:
    CompStatus status;
    FuncGroup group;
public:
    // Constructor for StatusClass with CompStatus and FuncGroup as input
    StatusClass(CompStatus status, FuncGroup group){
        this->status = status;
        this->group = group;
    }
    // Constructor for StatusClass with uint8_t as input
    StatusClass(uint8_t status, uint8_t group);
    // Get the function group:
    FuncGroup getComps(){
        return this->group;
    }
    // Get the status:
    CompStatus getStatus(){
        return this->status;
    }
    // Get the function group as uint8_t:
    uint8_t getCompsInt(){
        return static_cast<uint8_t>(this->group);
    }
    // Get the status as uint8_t:
    uint8_t getStatusInt(){
        return static_cast<uint8_t>(this->status);
    }
};

// ExceptionHandling
void printError(StatusClass status);      // Prints the error to the Serial Monitor

// EEPROM Functions
void writeToEEPROM(StatusClass status);   // Writes the Error-Code to the EEPROM
bool hasErrorEEPROM();  // Checks if an Error is stored in the EEPROM
void clearEEPROM();     // Clears the Error-Code from the EEPROM