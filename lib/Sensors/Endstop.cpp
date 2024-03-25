#include "Sensors.hpp"

namespace Sensor {

Endstops::Endstops(uint8_t pin1, uint8_t pin2) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
    return;
}

uint8_t Endstops::read() {
    if(digitalRead(pin1) == LOW && digitalRead(pin2) == LOW){
        state = 3;
    }
    else if(digitalRead(pin1) == LOW){
        state = 1;
    }
    else if(digitalRead(pin2) == LOW){
        state = 2;
    }
    else{
        state = 0;
    }
    return state;
}

bool Endstops::changed() {
    uint8_t prevState = state;
    if(read() != prevState){
        return true;
    }
    return false;
}

}