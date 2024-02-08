#include "Drivers.hpp"

StepExp::StepExp(uint8_t steps, uint8_t dir_pin, uint8_t step_pin, uint8_t enable_pin, uint8_t mode0_pin, uint8_t mode1_pin, uint8_t mode2_pin, uint8_t hall_pin)
    : DRV8825(steps, dir_pin, step_pin, enable_pin, mode0_pin, mode1_pin, mode2_pin), 
      hallSwitch(hall_pin, 0) {
}

void StepExp::begin(float rpm, uint8_t microsteps) {
    DRV8825::begin(rpm, microsteps);
}

void StepExp::run(int steps) {
    DRV8825::move(steps);
    position += steps;
    if(position > 360){
        position -= 360;
    }
    else if(position < 0){
        position += 360;
    }
    return;
}

void StepExp::home() {
    while(hallSwitch.read() == false){
        DRV8825::move(1);
    }
    position = 0;
}

void StepExp::sleep() {
    Standby = true;
    DRV8825::disable();
}

void StepExp::wake() {
    Standby = false;
    DRV8825::enable();
}

uint8_t StepExp::getPosition() {
    return position;
}

Status StepExp::sleepState() {
    if(Standby == true){
        return Status::Standby;
    }
    else{
        return Status::Active;
    }
}

