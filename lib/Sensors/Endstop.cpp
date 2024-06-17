#include "Sensors.hpp"

namespace Sensor {


//* Constructor for Endstops class (Only pin1 and pin2 are required)
Endstops::Endstops(uint8_t pin1, uint8_t pin2) {

    this->pin1 = pin1;  // Pin for Endstop 1
    this->pin2 = pin2;  // Pin for Endstop 2

    pinMode(this->pin1, INPUT_PULLUP);    // Set pin1 as input with pullup
    pinMode(this->pin2, INPUT_PULLUP);    // Set pin2 as input with pullup

    return;
}

//* Read the Endstops and returns the state
uint8_t Endstops::read() {
    this->state = 0;   // Variable for calculating the state
    if(digitalRead(this->pin1) == LOW){
        this->state += 1;   //endstop 1 triggered -> state = 1
    }
    if(digitalRead(this->pin2) == LOW){
       this->state += 2;    //endstop 2 triggered -> state = 2
    }                       //endstop 1 and 2 triggered -> state = 3
    return this->state;    //return the state, if no endstop is triggered -> state = 0
}

//* Check if the state of the Endstops has changed, returns true if changed
bool Endstops::changed() {
    // Save the previous state of the Endstops
    uint8_t prevState = state;
    // If the state of the Endstops has changed, return true
    return (read() != prevState);
}

}