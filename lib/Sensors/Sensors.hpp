#ifndef Sensors_hpp
#define Sensors_hpp

#include <Arduino.h>
#include <math.h>

class Hall {
    private:
        uint8_t pin_;
        bool TriggerState_;
    public:
        Hall(uint8_t pin);
        Hall(uint8_t pin, bool TriggerState);
        bool read();
        void waitUntil(bool state);
        void waitUntil(bool state, int rate);
        void printData();
        ~Hall();
};

class Endstop {
    private:
        uint8_t pin1_;
        uint8_t pin2_;
        bool mode_;      //0 = Pullup, 1 = Pulldown
    public:
        Endstop(uint8_t pin1,uint8_t pin2);
        Endstop(uint8_t pin1,uint8_t pin2, bool mode);
        uint8_t read();
        void waitUntil(uint8_t state);
        void waitUntil(uint8_t state, int rate);
        void printData();
        ~Endstop();
};

#endif