#include "Servo_Expanded.hpp"


// Constructor
ServoExpanded::ServoExpanded(int pin) {
    pin_ = pin;
    pinMode(pin_, OUTPUT);
    srv_.attach(pin_);
}

ServoExpanded::ServoExpanded(int pin, int min_angle, int max_angle) {
    pin_ = pin;
    min_angle_ = min_angle;
    max_angle_ = max_angle;
    pinMode(pin_, OUTPUT);
    srv_.attach(pin_, min_angle_, max_angle_);
}

void ServoExpanded::write(int angle) {
    srv_.write(angle);
}

void ServoExpanded::setMaxAngle(int angle) {
    srv_.detach();
    max_angle_ = angle;
    srv_.attach(pin_, min_angle_, max_angle_);
}

void ServoExpanded::setMinAngle(int angle) {
    srv_.detach();
    min_angle_ = angle;
    srv_.attach(pin_, min_angle_, max_angle_);
}

int ServoExpanded::getAngle() {
    return srv_.read();
}

int ServoExpanded::getMaxAngle() {
    return max_angle_;
}

int ServoExpanded::getMinAngle() {
    return min_angle_;
}

ServoExpanded::~ServoExpanded() {
    srv_.detach();
}
