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

void StepperExp::setPos(uint16_t steps, uint8_t angle) {
    this->StepsPerRev = steps;
    this->StepsPerPos = steps * microSteps * angle/360;
    return;
}

void StepperExp::nextPos() {
    if(SLPpin != 255) digitalWrite(SLPpin, HIGH);
    this->moveTo(this->currentPos + this->StepsPerPos);
    this->runToPosition();
    this->currentPos += this->StepsPerPos;
    if(SLPpin != 255 && autoSleep) digitalWrite(SLPpin, LOW);
    return;
}

void StepperExp::prevPos() {
    if(SLPpin != 255) digitalWrite(SLPpin, HIGH);
    this->moveTo(this->currentPos - this->StepsPerPos);
    this->runToPosition();
    this->currentPos -= this->StepsPerPos;
    if(SLPpin != 255 && autoSleep) digitalWrite(SLPpin, LOW);
    return;
}

void StepperExp::setSleep(uint8_t SLPpin, bool autoSleep) {
    this->SLPpin = SLPpin;
    this->autoSleep = autoSleep;
    pinMode(SLPpin, OUTPUT);
    if(autoSleep) {
        this->sleep();
    }
    return;
}

void StepperExp::sleep() {
    if(SLPpin == 255) return;
    if(this->isRunning()) {
        this->stop();
    }
    digitalWrite(SLPpin, LOW);
    return;
}

void StepperExp::wake() {
    if(SLPpin == 255) return;
    digitalWrite(SLPpin, HIGH);
    return;
}

void StepperExp::zeroPos() {
    this->currentPos = 0;
    return;
}