/**
 * @file ServoExp.cpp
 * @brief Implementation file for the ServoExp library.
 * 
 * The ServoExp library provides a class for controlling a servo motor expanding the functionality of the Servo.h library.
 * It allows setting the minimum and maximum angle, blocking time, and two positions for the servo to move to.
 * The library also provides functions to attach the servo to a pin, write the angle to the servo, and move the servo to a specific position, like the original Servo.h library.
 * 
 * @author [Shwyne]
 * 
 */
#include "Drivers.hpp"

// Constructor for ServoExp class (only pin is required, rest is optional)
ServoExp::ServoExp(uint8_t pin, uint8_t min, uint8_t max, uint32_t blockingTime) {
  
  this->MIN = min;    // Minimum angle
  this->MAX = max;    // Maximum angle
  this->blockingTime = blockingTime;    // Tolerance for target position
  this->pin = pin;    // Pin number (PWM-Control for Servo, typ. orange cable)
  this->pos1 = 255;     // Position 1 (default: 255 = not defined)
  this->pos2 = 255;   // Position 2 (default: 255 = not defined)

  return;
}

// Attach servo to pin
void ServoExp::attach() {

  Servo::attach(pin);   // Call attach function from Servo parent class
  
  return;
}

// Set the two positions for the servo to move to with runToPos(uint8_t pos)
void ServoExp::setPositions(uint8_t pos1, uint8_t pos2) { //Input: Pos1, Pos2 (0-255)
  
  this->pos1 = constrain(pos1, this->MIN, this->MAX);     // pos1 and pos 2 are constrained
  this->pos2 = constrain(pos2, this->MIN, this->MAX);     // to the min and max angle set in the constructor
  return;
}

// Write the angle to the servo
void ServoExp::write(uint8_t angle){  //Input: Angle (0-255)
  
  Servo::write(angle);  // Call write function from Servo parent class
  delay(this->blockingTime);  // Delay for blocking time (tolerance for target position
  
  return;
}

// Move the servo to a specific position
void ServoExp::runToPos(uint8_t pos) {  
  if(pos == 0 && pos1 != 255){  // If pos is 0 -> pos1, AND pos1 is defined (not 255)
    this->write(pos1);  // Move to pos1
  }
  else if(pos == 1 && pos2 != 255){ // If pos is 1 -> pos2, AND pos2 is defined (not 255)
    this->write(pos2);  // Move to pos2
  }
  return;
}

// Destructor for ServoExp class
ServoExp::~ServoExp() {
  
  Servo::detach();  // Call detach function from Servo parent class
  
  return;
}
