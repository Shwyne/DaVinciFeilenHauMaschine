#include "Sensors.hpp"

Hall::Hall(uint8_t pin) {
    pin_ = pin;
    pinMode(pin_, INPUT);
    TriggerState_ = HIGH;
}

Hall::Hall(uint8_t pin, bool TriggerState) {
    pin_ = pin;
    pinMode(pin_, INPUT);
    TriggerState_ = TriggerState;
}

bool Hall::read() {
    if(digitalRead(pin_) == TriggerState_) {
        return true;
    } else {
        return false;
    }
}

void Hall::waitUntil(bool state){
    while(read() != state) {
        delay(1);
    }
}

void Hall::printData(){
    Serial.println("\nHall-Sensor-Data:");
    Serial.println("----------------------");
    Serial.println("Pin: " + String(pin_));
    Serial.println("Trigger-State: " + String(TriggerState_));
    if(read() == TriggerState_){
        Serial.println("State: Triggered");
    }
    else{
        Serial.println("State: Untriggered");
    }
    Serial.println("----------------------\n");
    Serial.println("End of Data");
    Serial.println("-----------------");
}

Hall::~Hall() {
    // Nothing to do here
}


Endstop::Endstop(uint8_t pin1, uint8_t pin2) {
    pin1_ = pin1;
    pin2_ = pin2;
    mode_ = 0;
    pinMode(pin1_, INPUT_PULLUP);
    pinMode(pin2_, INPUT_PULLUP);
}

Endstop::Endstop(uint8_t pin1, uint8_t pin2, bool mode) {
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

uint8_t Endstop::read() {
    
    if(digitalRead(pin1_) == mode_) {
        return 1;
    } else if(digitalRead(pin2_) == mode_) {
        return 2;
    } else {
        return 0;
    }
}

void Endstop::waitUntil(uint8_t state){
    while(read() != state) {
        delay(1);
    }
}


void Endstop::printData(){
    Serial.println("\nEndstop-Data:");
    Serial.println("----------------------");
    Serial.println("Pin 1: " + String(pin1_));
    Serial.println("Pin 2: " + String(pin2_));
    Serial.println("Mode: " + String(mode_));
    switch(read()){
        case 0:
            Serial.println("State: Untriggered");
            break;
        case 1:
            Serial.println("State: Triggered by Pin 1");
            break;
        case 2:
            Serial.println("State: Triggered by Pin 2");
            break;
    }
    Serial.println("----------------------\n");
    Serial.println("End of Data");
    Serial.println("-----------------");
}

Endstop::~Endstop() {
    // Nothing to do here
}