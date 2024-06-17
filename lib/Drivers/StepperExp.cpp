#include "Drivers.hpp"

uint8_t StepperExp::log2(uint8_t n){
    uint8_t result = 0;
    while(n >>= 1){
        result++;
    }
    return result;
}

void StepperExp::setupMS(uint8_t M0pin, uint8_t M1pin, uint8_t M2pin) {
    this->M0pin = M0pin;
    this->M1pin = M1pin;
    this->M2pin = M2pin;
    pinMode(M0pin, OUTPUT);
    pinMode(M1pin, OUTPUT);
    pinMode(M2pin, OUTPUT);
    return;
}

void StepperExp::setMS(uint8_t microSteps) {
    uint8_t log2MicroSteps = (microSteps > 0) ? this->log2(microSteps) : 0;
    this->microSteps = microSteps;
    // Calculate the values for M0, M1, and M2
    bool M0 = log2MicroSteps & 1;
    bool M1 = log2MicroSteps & 2;
    bool M2 = log2MicroSteps & 4;
    // Write the values to the pins
    digitalWrite(M0pin, M0);
    digitalWrite(M1pin, M1);
    digitalWrite(M2pin, M2);
    return;
}

uint8_t StepperExp::getMS() {
    return this->microSteps;
}

void StepperExp::setPos(uint16_t stepsPerRev, uint8_t angle) {  
    this->StepsPerRev = stepsPerRev;    // Set Steps per Revolution
    this->StepsPerPos = this->StepsPerRev * this->microSteps * angle/360;   // Calculate Steps per Position
    return;
}

void StepperExp::nextPos() {
    if(SLPpin != 255){  // If SLPpin is used, set it to HIGH (Driver active)
        digitalWrite(SLPpin, HIGH);
    }
    uint32_t targetPos = this->currentPos + this->StepsPerPos; // Calculate the target position
    this->moveTo(targetPos);    // Sets the target position as the target
    this->runToPosition();  // Runs the motor to the target position
    this->currentPos = targetPos;   // Updates the current position
    if(SLPpin != 255 && autoSleep){
        digitalWrite(SLPpin, LOW);  //if SLPpin is set and autoSleep is active, put Driver to sleep
    } 
    return;
}

void StepperExp::prevPos() {
    if(SLPpin != 255){  // If SLPpin is used, set it to HIGH (Driver active)
        digitalWrite(SLPpin, HIGH);
    } 
    uint32_t targetPos = this->currentPos - this->StepsPerPos; // Calculate the target position
    this->moveTo(targetPos);    // Sets the target position as the target
    this->runToPosition();  // Runs the motor to the target position
    this->currentPos = targetPos;   // Updates the current position
    if(SLPpin != 255 && autoSleep){
        digitalWrite(SLPpin, LOW);  //if SLPpin is set and autoSleep is active, put Driver to sleep
    }
    return;
}

void StepperExp::setSleep(uint8_t SLPpin, bool autoSleep) {
    this->SLPpin = SLPpin;  // Set the SLPpin
    this->autoSleep = autoSleep;    // Set the autoSleep
    pinMode(SLPpin, OUTPUT);    // Set the SLPpin to OUTPUT
    if(autoSleep) { // If autoSleep is active,
        this->sleep();  // ... put the driver to sleep
    }
    return;
}

void StepperExp::sleep() {
    if(SLPpin == 255) return;   // If SLPpin is not used, return
    if(this->isRunning()) { // If the motor is running, ...
        this->stop();   // ... stop the motor
    }
    digitalWrite(SLPpin, LOW);  // Set SLPpin to LOW -> Standby
    return;
}

void StepperExp::wake() {
    if(SLPpin == 255) return;   // If SLPpin is not used, return
    digitalWrite(SLPpin, HIGH); // Set SLPpin to HIGH -> Active
    return;
}

void StepperExp::zeroPos() {
    this->currentPos = 0;   // Set the current position to 0
    return;
}