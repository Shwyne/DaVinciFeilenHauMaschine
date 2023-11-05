#ifndef Sensors_hpp
#define Sensors_hpp

#include <Arduino.h>

class Hall {
    private:
        int pin_;
        bool state_;
    public:
        Hall(int pin);
        bool getState();
        ~Hall();
};

class Endstop {
    private:
        int pin1_;
        int pin2_;
        bool state_;
        int mode_;      //0 = Pullup, 1 = Pulldown
    public:
        Endstop(int pin1,int pin2, int mode);
        int getState();
        ~Endstop();
};

#endif