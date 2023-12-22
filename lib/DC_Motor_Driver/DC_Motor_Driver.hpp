#pragma once
#include <Arduino.h>

namespace DC_Motor_Driver {

/*/
/// This library is for controlling the MP6550 Motor Driver
/// In future applications it might be helpful to extend the library
/// Different drivers might work on another way and therefore need different
functions
///	For extending, just create a new class with a fitting name and add
needed functions and attributes
///
/*/

enum class Status : bool {
  Standby,
  Active
}; // Enum for Driver-State if a Standby-Mode is available

/**
 * @class MP6550
 * @brief This class is for controlling the MP6550 Motor Driver.
 *
 * It's a simplified class missing the VISEN pin thus not being able to read the
 * current draw of the motor. If you don't provide a SLP pin, you have to pull
 * the SLP pin to HIGH on another way. Otherwise the driver will stay disabled.
 * If a positive speed value should result in a certain direction, it might be
 * helpful to switch IN1 and IN2 while object creation.
 */

class MP6550 {
public:
  /**
   * @brief Constructor for MP6550 class.
   * @param IN1pin Pin connected to IN1.
   * @param IN2pin Pin connected to IN2.
   * @param SLPpin Pin connected to SLP.
   */
  MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin = 255);

  /**
   * @brief Runs the MP6550 with the given speed.
   * @param speed Speed value (-255 to 255).
   */
  void run(int speed);

  /**
   * @brief Brakes the motor.
   */
  void brake();

  /**
   * @brief Brakes the motor with a given force.
   * @param brakeForce Force of the brake (0 to 255).
   */
  void brake(int brakeForce);

  /**
   * @brief Coasts the motor.
   */
  void coast();

  /**
   * @brief Gets the speed of the MP6550.
   * @return The speed of the MP6550.
   */
  int getSpeed();

  /**
   * @brief Gets the sleep state of the MP6550.
   * @return The sleep state of the MP6550 (Status mentioned standby_).
   */
  Status sleepState();

  /**
   * @brief Puts the driver to sleep.
   */
  void disable();

  /**
   * @brief Enables the driver.
   */
  void enable();

  /**
   * @brief Prints all data of the MP6550.
   */
  void printData();

  /**
   * @brief Destructor for MP6550 class.
   * Brakes first and then enables SLP.
   */
  ~MP6550();

private:
  //*Arguments for MP6550control:
  int speed_; // Integer for pwm-speed (255 = 0 speed | 0 = max Speed)

  //*Arguments for Control Pins:
  uint8_t IN1; // Pin connected to IN1
  uint8_t IN2; // Pin connected to IN2
  uint8_t SLP; // Pin connected to SLP
};

} // namespace DC_Motor_Driver
