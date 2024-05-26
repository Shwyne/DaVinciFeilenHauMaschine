#include "Sensors.hpp"

namespace Sensor {


//* Constructor for Endstops class (Only pin1 and pin2 are required)
Endstops::Endstops(uint8_t pin1, uint8_t pin2) {

    this->pin1 = pin1;  // Pin for Endstop 1
    this->pin2 = pin2;  // Pin for Endstop 2

    pinMode(pin1, INPUT_PULLUP);    // Set pin1 as input with pullup
    pinMode(pin2, INPUT_PULLUP);    // Set pin2 as input with pullup

    return;
}

//* Read the Endstops and returns the state
uint8_t Endstops::read() {

    // If both endstops are triggered -> state = 3
    if(digitalRead(pin1) == LOW && digitalRead(pin2) == LOW){
        state = 3;
    }

    // else if endstop 1 is triggered -> state = 1
    else if(digitalRead(pin1) == LOW){
        state = 1;
    }

    // else if endstop 2 is triggered -> state = 2
    else if(digitalRead(pin2) == LOW){
        state = 2;
    }

    // else (= no endstop is triggered) -> state = 0
    else{
        state = 0;
    }

    // Return the state of the Endstops
    return state;
}

//* Check if the state of the Endstops has changed, returns true if changed
bool Endstops::changed() {

    // Save the previous state of the Endstops
    uint8_t prevState = state;

    // If the state of the Endstops has changed, return true
    if(read() != prevState){
        return true;
    }

    else{
        return false;   // Else, return false
    }
}

}