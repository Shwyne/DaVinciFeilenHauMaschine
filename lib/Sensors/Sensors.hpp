#pragma once

/*
* This is the Sensors Library for reading sensors with an arduino microcontroller
* It contains classes for the HallSwitch, Endstops and an RBG-Button
*/

#include <Arduino.h>

namespace Sensor {

//*----------------------- HallSwitch -----------------------*//

//Class for the HallSwitch
class HallSwitch {

private:
  uint8_t pin;  //Pin for the HallSwitch
  bool detectsMF; //True, if Magnetic Field is detected
  bool MagnetSignal; //Defines, if the HallSwitch detects a Magnetic Field while being LOW or HIGH

public:
  HallSwitch(uint8_t pin, bool MAgnetSignal = HIGH); //Constructor: Pin is required, triggeredIf is optional
  bool read();  //Read the HallSwitch and returns the state
  bool changed(); //Check if the state of the HallSwitch has changed, returns true if changed

};


//*----------------------- Endstops -----------------------*//

//Class for the Endstops
class Endstops {

private:
  uint8_t pin1;   //Pin for Endstop 1
  uint8_t pin2;   //Pin for Endstop 2
  uint8_t state;  //State of the Endstops (0 = no endstop triggered, 1 = endstop_1 triggered, 2 = endstop_2 triggered, 3 = both endstops triggered)

public:
  Endstops(uint8_t pin1, uint8_t pin2); //Constructor: Pin1 and Pin2 are required
  uint8_t read(); //Read the Endstops and returns the state
  bool changed(); //Check if the state of the Endstops has changed, returns true if changed

};


//*----------------------- RGB-Button -----------------------*//

//Class for a Button with an RGB-LED (Can be used as a normal Button as well, if Rpin, Gpin and Bpin are not defined)
class Button {

private:
  uint8_t Butpin; //Pin for the Button
  uint8_t Rpin;   //Pin for the Red LED
  uint8_t Gpin;   //Pin for the Green LED
  uint8_t Bpin;   //Pin for the Blue LED
  bool isPressed; //State of the Button (Triggered = pressed, not triggered = not pressed)

public:
  Button(uint8_t Butpin, uint8_t Rpin = 255, uint8_t Gpin = 255, uint8_t Bpin = 255); //Constructor: Butpin is required, Rpin, Gpin and Bpin are optional
  bool read();  //Read the Button and returns the state
  bool changed(); //Check if the state of the Button has changed, returns true if changed
  void waitForPress();  //Wait for the Button to be pressed, blocks the programm!
  void updateLED(uint8_t mode); //Update the LED of the Button (0 = off, 1 = red, 2 = green, 3 = blue, 4 = yellow, 5 = cyan, 6 = magenta, 7 = white)
  void updateLED(uint8_t red, uint8_t green, uint8_t blue); //Update the LED of the Button with custom RGB values, needs PWM pins
  
};

}
