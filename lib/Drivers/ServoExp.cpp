#include "Drivers.hpp"

/* 
 * Expanding the Servo class to include a tolerance for the target position
 * and the ability to set two positions for the servo to move to.
 * 
 * The tolerance is used to determine if the servo has reached the target position.
 * 
 * The two positions are used to move the servo to a specific position.
 * 
 * The ServoExp class is a child class of the Servo class.
*/

// Constructor for ServoExp class (only pin is required, rest is optional)
ServoExp::ServoExp(uint8_t pin, uint8_t min, uint8_t max, uint8_t tolerance) {
  this->MIN = min;    // Minimum angle
  this->MAX = max;    // Maximum angle
  this->tolerance = tolerance;    // Tolerance for target position
  this->pin = pin;    // Pin number (PWM-Control for Servo, typ. orange cable)
  this->pos1 = 255;     // Position 1 (default: 255 = not defined)
  this->pos2 = 255;   // Position 2 (default: 255 = not defined)
  return;
}

// Attach servo to pin
void ServoExp::attach() {
  Servo::attach(pin);   // Call attach function from Servo parent class
  this->posTarg = read();  // Set target position to current position as a starting point
                          // So the servo is at his target, whenever its attached, no matter where it is.
  return;
}

// Set the two positions for the servo to move to with runToPos(uint8_t pos)
void ServoExp::setPositions(uint8_t pos1, uint8_t pos2) { //Input: Pos1, Pos2 (0-255)
  this->pos1 = constrain(pos1, this->MIN, this->MAX);     // pos1 and pos 2 are constrained
  this->pos2 = constrain(pos2, this->MIN, this->MAX);     // to the min and max angle set in the constructor
  return;
}

// Set the tolerance for the target position
void ServoExp::setTolerance(uint8_t tolerance) {  //Input: Tolerance (0-255)
  this->tolerance = constrain(tolerance, 0, abs(this->MAX - this->MIN) / 2);  // Tolerance is constrained to 0 and half of the angle range
  return;
}

// Write the angle to the servo
void ServoExp::write(uint8_t angle){  //Input: Angle (0-255)
  this->posTarg = constrain(angle, this->MIN, this->MAX); // Target position is constrained to the min and max angle set in the constructor
  Servo::write(posTarg);  // Call write function from Servo parent class
  return;
}

// Same as write, but with a different name. Write and run are both common names for this kind of function
void ServoExp::run(uint8_t angle) {
  this->write(angle); // Call write function from ServoExp class
  return;
}

// Move the servo to a specific position
void ServoExp::runToPos(uint8_t pos) {  //Input: Pos (0-255), only 0 or 1 will move the servo
  if(pos == 0 && pos1 != 255){  // If pos is 0 -> pos1, AND pos1 is defined (not 255)
    this->write(pos1);  // Move to pos1
  }
  else if(pos == 1 && pos2 != 255){ // If pos is 1 -> pos2, AND pos2 is defined (not 255)
    this->write(pos2);  // Move to pos2
  }
  return;
}

// Check if the servo has reached the target position
bool ServoExp::reachedTarget() {
  if(abs(read() - posTarg) <= tolerance){ // If the difference between the current position and the target position is less than or equal to the tolerance
    return true;  // Return true
  }
  else{
    return false; // Return false
  }
}

// Destructor for ServoExp class
ServoExp::~ServoExp() {
  Servo::detach();  // Call detach function from Servo parent class
  return;
}
