#pragma once

#include <Arduino.h>

namespace Sensor {

class HallSwitch {
private:
  uint8_t pin;
  bool triggered;
  bool triggeredIf;
public:
  HallSwitch(uint8_t pin, bool triggeredIf = HIGH);
  bool read();
  bool changed();
  void printData();
};

class Endstops {
private:
  uint8_t pin1;
  uint8_t pin2;
  uint8_t state;
public:
  Endstops(uint8_t pin1, uint8_t pin2);
  uint8_t read();
  bool changed();
  void printData();
};

class Button {
private:
  uint8_t Butpin;
  uint8_t Rpin;
  uint8_t Gpin;
  uint8_t Bpin;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  bool triggered;
public:
  Button(uint8_t Butpin, uint8_t Rpin, uint8_t Gpin, uint8_t Bpin);
  bool read();
  bool changed();
  void waitForPress();
  void updateLED(uint8_t mode);
  void updateLED(uint8_t red, uint8_t green, uint8_t blue);
  void printData();
};

}
