#include "ServoExp.hpp"

ServoExp::ServoExp(uint8_t pin, uint8_t min, uint8_t max) {
    
    MIN_ = min;
    MAX_ = max;
    pos1_ = MIN_;
    pos2_ = MAX_;

    pin_ = pin;
}

void ServoExp::setPosData(uint8_t pos1, uint8_t pos2){
    
    //Set pos1 so it doesn't exceed min or max
    if(pos1 > MAX_) pos1 = MAX_;
    else if(pos1 < MIN_) pos1 = MIN_;

    //Set pos2 so it doesn't exceed min or max
    if(pos2 > MAX_) pos2 = MAX_;
    else if(pos2 < MIN_) pos2 = MIN_;

    //Attributes of object are set to parameters:
    pos1_ = pos1;
    pos2_ = pos2;
}

void ServoExp::attach() {

    //Attach servo to pin:
    Servo::attach(pin_);

    //Set target position to current position:
    if(posTarg_ == -1){
        posTarg_ = read();
    }
}

void ServoExp::run(uint8_t angle) {

    //Check if angle is in range and if not, corrects it:
    if(angle > MAX_)
        angle = MAX_;
    else if(angle < MIN_)
        angle = MIN_;

    //Set target position:
    posTarg_ = angle;

    //Write target position to servo:
    Servo::write(posTarg_);
}

bool ServoExp::reachedTarget() {

    //Check if target position is reached:
    return (posTarg_ == read()) ? true : false;
}

void ServoExp::pos1() {
    
    posTarg_ = pos1_;
    write(pos1_);
}

void ServoExp::pos2() {
    
    posTarg_ = pos2_;
    write(pos2_);
}

uint8_t ServoExp::getPosTarg() {

    return posTarg_;
}

void ServoExp::printData() {
    
    Serial.println("Pin: " + String(pin_));
    
    Serial.println("Current Position: " + String(read()));
    Serial.println("Target Position: " + String(posTarg_));
    Serial.println(String((reachedTarget()) ? "Target Position reached" : "Target Position not reached"));
    
    Serial.println("Minimum Angle: " + String(MIN_));
    Serial.println("Maximum Angle: " + String(MAX_));
    
    Serial.println("Position 1: " + String(pos1_));
    Serial.println("Position 2: " + String(pos2_));
}

ServoExp::~ServoExp() {
    
    detach();
}