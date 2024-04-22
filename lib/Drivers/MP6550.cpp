#include "Drivers.hpp"

/* Table for Running this driver:

* This library is for the MP6550 H-Bridge Motor Driver
* It is used to control the speed and direction of a motor using two PWM signals and a sleep pin
* Contains functions to run the motor, brake the motor, coast the motor, and put the driver to sleep
* Created by: Shwyne, 2023

* The MP6550 is a dual H-Bridge motor driver with two PWM inputs and a sleep pin
* SLP pin has to be pulled HIGH (5V) if not controlled by a microcontroller
* IN1 and IN2 are used to control the direction of the motor:

1. Input Logic Truth Table:
  IN1  |  IN2  | OUT1  | OUT2  | Function
----------------------------------------------
   L   |   L   |   Z   |   Z   | Coast
   L   |   H   |   L   |   H   | Reverse
   H   |   L   |   H   |   L   | Forward
   H   |   H   |   L   |   L   | Brake

  Z = High Impedance
  L = Low
  H = High

2. PWM Input Control
  IN1  |  IN2  |  OUT1  |  OUT2  | Function
----------------------------------------------
   H   | PWM L |   H    |   L    | Forward
   H   | PWM H |   L    |   L    | Slow Decay
 PWM L |   H   |   L    |   H    | Reverse
 PWM H |   H   |   L    |   L    | Slow Decay

  PWM = Pulse Width Modulation: 0-255

3. Sleep Mode
  SLP = LOW: Standby
  SLP = HIGH: Active

*/

//* Constructor for MP6550 class (Only IN1 and IN2 are required, rest is optional)
MP6550::MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin, bool reversed, bool autoSleep) {
  this->IN1pin = IN1pin;  // Pin for IN1
  this->IN2pin = IN2pin;  // Pin for IN2
  this->SLPpin = SLPpin;  // Pin for SLP (default: 255)
  this->autoSleep = autoSleep;  // Auto sleep mode (default: false)
  this->reversed = reversed;    // Reversed motor direction (default: false)
  this->speed = 0;          // Speed starts at 0

  pinMode(this->IN1pin, OUTPUT);  // Set IN1pin as output
  pinMode(this->IN2pin, OUTPUT);  // Set IN2pin as output
  digitalWrite(this->IN1pin, LOW);  // Set IN1pin to LOW 
  digitalWrite(this->IN2pin, LOW);  // Set IN2pin to LOW (-> Idle state)

  // If SLPpin is used, set it as output and set it to HIGH -> Active
  if (this->SLPpin != 255) {
    pinMode(this->SLPpin, OUTPUT);
    digitalWrite(this->SLPpin, HIGH);
  }
  return;
}

//* Run the motor with a specific speed
void MP6550::run(int speed) { //Input: Speed (-255 to 255)
  
  // Checks if driver in sleep mode (SLP == LOW) and if SLP is used (SLP != 255)
  if(SLPpin != 255 && isSleeping() == true) {
    this->wake();
  }

  // constraints input speed to -255 to 255, because of 8bit PWM control
  this->speed = constrain(speed, -255, 255);

  // If speed is positive and not reversed or speed is negative and reversed
  if ((this->speed > 0 && !reversed) || (this->speed < 0 && reversed)) {
    analogWrite(IN1pin, abs(this->speed));
    digitalWrite(IN2pin, LOW);
    Serial.println("IN1 - PWM | IN2 - LOW");
  } 
  
  //if speed is negative and not reversed or speed is positive and reversed
  else if ((this->speed < 0 && !reversed) || (this->speed > 0 && reversed)) {
    digitalWrite(IN1pin, LOW);
    analogWrite(IN2pin, abs(this->speed));
  } 
  
  //if speed is 0
  else {
    digitalWrite(IN1pin, LOW);
    digitalWrite(IN2pin, LOW);
  }

  return;
}

//* Brake the motor
void MP6550::brake() {
  //Active braking
  digitalWrite(IN1pin, HIGH); // Set IN1pin to HIGH
  digitalWrite(IN2pin, HIGH); // Set IN2pin to HIGH -> leads to braking

  speed = 0;  // Speed is 0
  delay(500); // Delay to ensure the motor is stopped

  //Idle state
  digitalWrite(IN1pin, LOW);  // Set IN1pin to LOW
  digitalWrite(IN2pin, LOW);  // Set IN2pin to LOW -> Idle state

  //Autosleep mode (If autoSleep is true, put driver to sleep)
  if(autoSleep) { 
    this->sleep();
  }
  return;
}

//* Coast the motor
void MP6550::coast() {
  //Idle state
  digitalWrite(IN1pin, LOW);  // Set IN1pin to LOW
  digitalWrite(IN2pin, LOW);  // Set IN2pin to LOW -> Idle state

  speed = 0;  // Speed is 0

  //Autosleep mode (If autoSleep is true, put driver to sleep)
  if(autoSleep) {
    this->sleep();
  }
  return;
}

//* Put the driver to sleep
void MP6550::sleep() {
  // If SLPpin is not used, return
  if(SLPpin == 255) return;  
  // If speed is not 0, brake the motor. Ensures braking is necessary before sleep
  if (speed != 0) {  
    this->brake();  // Brake the motor
  }
  // Set SLPpin to LOW -> Standby
  digitalWrite(SLPpin, LOW);
  return;
}

//* Wake the driver
void MP6550::wake() {
  // If SLPpin is not used, return
  if (SLPpin == 255) return; 
  // Set SLPpin to HIGH -> Active
  digitalWrite(SLPpin, HIGH); 
  return;
}

//* Get the current speed
int MP6550::getSpeed() {
   // Return speed as int (-255 to 255)
   return speed;  
}

//* Get the sleep state of the driver
bool MP6550::isSleeping() {
  // If SLPpin is not used, return false;
  if(SLPpin == 255) return false;
  // Return true if SLPpin is HIGH, false if LOW  
  return digitalRead(SLPpin) == LOW;  
}