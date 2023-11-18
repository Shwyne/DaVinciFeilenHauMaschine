#include "CustomStepper.hpp"


CustomStepper::CustomStepper(uint8_t Interface, uint8_t STP, uint8_t DIR, uint8_t EN, int maxSpeed, int Accel, uint8_t HALL, bool HallTriggerState) : stp(Interface, STP, DIR, false), hall(HALL, HallTriggerState){
    stp.setEnablePin(EN);
    stp.setMaxSpeed(maxSpeed);
    stp.setAcceleration(Accel);
    MicroSteps_ = 0;
    Steps_ = 0;
}

void CustomStepper::setStepData(uint8_t MicroSteps, int steps){
    MicroSteps_ = MicroSteps;
    Steps_ = steps;
}

void CustomStepper::printData(){
    Serial.println("\nStepper-Data:");
    Serial.println("----------------------");
    switch(Interface_){
        case 0:
            Serial.println("Interface: FUNCTION");
            break;
        case 1:
            Serial.println("Interface: DRIVER");
            break;
        case 2:
            Serial.println("Interface: FULL2WIRE");
            break;
        case 3:
            Serial.println("Interface: FULL3WIRE");
            break;
        case 4:
            Serial.println("Interface: FULL4WIRE");
            break;
        case 6:
            Serial.println("Interface: HALF3WIRE");
            break;
        case 8:
            Serial.println("Interface: HALF4WIRE");
            break;
    }
    Serial.println("Microsteps: " + String(MicroSteps_));
    Serial.println("Steps: " + String(Steps_));
    Serial.println("MaxSpeed: " + String(MaxSpeed_));
    Serial.println("Acceleration: " + String(Accel_));
    Serial.println("Speed: " + String(stp.speed()));
    Serial.println("Position: " + String(stp.currentPosition()));
    Serial.println("Hall-Sensor: " + String(hall.read()));
    Serial.println("----------------------\n");
    Serial.println("Pins used:");
    Serial.println("Step-Pin: " + String(STP_));
    Serial.println("Direction-Pin: " + String(DIR_));
    Serial.println("Enable-Pin: " + String(EN_));
    Serial.println("Hall-Sensor-Pin: " + String(HALL_));
    Serial.println("-----------------\nEnd of Data\n-----------------");
}

bool CustomStepper::run(int speed){         //? This function has to be looped in the main loop?
    stp.setSpeed(speed);                    //! Shouldnt be used until tested!
    stp.move(speed);
    return stp.runSpeed();
}

bool CustomStepper::runTo(int pos, int speed){  //? This function has to be looped in the main loop?
    if(speed == 0 && stp.speed() == 0){         //! Shouldnt be used until tested!
        return false;
    }
    speed = (speed == 0) ? stp.speed() : abs(speed);
    stp.moveTo(pos);
    stp.setSpeed(speed);
    return stp.runSpeedToPosition();
}

void CustomStepper::home(int speed){
    stp.setSpeed(speed);
    while(hall.read() != 0){
        stp.runSpeed();
    }
    stp.setCurrentPosition(0);
}

int CustomStepper::getSpeed(){
    return stp.speed();
}

int CustomStepper::getPos(){
    return stp.currentPosition();
}

void CustomStepper::sleep(){
    stp.disableOutputs();
}

void CustomStepper::wakeUp(){
    stp.enableOutputs();
}