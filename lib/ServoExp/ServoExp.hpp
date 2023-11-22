#ifndef CustomServo_hpp
#define CustomServo_hpp

#include <Servo.h>
#include <Arduino.h>

class ServoExp : public Servo {
    private:
        uint8_t MIN_;           //Minimum angle
        uint8_t MAX_;           //Maximum angle
        uint8_t pos1_;          //Position 1
        uint8_t pos2_;          //Position 2
        uint8_t posTarg_;       //Target position
        uint8_t pin_;          //Control Pin
    public:
        ServoExp(uint8_t pin, uint8_t min = 0, uint8_t max = 180);//Constructor
        void setPosData(uint8_t pos1, uint8_t pos2);
        void attach();   //Attach servo to pin
        void run(uint8_t angle);    //Run to target position
        void pos1();            //Run to position 1
        void pos2();            //Run to position 2
        bool reachedTarget();  //Check if target position is reached
        uint8_t getPosTarg();       //Get target position
        void printData();    //Get servo data
        ~ServoExp();          //Destructor
};

#endif


