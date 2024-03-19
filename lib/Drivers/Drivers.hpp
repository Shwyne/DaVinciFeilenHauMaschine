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
  void coast();
  void brake();

  int getSpeed();
  Status sleepState();

  void sleep();
  void wake();

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