/**
 * @file ServoExp.hpp
 * @brief Header file for the ServoExp class.
 */

#pragma once

#include <Arduino.h>
#include <Servo.h>


/**
 * @class ServoExp
 * @brief A class that extends the Servo class to provide additional
 * functionality for controlling a servo motor.
 */
class ServoExp : public Servo {
private:
  uint8_t MIN_;       /**< Minimum angle */
  uint8_t MAX_;       /**< Maximum angle */
  uint8_t posTarg_;   /**< Target position */
  uint8_t tolerance_; /**< Tolerance for target position */

  uint8_t pin_; /**< Control Pin */
public:
  /**
   * @brief Constructor for the ServoExp class.
   * @param pin The control pin for the servo motor.
   * @param min The minimum angle of the servo motor (default: 0).
   * @param max The maximum angle of the servo motor (default: 180).
   */
  ServoExp(uint8_t pin, uint8_t min = 0, uint8_t max = 180);

  /**
   * @brief Attaches the servo motor to the control pin.
   */
  void attach();

  /**
   * @brief Runs the servo motor to the target position.
   * @param angle The target position in degrees.
   */
  void run(uint8_t angle);

  /**
   * @brief Sets the tolerance for the target position.
   * @param tolerance The tolerance value in degrees.
   */
  void setTolerance(uint8_t tolerance);

  /**
   * @brief Checks if the target position is reached within the tolerance range.
   * @return True if the target position is reached, false otherwise.
   */
  bool reachedTarget();

  /**
   * @brief Prints the servo data (minimum angle, maximum angle, target
   * position, tolerance).
   */
  void printData();

  /**
   * @brief Destructor for the ServoExp class.
   */
  ~ServoExp();
};
