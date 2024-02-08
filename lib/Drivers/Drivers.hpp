#pragma once

#include <Arduino.h>

enum class Status : uint8_t {
  Standby,
  Active
};

class MP6550 {
public:
  MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin = 255);
  void run(int speed);
  void brake();
  void brake(int brakeForce);
  void coast();
  int getSpeed();
  Status sleepState();
  void sleep();
  void wake();
  void printData();
  ~MP6550();

private:
  int speed_;
  uint8_t IN1;
  uint8_t IN2;
  uint8_t SLP;
};

#include <Servo.h>

class ServoExp : public Servo {
private:
  uint8_t MIN_;
  uint8_t MAX_;
  uint8_t posTarg_;
  uint8_t tolerance_;

  uint8_t pin_;
public:
  ServoExp(uint8_t pin, uint8_t min = 0, uint8_t max = 180);
  void write(int value);
  void attach();
  void run(uint8_t angle);
  void setTolerance(uint8_t tolerance);
  bool reachedTarget();
  void printData();

  ~ServoExp();
};

#include <DRV8825.h>
#include <A4988.h>