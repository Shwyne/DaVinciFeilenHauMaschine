#include "Sensors.hpp"

namespace Sensor {

HallSwitch::HallSwitch(uint8_t pin, bool triggeredIf) {
    this->pin = pin;
    this->triggeredIf = triggeredIf;
    pinMode(pin, INPUT);
    return;
}

bool HallSwitch::read() {
    if(digitalRead(pin) == triggeredIf){
        triggered = true;
    }
    else{
        triggered = false;
    }
    return triggered;
}

bool HallSwitch::changed() {
    bool prevState = triggered;
    if(read() != prevState){
        return true;
    }
    return false;
}

}