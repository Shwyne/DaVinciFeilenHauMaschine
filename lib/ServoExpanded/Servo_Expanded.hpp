#ifndef Servo_Expanded_hpp
#define Servo_Expanded_hpp

#include <Servo.h>
#include <Arduino.h>

class ServoExpanded {
private:
    int max_angle_;
    int min_angle_;
    int pin_;
    Servo srv_;

public:
    ServoExpanded(int pin);
    ServoExpanded(int pin, int min_angle, int max_angle);
    void write(int angle);
    void setMaxAngle(int angle);
    void setMinAngle(int angle);
    int getAngle();
    int getMaxAngle();
    int getMinAngle();
    ~ServoExpanded();
};

#endif
