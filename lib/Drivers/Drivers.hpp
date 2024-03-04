#pragma once

#include <Arduino.h>

enum class Status : uint8_t {
  Standby,
  Active
};

//*----------------------- MP6550 -----------------------*//

class MP6550 {

private:
  int speed;
  uint8_t IN1pin;
  uint8_t IN2pin;
  uint8_t SLPpin;
  bool autoSleep;
  bool reversed;

public:
  MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin = 255, bool reversed = false, bool autoSleep = false);
  
  void run(int speed);
  void brake();

  int getSpeed();
  Status sleepState();

  void sleep();
  void wake();
  
  ~MP6550();

};

//*----------------------- Servo Expansion -----------------------*//

#include <Servo.h>

class ServoExp : public Servo {

private:
  uint8_t MIN;
  uint8_t MAX;
  uint8_t posTarg;
  uint8_t tolerance;
  uint8_t pin;
  uint8_t pos1;
  uint8_t pos2;

public:
  ServoExp(uint8_t pin, uint8_t min = 0, uint8_t max = 180, uint8_t tolerance = 0);
  
  void attach();

  void setPositions(uint8_t pos1 = 255, uint8_t pos2 = 255);
  void setTolerance(uint8_t tolerance);

  void write(uint8_t angle);
  void run(uint8_t angle);
  void runToPos(uint8_t pos);
  
  bool reachedTarget();
  
  ~ServoExp();
};

//*----------------------- Step Expansion -----------------------*//

#include <DRV8825.h>
#include <A4988.h>
#include "Sensors.hpp"

class StepExp : public DRV8825 {

private:
  Sensor::HallSwitch hallSwitch;
  uint16_t position;
  bool Standby;

public:
  StepExp(uint8_t steps, uint8_t dir_pin, uint8_t step_pin, uint8_t enable_pin, uint8_t mode0_pin, uint8_t mode1_pin, uint8_t mode2_pin, uint8_t hall_pin);
  
  void begin(float rpm = 60, uint8_t microsteps = 1);
  
  void run(int steps);
  void home();

  void sleep();
  void wake();

  uint8_t getPosition();
  Status sleepState();

  ~StepExp() = default;
};