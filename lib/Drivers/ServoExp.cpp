#include "Drivers.hpp"

ServoExp::ServoExp(uint8_t pin, uint8_t min, uint8_t max, uint8_t tolerance) {
  this->MIN = min;
  this->MAX = max;
  this->tolerance = tolerance;
  this->pin = pin;
  this->pos1 = 0;
  this->pos2 = 180;
  return;
}

void ServoExp::attach() {
  Servo::attach(pin);
  this->posTarg = read();
  return;
}

void ServoExp::setPositions(uint8_t pos1, uint8_t pos2) {
  this->pos1 = constrain(pos1, this->MIN, this->MAX);
  this->pos2 = constrain(pos2, this->MIN, this->MAX);
  return;
}

void ServoExp::setTolerance(uint8_t tolerance) {
  this->tolerance = constrain(tolerance, 0, abs(this->MAX - this->MIN) / 2);
  return;
}

void ServoExp::write(uint8_t angle){
  this->posTarg = constrain(angle, this->MIN, this->MAX);
  Servo::write(posTarg);
  return;
}

void ServoExp::run(uint8_t angle) {
  this->write(angle);
  return;
}

void ServoExp::runToPos(uint8_t pos) {
  if(pos == 0 && pos1 != 255){
    this->write(pos1);
  }
  else if(pos == 1 && pos2 != 255){
    this->write(pos2);
  }
  return;
}

bool ServoExp::reachedTarget() {
  if(abs(read() - posTarg) <= tolerance){
    return true;
  }
  else{
    return false;
  }
}

ServoExp::~ServoExp() {
  Servo::detach();
  return;
}
