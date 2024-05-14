#include "Sensors.hpp"

namespace Sensor {


//* Constructor for HallSwitch class (Only pin is required, rest is optional)
HallSwitch::HallSwitch(uint8_t pin, bool MagnetSignal) {
    this->pin = pin;    // Pin for the HallSwitch
    this->MagnetSignal = MagnetSignal;    // Triggered if HIGH or LOW (Depends on the sensor, Datasheet!)
    pinMode(pin, INPUT);    // Set pin as input
    return;   // Return
}

//* Read the HallSwitch and returns the state
bool HallSwitch::read() {
    // If the pin is detects a magnetic field, set detectsMF to true
    if(digitalRead(pin) == MagnetSignal){
        detectsMF = true;
    }
    // If the pin is not detecting a magnetic field, set detectsMF to false
    else{
        detectsMF = false;
    }
    // Return the state of the HallSwitch
    return detectsMF;
}

//* Check if the state of the HallSwitch has changed, returns true if changed
bool HallSwitch::changed() {
    // Save the previous state of the HallSwitch
    bool prevState = detectsMF;
    // If the state of the HallSwitch has changed, return true
    if(this->read() != prevState){
        return true;
    }
    // If the state of the HallSwitch has not changed, return false
    return false;
}

}