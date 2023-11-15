#include "CustomServo.hpp"

CustomServo::CustomServo(uint8_t pin, uint8_t min, uint8_t max, uint8_t pos1, uint8_t pos2) {
    //Set min and max values:
    MIN_ = min;         
    MAX_ = max;
    //Set pos1 and pos2 so they dont exceed min and max and pos1 < pos2:
    if(pos1 > MAX_){
        pos1 = MAX_;
    }
    else if(pos1 < MIN_)
        pos1 = MIN_;
    if(pos2 > MAX_){
        pos2 = MAX_;
    }
    else if(pos2 < MIN_){
        pos2 = MIN_;
    }
    if(pos1_ > pos2_){
        uint8_t temp = pos1_;
        pos1_ = pos2_;
        pos2_ = temp;
    }
    pos1_ = pos1;
    pos2_ = pos2;
    posTarg_ = MIN_;
    pin_ = pin;
}

void CustomServo::attach() {
    //Attach servo to pin:
    Servo::attach(pin_);
}

void CustomServo::run(uint8_t angle) {
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

bool CustomServo::reachedTargetPos() {
    //Check if target position is reached:
    return (posTarg_ == read()) ? true : false;
}

void CustomServo::pos1() {
    write(pos1_);
}

void CustomServo::pos2() {
    write(pos2_);
}

uint8_t CustomServo::getPosTarg() {
    return posTarg_;
}

uint8_t CustomServo::getMax() {
    return MAX_;
}

uint8_t CustomServo::getMin() {
    return MIN_;
}

uint8_t CustomServo::getPos1() {
    return pos1_;
}

uint8_t CustomServo::getPos2() {
    return pos2_;
}

void CustomServo::printData() {
    Serial.print("Current Position: ");
    Serial.println(read());
    Serial.print("Target Position: ");
    Serial.println(getPosTarg());
    Serial.print("Min Position: ");
    Serial.println(getMin());
    Serial.print("Max Position: ");
    Serial.println(getMax());
    Serial.print("Pos1: ");
    Serial.println(getPos1());
    Serial.print("Pos2: ");
    Serial.println(getPos2());
}

CustomServo::~CustomServo() {
    detach();
}