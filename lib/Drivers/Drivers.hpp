#pragma once
#include <Arduino.h>
#include <AccelStepper.h>

//*----------------------- MP6550 -----------------------*//

//Class for the MP6550 Motor-Driver
class MP6550 {

private:
  int speed;  //Speed of the Motor
  uint8_t IN1pin;   //Pin for IN1
  uint8_t IN2pin;   //Pin for IN2
  uint8_t SLPpin;   //Pin for Sleep (default = 255 -> Disables Sleep Functions)
  bool autoSleep;   //Auto-Sleep - Setting (default = false)
  bool reversed;    //Reversed - Setting (default = false)

public:
  //Constructor - required: IN1, IN2, optional: SLP, reversed, autoSleep
  // IN1, IN2, SPL = uint8_t = Pins
  // reversed, autoSleep = bool = Settings
  MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin = 255, bool reversed = false, bool autoSleep = false);
  
  void run(int speed);  //Run the Motor with a specific speed (-255 to 255)
  void brake();         //Brake the Motor
  void coast();         //Coast the Motor


  int getSpeed();       //Get the current speed of the Motor and returns the value (-255 to 255)
  bool isSleeping();    //Get the current sleepState of the Motor and returns true if sleeping, false if not

  void sleep();         //Put the Motor to Sleep, if SLPpin is set
  void wake();          //Wake the Motor up, if SLPpin is set

};

//*----------------------- Servo Expansion -----------------------*//

#include <Servo.h>    //Include the Servo-Library for basic Servo-Functions

//Derived Class from Servo adding some additional functions
class ServoExp : public Servo {

private:
  uint8_t MIN;    //Minimum Angle possible for the Servo (default = 0)
  uint8_t MAX;    //Maximum Angle possible for the Servo (default = 180)
  uint8_t pin;      //Pin for the Servo-PWM (typ. Orange Wire)
  uint8_t pos1;     //Position 1 for the Servo (default = 255 -> OFF)
  uint8_t pos2;     //Position 2 for the Servo (default = 255 -> OFF)
  uint32_t blockingTime;  //Blocking Time for the Servo in ms (default = 500 ms)

public:
  //Constructor - required: pin, optional: min, max, blockingTime
  // pin = uint8_t = Pin for the Servo
  // min, max = uint8_t = Minimum and Maximum Angle for the Servo
  // blockingTime = uint32_t = Blocking Time for the Servo in ms
  ServoExp(uint8_t pin, uint8_t min = 0, uint8_t max = 180, uint32_t blockingTime = 500);
  
  void attach();  //Attach the Servo (Needed for the Servo-Library to work properly)

  void setPositions(uint8_t pos1 = 255, uint8_t pos2 = 255);  //Set the Positions for the Servo (pos1 = OFF, pos2 = ON)

  void write(uint8_t angle);  //Write a specific angle to the Servo (angle in degrees)
  void runToPos(uint8_t pos); //Run the Servo to a specific position (pos1 = OFF, pos2 = ON) -> blocks until reachedTarget()
  
  ~ServoExp();  //Destructor: Detach the Servo
};

//*----------------------- Stepper Expansion -----------------------*//

//Derived Class from AccelStepper adding some additional functions
class StepperExp : public AccelStepper {
  private:
    uint8_t M0pin;    //Pin for M1
    uint8_t M1pin;    //Pin for M2
    uint8_t M2pin;    //Pin for M3
    uint8_t SLPpin = 255;   //Pin for Sleep (default = 255 -> Disables Sleep Functions)
    uint8_t microSteps = 1;   //Microstep Mode
    uint32_t StepsPerRev = 200;  //Steps per Revolution
    uint32_t StepsPerPos = 66;  //Steps per Position
    uint32_t currentPos = 0;  //Current Position
    bool autoSleep;  //Auto-Sleep Mode

    uint8_t log2(uint8_t n);  //Calculate the Log2 of a number
  public:
    //Constructor calls constructor of a AccelStepper
    StepperExp(uint8_t stepPin, uint8_t dirPin) : AccelStepper(AccelStepper::DRIVER, stepPin, dirPin) {}
    void setupMS(uint8_t M0pin, uint8_t M1pin, uint8_t M2pin);  //Set the Microsteps for the Stepper
    void setMS(uint8_t microSteps);  //Set the Microsteps for the Stepper
    void setPos(uint16_t steps, uint8_t angle);  //Set the Position for the Stepper (steps = Steps per Revolution, angle = Angle till the next surface of the sign in degrees (°)
    void setSleep(uint8_t SLPpin, bool autoSleep = false);  //Set the Sleep Mode for the Stepper (autoSleep = true/false
    void zeroPos(); //Set the Position to 0
    uint8_t getMS();  //Get the Microsteps of the Stepper
    void nextPos();  //Go to the next position
    void prevPos();  //Go to the previous position
    void sleep();  //Put the Stepper to Sleep
    void wake();  //Wake the Stepper up
};