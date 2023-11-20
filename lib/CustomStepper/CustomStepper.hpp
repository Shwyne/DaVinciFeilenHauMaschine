#ifndef CustomStepper_HPP
#define CustomStepper_HPP

#include <Arduino.h>
#include <AccelStepper.h>
#include "Sensors.hpp"
#include <math.h>

class CustomStepper{

    private:
        //*AccelStepper Object with Attributes:
        AccelStepper stp;
        
        uint8_t Interface_;
        uint8_t MicroSteps_;
        int Steps_;
        int MaxSpeed_;
        int Accel_;

        uint8_t STP_;
        uint8_t DIR_;
        uint8_t EN_;

        //*Hall-Sensor Object with Attributes:
        Hall hall;
        
        bool TriggerHallState_;
        uint8_t HALL_;

    public:
        CustomStepper(uint8_t Interface, uint8_t STP, uint8_t DIR, uint8_t EN, int maxSpeed, int Accel, uint8_t HALL, bool TriggerHallState);
        void setStepData(uint8_t MicroSteps, int Steps);
        void printData();
        
        int getSpeed();
        int getPos();

        bool run(int speed);
        bool runTo(int pos, int speed);
        void home(int speed);
        
        void sleep();
        void wakeUp();

};

#endif // CUSTOMSTEPPER_HPP
