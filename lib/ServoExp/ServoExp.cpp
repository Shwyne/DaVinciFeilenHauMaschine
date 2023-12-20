#include "ServoExp.hpp"

ServoExp::ServoExp(uint8_t pin, uint8_t min, uint8_t max) {
    
    MIN_ = min;
    MAX_ = max;
    tolerance_ = 0;
    pin_ = pin;
}

void ServoExp::attach() {

    Servo::attach(pin_); //Attach servo to pin
    posTarg_ = read(); //Set target position to current position
}

void ServoExp::run(uint8_t angle) {

    posTarg_ = constrain(angle, MIN_, MAX_); //Check if angle is in range and if not, corrects it
    Servo::write(posTarg_);  //Write target position to servo
}

void ServoExp::setTolerance(uint8_t tolerance) {
    // Set tolerance for target position. If tolerance is 0, target position must be reached exactly.
    // Maximum is half of the angle range.
    tolerance_ = constrain(tolerance, 0, abs(MAX_ - MIN_)/2);
}

bool ServoExp::reachedTarget() {

    //Check if target position is reached (with tolerance)
    return (abs(read() - posTarg_) <= tolerance_);
    
}

void ServoExp::printData() {
    
    Serial.println("Servo-Data:");
    Serial.println("\n----------------------");
    Serial.print(" Control-Pin: ");
    Serial.println(pin_);
    Serial.print(" Current Angle: ");
    Serial.print(read());
    Serial.println("°");
    Serial.print(" Target Angle: ");
    Serial.print(posTarg_);
    Serial.println("°");
    Serial.print(" Tolerance: ");
    Serial.print(tolerance_);
    Serial.println("°");
    if(reachedTarget() == true){
        Serial.println(" Target Position reached.");
    }
    else{
        Serial.println(" Target Position not reached.");
    }
    Serial.print(" Minimum Angle: ");
    Serial.print(MIN_);
    Serial.println("°");
    Serial.print(" Maximum Angle: ");
    Serial.print(MAX_);
    Serial.println("°");
}

ServoExp::~ServoExp() {
    Servo::detach();
}