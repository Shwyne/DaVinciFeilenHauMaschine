#include "Sensors.hpp"

Hall::Hall(int pin) {
    pin_ = pin;
    pinMode(pin_, INPUT);
}

bool Hall::read() {
    state_ = digitalRead(pin_);
    return state_;
}

Hall::~Hall() {
    // Nothing to do here
}

Endstop::Endstop(int pin1, int pin2, int mode) {
    pin1_ = pin1;
    pin2_ = pin2;
    mode_ = mode;
    if(mode_ == 0){
        pinMode(pin1_, INPUT_PULLUP);
        pinMode(pin2_, INPUT_PULLUP);
    }
    else{
        pinMode(pin1_, INPUT);
        pinMode(pin2_, INPUT);
    }
}

int Endstop::getState() {
    
    if(digitalRead(pin1_) == mode_) {
        state_ = 1;
    } else if(digitalRead(pin2_) == mode_) {
        state_ = 2;
    } else {
        state_ = 0;
    }
    return state_;
}

Endstop::~Endstop() {
    // Nothing to do here
}