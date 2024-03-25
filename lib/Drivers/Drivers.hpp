#include <Arduino.h>

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
  //Constructor: IN1pin, IN2pin are required, SLPpin, reversed and autoSleep are optional
  MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin = 255, bool reversed = false, bool autoSleep = false);
  
  void run(int speed);  //Run the Motor with a specific speed (-255 to 255)
  void brake();         //Brake the Motor
  void coast();         //Coast the Motor


  int getSpeed();       //Get the current speed of the Motor and returns the value
  bool isSleeping();    //Get the current sleepState of the Motor and returns true if sleeping, false if not

  void sleep();         //Put the Motor to Sleep
  void wake();          //Wake the Motor up

};

//*----------------------- Servo Expansion -----------------------*//

#include <Servo.h>    //Include the Servo-Library for basic Servo-Functions

//Derived Class from Servo adding some additional functions
class ServoExp : public Servo {

private:
  uint8_t MIN;    //Minimum Angle possible for the Servo
  uint8_t MAX;    //Maximum Angle possible for the Servo
  uint8_t posTarg;    //Target Position for the Servo
  uint8_t tolerance;  //Tolerance for the Servo -> bool reachedTarget()
  uint8_t pin;      //Pin for the Servo-PWM (typ. Orange Wire)
  uint8_t pos1;     //Position 1 for the Servo
  uint8_t pos2;     //Position 2 for the Servo

public:
  //Constructor: pin, min, max, tolerance are optional
  ServoExp(uint8_t pin, uint8_t min = 0, uint8_t max = 180, uint8_t tolerance = 0);
  
  void attach();  //Attach the Servo (Needed for the Servo-Library to work properly)

  void setPositions(uint8_t pos1 = 255, uint8_t pos2 = 255);  //Set the Positions for the Servo (pos1 = OFF, pos2 = ON)
  void setTolerance(uint8_t tolerance); //Set the Tolerance for the Servo -> bool reachedTarget()

  void write(uint8_t angle);  //Write a specific angle to the Servo
  void run(uint8_t angle);    //Run the Servo to a specific angle (Does same as write, just a different name)
  void runToPos(uint8_t pos); //Run the Servo to a specific position (pos1 = OFF, pos2 = ON) -> blocks until reachedTarget()
  
  bool reachedTarget();  //Check if the Servo reached the Target Position -> returns true if reached, false if not
  
  ~ServoExp();  //Destructor: Detach the Servo
};