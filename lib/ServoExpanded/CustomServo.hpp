#ifndef CustomServo_hpp
#define CustomServo_hpp

#include <Servo.h>
#include <Arduino.h>

class CustomServo : public Servo {
    private:
        uint8_t MIN_;           //Minimum angle
        uint8_t MAX_;           //Maximum angle
        uint8_t pos1_;          //Position 1
        uint8_t pos2_;          //Position 2
        uint8_t posTarg_;       //Target position
        uint8_t pin_;         //Pin
    public:
        CustomServo(uint8_t pin, uint8_t min, uint8_t max, uint8_t pos1, uint8_t pos2);//Constructor
        void attach();   //Attach servo to pin
        void run(uint8_t value);    //Run to target position
        void pos1();            //Run to position 1
        void pos2();            //Run to position 2
        bool reachedTargetPos();  //Check if target position is reached
        uint8_t getPosTarg();       //Get target position
        uint8_t getMin();          //Get minimum angle
        uint8_t getMax();          //Get maximum angle
        uint8_t getPos1();         //Get position 1
        uint8_t getPos2();         //Get position 2
        void printData();    //Get servo data
        ~CustomServo();          //Destructor
};

#endif


