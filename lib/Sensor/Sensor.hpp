#pragma once

#include <Arduino.h>

/**
 * @file Sensor.hpp
 * @brief Contains the declarations of various sensor classes.
 */

namespace Sensor {

/**
 * @class HallSwitch
 * @brief Represents a Hall switch sensor.
 */
class HallSwitch {
private:
  uint8_t pin_;    /**< The pin number to which the Hall switch is connected. */
  bool triggered_; /**< Attribute for storing the triggered State (LOW, HIGH) */
public:
  /**
   * @brief Constructs a HallSwitch object.
   * @param pin The pin number to which the Hall switch is connected.
   * @param triggeredIf The logic level at which the Hall switch is considered
   * triggered. Defaults to HIGH.
   */
  HallSwitch(uint8_t pin, bool triggeredIf = HIGH);

  /**
   * @brief Sets the logic level at which the Hall switch is considered
   * triggered.
   * @param triggeredIf The logic level at which the Hall switch is considered
   * triggered.
   */
  void setTriggeredIf(bool triggeredIf);

  /**
   * @brief Reads the current state of the Hall switch.
   * @return true if the Hall switch is triggered, false otherwise.
   */
  bool read();

  /**
   * @brief Prints the data of the Hall switch.
   */
  void printData();
};

/**
 * @class Endstops
 * @brief Represents a pair of endstops.
 */
class Endstops {
private:
  uint8_t pin1_;   /**< The first pin number to which first the endstop is
                      connected. */
  uint8_t pin2_;   /**< The second pin number to which the second endstop is
                      connected. */
  bool triggered_; /**< Attribute for storing the triggered State (LOW, HIGH) */
public:
  /**
   * @brief Constructs an Endstop object.
   * @param pin1 The first pin number to which the endstop is connected.
   * @param pin2 The second pin number to which the endstop is connected.
   * @param triggeredIf The logic level at which the endstop is considered
   * triggered. Defaults to LOW.
   */
  Endstops(uint8_t pin1, uint8_t pin2, bool triggeredIf = LOW);

  /**
   * @brief Reads the current state of the endstop.
   * @return 0 if no endstops are triggered, 1 if first Endstop is triggered,
   * and 2 if second Endstop is triggered.
   */
  int8_t read();

  /**
   * @brief Prints the data of the endstop.
   */
  void printData();
};

class Button {
private:
  uint8_t Butpin;
  uint8_t Rpin;
  uint8_t Gpin;
  uint8_t Bpin;
public:
  
  Button(uint8_t Butpin = 255, uint8_t Rpin = 255, uint8_t Gpin = 255, uint8_t Bpin = 255);

  bool read();

  void updateLED(uint8_t mode); //0 = off, 1 = red, 2 = green, 3 = blue, 4 = yellow, 5 = cyan, 6 = magenta, 7 = white
  void updateLED(uint8_t red, uint8_t green, uint8_t blue); //Needs PWM-Pins!

  void printData();
};

} // namespace Sensor
