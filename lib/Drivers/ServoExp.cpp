#include "Drivers.hpp"

ServoExp::ServoExp(uint8_t pin, uint8_t min, uint8_t max, uint8_t tolerance) {
  this->MIN = min;
  this->MAX = max;
  this->tolerance = tolerance;
  this->pin = pin;
}

void ServoExp::attach() {
  Servo::attach(pin);
  posTarg = read();
}

void ServoExp::write(int angle){
  posTarg = constrain(angle, MIN, MAX);
  Servo::write(posTarg);
}

void ServoExp::run(uint8_t angle) {
  posTarg = constrain(angle, MIN, MAX);
  Servo::write(posTarg);
}

void ServoExp::setTolerance(uint8_t tolerance) {
  this->tolerance = constrain(tolerance, 0, abs(MAX - MIN) / 2);
}

bool ServoExp::reachedTarget() {
  if(abs(read() - posTarg) <= tolerance){
    return true;
  }
  else{
    return false;
  }
}

void ServoExp::printData() {
  Serial.println("Servo-Data:");
  Serial.print("Control-Pin: ");
  Serial.println(pin);
  Serial.print("Current Angle: ");
  Serial.print(read());
  Serial.println("°");
  Serial.print("Target Angle: ");
  Serial.print(posTarg);
  Serial.println("°");
  Serial.print("Tolerance: ");
  Serial.print(tolerance);
  Serial.println("°");
  if (reachedTarget() == true) {
    Serial.println(" Target Position reached.");
  } else {
    Serial.println(" Target Position not reached.");
  }
  Serial.print(" Min Angle: ");
  Serial.print(MIN);
  Serial.println("°");
  Serial.print(" Max Angle: ");
  Serial.print(MAX);
  Serial.println("°");
}

ServoExp::~ServoExp() {
  Servo::detach();
}
