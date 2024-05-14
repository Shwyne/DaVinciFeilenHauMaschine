//#include "TestFunctions.hpp"    //All Functions includes all (custom) libraries and Arduino.h
#include "ExceptHandling.hpp"  //Error-Handling
#include "drivers.hpp"  //Drivers
#include "Sensors.hpp"  //Sensors
#include "config.hpp"   //Configurations
#include <AccelStepper.h>   //Stepper-Library

//*----------------- Variables ----------------------

uint32_t ctime = 0; //Common Timer, used for general and temporal Timeout-Calculations
uint32_t stime = 0; //Slider Timer, counts only, if Slider is moving and resets with full reset
uint32_t hwtime = 0; //Hammerwheel Timer, counts only, if Hammerwheel is moving
uint32_t wtime = 0; //Weight Timer for resetting manners

//*------------------ Components -----------------------

MP6550 SLdc(pin::SL_IN1, pin::SL_IN2, pin::SL_SLP, SL::REVERSED, SL::AUTO_SLEEP); // Motor Slider (In1, In2, Sleep)
MP6550 HWdc(pin::HW_IN1, pin::HW_IN2, pin::HW_SLP, HW::REVERSED, HW::AUTO_SLEEP); // Motor Hammerwheel (In1, In2, Sleep)
ServoExp HSsv(pin::HS_SRV, HS::MIN, HS::MAX, HS::BLOCKTIME); // Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp COsv(pin::CO_SRV, COUP::MIN, COUP::MAX, COUP::BLOCKTIME); // Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
AccelStepper SGst(AccelStepper::DRIVER, pin::STP_STP, pin::STP_DIR); // Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
Sensor::Endstops WGes(pin::WG_ES_T, pin::WG_ES_B); // Endschalter Weight (BottomPin, TopPin, TriggerState)
Sensor::Endstops SLes(pin::SL_ES_R, pin::SL_ES_L); // Endschalter Slider (LeftPin, RightPin, TriggerState)
Sensor::HallSwitch HWha(pin::HR_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Hammerwheel (Hallpin, TriggerState)
Sensor::HallSwitch SGha(pin::SG_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Sign (Hallpin, TriggerState)
Sensor::Button Go(pin::GO_BUT,pin::GO_LED_R,pin::GO_LED_G,pin::GO_LED_B); // Go-Button (Pin, TriggerState)

StatusClass MachineStatus = StatusClass(CompStatus::SUCCESS, FuncGroup::UNDEFINED);

//*-------------------- Math ---------------------------

//Calculates the logarithm to the base 2
//Used in the stepper-function to calculate the microsteps
namespace mathFunc {
    uint8_t log2(uint8_t n);
}

//*----------- Servo/Stepper Functions ----------------

//Servo Functions
namespace serv {
    StatusClass hammerstop();  //Stops the hammerwheel by engaging the hammerstop in the right position
    StatusClass hammergo();    //Releases the hammerstop
}

//Stepper Functions
namespace step {
    void setMicroSteps(uint8_t microSteps); //Sets the microsteps for the stepper (1-32);
    StatusClass home();                            //Homes the stepper (Hall-Sensor and Magnet -> Pos1)
    void move(int steps);                   //Moves the stepper a specific amount of steps
}

//*----------------- States -----------------------

//Initializes the Machine to the Start-Position
//Also used for resetting the machine after an error
void inline initStateOfMachine();
//Checks the state of the machine and handles errors, if enabled 
void inline checkState(StatusClass status);

//*----------------------- Setup -------------------------------------------------------------------------------

void setup() {
  bool skipInit = false;  //Used for skipping the initStateOfMachine of setup in case of eeprom-error (cuz after clearing, init will be called anyways)
  //*Serial-Setup:
  if(DEBUG>0) { //If Debugging is enabled, start the Serial Connection
  Serial.begin(SERIALSET::BAUTRATE); // Starts Serial Connection between Arduino and PC via USB -> Baudrate = Bits per second (Bps)
  Serial.println("Setup started."); 
  }
  //* Servo-Setup:
  HSsv.attach(); // Initializes Servo Hammerstop
  COsv.attach(); // Initializes Servo Kupplung
  
  //*EEPROM Error-Checking:
  pinMode(pin::CLEAR_ERROR, INPUT_PULLUP);  //Sets the Clear-Error-Button to Input-Pullup
  if(hasErrorEEPROM()){ //If an Error is stored in the EEPROM, read the Error and set the Machine to the Error-State
    if(DEBUG>0) Serial.println("EEPROM-Error detected.");
    MachineStatus = StatusClass(EEPROM.read(EEPROM_ADDRESS+1), EEPROM.read(EEPROM_ADDRESS+2));
    if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS) skipInit = true; //If Error-Management is enabled and the Error-Status is not Success, skip the initStateOfMachine
    checkState(MachineStatus);
  }

  //*Fan-Setup:
  //Initializes the Fan, if enabled (config)
  if(FAN){
    pinMode(pin::FAN, OUTPUT); //Initializes the fan, if enabled (config)
    digitalWrite(pin::FAN, LOW);  //Turns off the fan
  }

  //* Sign-Stepper Setup:
  SGst.setMaxSpeed(5000.0); // Sets the maximum speed of the stepper
  SGst.setAcceleration(1000.0); // Sets the acceleration of the stepper
  digitalWrite(pin::STP_RST, HIGH); // Disables Reset-Mode of the stepperdriver
  digitalWrite(pin::STP_SLP, LOW);  // Disables the sleep mode of the stepperdriver
  step::setMicroSteps(STP::MICRO_STEPS);  // Sets the microsteps of the stepper

  if(skipInit == false)initStateOfMachine(); //Initializes the Machine to the Home-Position
  
  if(DEBUG>0) Serial.println("Setup done.");  
}

//* ----------------- Loop --------------------------------------------------------------------------------

void loop() {

  //* ----------------- IDLE ---------------------------
  if(DEBUG>0) Serial.println("IDLE: Waiting for Go-Signal.");
  //*1. Sign to Pos 1:
  if(STP::ENABLED == true){  //If the sign is not at the home position and the stepper is enabled (config)
    MachineStatus = step::home();                           //Homes the stepper and returns the status -> Sign Pos 1 (Hall-Sensor, Magnet)
    checkState(MachineStatus);                          //Error-Handling                
  }
  //*2 Button LED to green:
  Go.updateLED(LED::GREEN);                              //Updates the LED to green, showing the visitor the machine is ready
  //*3. Waiting for User Input:
  Go.waitForPress();                                      //Waits for the Go-Button to be pressed and released
  if(DEBUG>0) Serial.println("IDLE: Go-Signal received.");


  //* ----------------- RUN ---------------------------

  //*1. (optional) start the fan
  if(FAN) digitalWrite(pin::FAN, HIGH); //Turns on the fan if enabled (config)
  //*2. Button LED to cyan:
  Go.updateLED(LED::CYAN);        //Updates the LED to cyan, showing the visitor the machine is running
  //*3. Stepper to Pos 2:
  if(STP::ENABLED) step::move(STP::POS); //Moves the stepper to the position (config) -> Sign pos 2
  //*4. Running the Motors:
  HWdc.run(HW::SPEED);             //Runs the Hammerwheel with the given speed (config)
  SLdc.run(SL::SPEED);              //Runs the Slider with the given speed (config)
  if(DEBUG>0) Serial.println("RUN: Motors started, waiting for endstops.");
  //*5. Setting the timers to the current time
  ctime = millis();                //Sets the current time to the common timer (ctime) for SL-Timeout (calculated by geometry) calculations
  hwtime = millis();             //Sets the current time to the hammerwheel-timer for HW-Timeout (Hall-Sensor and Magnets) calculations
  bool detectedMagnet = false;
  //*6. Waiting for ONE endstop to be triggered
  while(WGes.read() != Weight::BOTTOM && SLes.read() != Slider::LEFT){  //Is running till one endstop is reached (Bottom or Left)
    if(ERROR_MANAGEMENT){ //If Error-Management is enabled
      if(HWha.read() == HIGH && detectedMagnet == false){ //If the Hall-Sensor is triggered and no magnet was detected before
        detectedMagnet = true;  //Sets the detectedMagnet to true
        hwtime = millis();    //Sets the current time to the hammerwheel-timer
      } //This prevents the timer to be resetet, if the Hall-Sensor is still detecting a magnetic field.
      else if(HWha.read() == LOW && detectedMagnet == true){  //If the Hall-Sensor is not triggered and a magnet was detected before
        detectedMagnet = false; //Sets the detectedMagnet to false
      }
      if((millis() - hwtime) >= HW::TIMEOUT){ //Timeout occurs, if the time from the current run (millis() - hwtime) is greater than HW::TIMEOUT
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);  //Sets the MachineStatus to a Timeout-Error
        checkState(MachineStatus);  //Error-Handling
        return;
      }
      if((millis() - ctime + stime) >= SL::TIMEOUT){  //Timeout occurs, if the sum of time from when the slider started (stime) and the time from the current run (millis() - ctime) is greater than SL::TIMEOUT
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);  //Sets the MachineStatus to a Timeout-Error
        checkState(MachineStatus);  //Error-Handling
        return;
      }
    }
  }
  
  if(DEBUG>1) Serial.print("RUN: Hit Endstops, starting ");
  //*7. Updating Slider Timer:
  stime += millis()-ctime;  //Adding the time span till an endstop was hit to the slider-timer for the next runs. Is set to 0 if the slider is resetted.
  //*8. Braking the motors
  HWdc.brake(); //Brakes the Hammerwheel
  SLdc.brake(); //Brakes the Slider
  if(DEBUG>0) Serial.println("RUN: Endstops reached");


  //* ----------------- RESET ---------------------------
  
  //*1. update the LED to yellow
  Go.updateLED(LED::YELLOW);

  //*2. Stepper to Pos 3:
  if(STP::ENABLED) step::move(2*STP::POS); //Moves the stepper to the next position (config) -> Sign pos 3

  //*3. Decoupling the Slider and Hammer shafts
  COsv.write(COUP::DIS);
  //*4. Blocking the Hammerwheel
  MachineStatus = serv::hammerstop(); //Blocking the Hammerwheel by calling the hammerstop-function that returns a MachineStatus
  checkState(MachineStatus);  //Error-Handling
  
  if(DEBUG>0) Serial.println("RESET: Motors reversed, waiting for endstops.");
  //*5. Running either both or just the weight motor back (depending on the condition of the Slider Endstops)
  if(SLes.read() == Slider::LEFT){  //If the Slider is at the left endstop -> Reset the slider AND the weight
    SLdc.run(-SL::RS_SPEED);  //Runs the Slider back with the reset-speed
    HWdc.run(-HW::RS_SPEED);  //Runs the Hammerwheel back with the reset-speed
    if(DEBUG > 1) Serial.println("RESET: Endstops waiting...");
    bool ReachedWeightTarget = false; //Setting the reachedWeightTarget to false
    bool ReachedSliderTarget = false; //Setting the reachedSliderTarget to false
    //*1. Setting the timer to the current time
    ctime = millis(); //Sets the current time to the common timer (ctime) for SL-Timeout (calculated by geometry) and WG-Timeout (calculated by geometry) calculations
    //*2. Waiting for BOTH endstops to be triggered
    while((ReachedWeightTarget == false) || (ReachedSliderTarget == false)){  //Is running till BOTH endstops are reached (Top and Right)
      if(WGes.read() == Weight::TOP && ReachedWeightTarget == false){ //If the Weight-Endstop is reached and the Weight-Endstop is not reached before
        if(DEBUG>1) Serial.print("Weight reached TOP"); //Debug-Message
        ReachedWeightTarget = true; //Sets the reachedWeightTarget to true
        HWdc.brake();   //Reaching Weight top brakes the HW-Motor
      }
      if(SLes.read() == Slider::RIGHT && ReachedSliderTarget == false){ //If the Slider-Endstop is reached and the Slider-Endstop is not reached before
        if(DEBUG>1) Serial.print("Slider reached RIGHT"); //Debug-Message
        ReachedSliderTarget = true; //Sets the reachedSliderTarget to true
        SLdc.brake();   //Reaching Slider right brakes the SL-Motor
      }
      if(ERROR_MANAGEMENT){ //If Error-Management is enabled
        if(HW::wtime_calc != 0 && millis() - ctime >= HW::wtime_calc && ReachedWeightTarget == false){  //If the time from the current run (millis() - ctime) is greater than HW::wtime_calc and the Weight-Endstop is not reached before
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::WG);  //Sets the MachineStatus to a Timeout-Error
          checkState(MachineStatus);  //Error-Handling
        }
        if(SL::stime_calc != 0 && millis() - ctime >= SL::stime_calc && ReachedSliderTarget == false){  //If the time from the current run (millis() - ctime) is greater than SL::stime_calc and the Slider-Endstop is not reached before
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);  //Sets the MachineStatus to a Timeout-Error
          checkState(MachineStatus);  //Error-Handling
        }
      }
    }
    //*3. Resetting the slider-timer
    stime = 0;  //Resets the slider-timer, because the slider got resetted
  } 
  else{ //If the Slider is not at the left endstop -> Reset ONLY the weight
    HWdc.run(-HW::RS_SPEED);  //Runs the Hammerwheel back with the reset-speed
    if(DEBUG > 1) Serial.println("RESET: Endstops waiting..."); //Debug-Message
    //*1. Setting the timer to the current time
    ctime = millis(); //Sets the current time to the common timer (ctime) for WG-Timeout (calculated by geometry) calculations
    //*2. Waiting for the Weight-Endstop to be triggered
    while(WGes.read() != Weight::TOP){  //Is running till the Weight-Endstop is reached (Top)
      if(ERROR_MANAGEMENT){ //If Error-Management is enabled
        if(HW::wtime_calc != 0 && millis() - ctime >= HW::wtime_calc){  //If the time from the current run (millis() - ctime) is greater than HW::wtime_calc
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::WG);  //Sets the MachineStatus to a Timeout-Error
        checkState(MachineStatus);  //Error-Handling
        }
      }
  }
  //*3. Braking the motor
  HWdc.brake(); //Brakes the Hammerwheel
  } 

  if(DEBUG>0) Serial.println("RESET: Endstops reached.");
  //*6. Releasing the Hammerwheel
  MachineStatus = serv::hammergo(); //Releasing the Hammerwheel by calling the hammergo-function that returns a MachineStatus
  checkState(MachineStatus);  //Error-Handling
  //*7. Coupling the Slider and Hammer shafts
  COsv.write(COUP::EN); //Coupling the Slider and Hammer shafts
  delay(1000);  //Delay for the Servo to move to the engaged position
  //*8. Disabling the fan
  if(FAN) digitalWrite(pin::FAN, LOW); //Turns off the fan (if available)
  
  if(DEBUG>0) Serial.println("RESET: Done");

  //* ----------------- IDLE --------------------------- LOOP END
  if(DEBUG>1) Serial.println("-------------------------------");
}


//*----------------- States --------------------------------------------------------------------------------

//* INIT State of the Machine:
//This functions is called in the setup and whenever an error occurs. It initializes the machine to the start position.
void inline initStateOfMachine(){
  //*1. Update the LED to white
  Go.updateLED(LED::WHITE);
  //*2. (Optional) Wait for the Go-Button to be pressed (debugging purposes)
  if(DEBUG>0){
    Serial.println("Press Go-Button to initialize the machine.");
    Go.waitForPress();
  }
  
  //*3. Update the LED to yellow
  Go.updateLED(LED::YELLOW);
  if(DEBUG>0) Serial.print("INIT: Begin | ");
  COsv.write(COUP::DIS); // Sets the Coupling to the disenaged position
  HSsv.write(HS::OFF); // Sets the Hammerstop to the off position
  //*4. Checking Position (Slider, Weight), if not in Start position
  if(SLes.read() != Slider::RIGHT || WGes.read() != Weight::TOP){ //If one of the axis isnt at start position -> Reset
    COsv.write(COUP::DIS);  //Disengaging the Coupling
    //*4b Blocking the Hammerwheel
    MachineStatus = serv::hammerstop(); //Blocking the Hammerwheel by calling the hammerstop-function that returns a MachineStatus
    checkState(MachineStatus);  //Error-Handling
    //*4c Running the motors back to the start position
    SLdc.run(-SL::RS_SPEED);  //Running the Slider back with the reset-speed
    HWdc.run(-HW::RS_SPEED);  //Running the Hammerwheel back with the reset-speed
    if(DEBUG > 1) Serial.println("RESET: Endstops waiting..."); //Debug-Message
    bool ReachedWeightTarget = false; //Setting the reachedWeightTarget to false
    bool ReachedSliderTarget = false; //Setting the reachedSliderTarget to false
    //*1. Setting the timer to the current time
    ctime = millis(); //Sets the current time to the common timer (ctime) for SL-Timeout (calculated by geometry) and WG-Timeout (calculated by geometry) calculations
    //*2. Waiting for BOTH endstops to be triggered
    while((ReachedWeightTarget == false) || (ReachedSliderTarget == false)){  //Is running till BOTH endstops are reached (Top and Right)
      if(WGes.read() == Weight::TOP && ReachedWeightTarget == false){ //If the Weight-Endstop is reached and the Weight-Endstop is not reached before
        if(DEBUG>1) Serial.print("Weight reached TOP"); //Debug-Message
        ReachedWeightTarget = true; //Sets the reachedWeightTarget to true
        HWdc.brake();   //Reaching Weight top brakes the HW-Motor
      }
      if(SLes.read() == Slider::RIGHT && ReachedSliderTarget == false){ //If the Slider-Endstop is reached and the Slider-Endstop is not reached before
        if(DEBUG>1) Serial.print("Slider reached RIGHT"); //Debug-Message
        ReachedSliderTarget = true; //Sets the reachedSliderTarget to true
        SLdc.brake();   //Reaching Slider right brakes the SL-Motor
      }

      if(ERROR_MANAGEMENT){ //If Error-Management is enabled
        if((millis() - ctime) >= HW::wtime_calc && HW::wtime_calc != 0 &&  ReachedWeightTarget == false){  //If the time from the current run (millis() - ctime) is greater than HW::wtime_calc and the Weight-Endstop is not reached before
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::WG);  //Sets the MachineStatus to a Timeout-Error
          checkState(MachineStatus);  //Error-Handling
        }
        if((millis() - ctime) >= SL::stime_calc && SL::stime_calc != 0 &&  ReachedSliderTarget == false){  //If the time from the current run (millis() - ctime) is greater than SL::stime_calc and the Slider-Endstop is not reached before
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);  //Sets the MachineStatus to a Timeout-Error
          checkState(MachineStatus);  //Error-Handling
        }
      }
    }
  }
  //*5. Releasing the Hammerwheel
  MachineStatus = serv::hammergo(); //Releasing the Hammerwheel by calling the hammergo-function that returns a MachineStatus
  checkState(MachineStatus);  //Error-Handling
  //*6. Coupling the Slider and Hammer shafts
  COsv.write(COUP::EN); //Coupling the Slider and Hammer shafts

  if(DEBUG>0) Serial.println("Done | INIT: End"); //Debug-Message
  return;
}

//* Check the State of the Machine:
//This function is called whenever an error occurs. It checks the state of the machine and handles the error, if enabled.
void inline checkState(StatusClass status){
  //*1.Check for Error
  //*1a: If No Error-Management -> Return
  if(ERROR_MANAGEMENT == false){  //If Error-Management is disabled, return
    if(DEBUG>1) Serial.println("Check inactive"); //Debug-Message
    return;
  }
  //*1b: If Status == Success -> Return
  if(status.getStatus() == CompStatus::SUCCESS){  //If the Status is Success, return
    if(DEBUG>1) Serial.println("No Error.");  //Debug-Message
    return;
  }
  //*1c: If Error -> Disable Drivers, stop Motors
  else{
    HWdc.brake(); //Brakes the Hammerwheel
    SLdc.brake(); //Brakes the Slider
    HWdc.sleep(); //Puts the Hammerwheel-Motordriver to sleep
    SLdc.sleep(); //Puts the Slider-Motordriver to sleep
    digitalWrite(pin::STP_EN, LOW); //Puts the Stepper-Driver to sleep
  }
  //*2. (optional) Print Error to Serial Monitor and write Error to EEPROM
  if(DEBUG>0) printError(status); //Prints the Error to the Serial Monitor, if Debugging is enabled
  if(EEPROM_ENABLED) writeToEEPROM(status); //Writes the Error to the EEPROM, if enabled

  //*3. Starting Endless Loop
  uint32_t blinkTime = millis();  //Sets the blinkTime to the current time
  LED::color LEDColor = LED::RED; //Sets the LEDColor to RED
  while(1){ //Endless Loop
    if(millis() - blinkTime > ERROR_LED_DELAY){ //If the time from the last blink is greater than ERROR_LED_DELAY
      Go.updateLED(LEDColor); //Updates the LED to the LEDColor
      LEDColor = (LEDColor == LED::RED) ? LED::OFF : LED::RED;  //Switches the LEDColor between RED and OFF
      blinkTime = millis(); //Sets the blinkTime to the current time
    } //The LED blinks in the defined interval ERROR_LED_DELAY
    //*4. Check for Clear Error Button
    if(digitalRead(pin::CLEAR_ERROR) == LOW){ //If the Clear-Error-Button is pressed
      Go.updateLED(LED::MAGENTA); //Updates the LED to MAGENTA to indicate the clearing of the Error
      uint32_t resetTime = millis();  //Sets the resetTime to the current time to calculate the time the button is pressed
      while(digitalRead(pin::CLEAR_ERROR) == LOW){ //Is running till the Clear-Error-Button is released
        //*5. If Clear Button is pressed for 5 seconds -> Clear EEPROM and initialize Machine again
        if(millis() - resetTime > CLEAR_ERROR_TIME){    //If the time from the button press is greater than CLEAR_ERROR_TIME (e.g. 5 seconds)
          clearEEPROM();  //Clears the Error from the EEPROM
          if(DEBUG>0) Serial.println("EEPROM cleared"); //Debug-Message
          Go.updateLED(LED::OFF); //Updates the LED to OFF
          initStateOfMachine(); //Initializes the Machine to the Home-Position
          return;
        }
      }
    }
  }
}


//*----------------- Functions --------------------------------------------------------------------------------
//Severals functions that are used in the main-loop for the different components
//Stepper: setMicroSteps, home, move
//Servo: hammerstop, hammergo

//*------------- Stepper Functions -------------------

namespace step{

//Sets the microsteps for the stepper (1-32) and writes the correct values to the pins
void setMicroSteps(uint8_t microSteps){
  // Calculate the logarithm base 2 of microSteps, rounding down
  uint8_t log2MicroSteps = (microSteps > 0) ? mathFunc::log2(microSteps) : 0;

  // Calculate the values for M0, M1, and M2
  bool M0 = log2MicroSteps & 1;
  bool M1 = log2MicroSteps & 2;
  bool M2 = log2MicroSteps & 4;

  // Write the values to the pins
  digitalWrite(pin::STP_M0, M0);
  digitalWrite(pin::STP_M1, M1);
  digitalWrite(pin::STP_M2, M2);
}

//Homes the Stepper to the Home-Position (Hall-Sensor and Magnet)
StatusClass home(){
  digitalWrite(pin::STP_SLP, HIGH); //Enables the Stepper-Driver
  Serial.println("Homing"); //Debug-Message
  int homing = -1;  //Sets the homing to -1 (negative direction, one step)
  uint32_t errtime = millis();  //Sets the errtime to the current time for timeout-calculations
  while(SGha.read() == LOW){  //Is running till the Hall-Sensor is triggered
    SGst.moveTo(homing);  //Sets the stepper one step in the negative direction
    SGst.run(); //Runs the stepper
    delayMicroseconds(500); //Delay for the stepper to move
    homing--; //Decreases the homing by one

    if(ERROR_MANAGEMENT){ //If Error-Management is enabled
      if(STP::MICRO_STEPS * STP::SPR * 2 < abs(homing)){  //If the absolute value of homing is greater than 2 full turns
        digitalWrite(pin::STP_SLP, LOW);  //Disables the Stepper-Driver
        return StatusClass(CompStatus::TIMEOUT, FuncGroup::SG); //Returns a Timeout-Error
        }
    //Throwing error after 5 seconds without finding the Magnet (Hall-Sensor not sensing or Stepper not turning)
      if(millis() - errtime > STP::TIMEOUT){  //If the time from the errtime is greater than STP::TIME
        digitalWrite(pin::STP_SLP, LOW);
        return StatusClass(CompStatus::TIMEOUT, FuncGroup::SG);
      }
    }
  }
  if(DEBUG>1) Serial.println("Homed");  //Debug-Message
  SGst.setCurrentPosition(0); //Sets the current position of the stepper to 0
  digitalWrite(pin::STP_SLP, LOW);  //Disables the Stepper-Driver
  delay(500);
  return StatusClass(CompStatus::SUCCESS, FuncGroup::SG); //Returns a Success-Status
}

void move(int steps){ //Moves the stepper a specific amount of steps
  digitalWrite(pin::STP_SLP, HIGH); //Enables the Stepper-Driver
  SGst.moveTo(steps); //Sets the stepper to the given position
  SGst.runToPosition(); //Runs the stepper to the given position
  digitalWrite(pin::STP_SLP, LOW);  //Disables the Stepper-Driver
  delay(500);
  return;
}

//* ----------------- Servo Functions -----------------------

}

namespace serv {

//*Engage the hammerstop

StatusClass hammerstop() {
  //Debug Message
  if(DEBUG>1) Serial.println("HAMMERSTOP: Starts"); 
  HWdc.run(HW::SPEED);
  uint32_t errtime = millis();
  while(HWha.read() == true){
    delay(1);
    if(millis() - errtime > HW::TIMEOUT){
      //TimeOut Error-Handling -> millis() - errtime = deltaT
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
    }
  }
  //Wait till magnet isnt detected anymore
  errtime = millis();
  while(HWha.read() != true){
    delay(1);
    if(millis() - errtime > HW::TIMEOUT){
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
    }
  }
  //Delay to match position perfectly
  uint32_t WaitTillEngage = 2200/HW::RPM;
  Serial.println(HW::RPM);
  Serial.println(WaitTillEngage);
  delay(WaitTillEngage);
  HWdc.brake();
  delay(100);
  //Motor in postion -> engage hammerstop
  HSsv.run(HS::ON);
  delay(500);
  
  if(DEBUG>1) Serial.println("HAMMERSTOP: Done");
  return StatusClass(CompStatus::SUCCESS, FuncGroup::HW);
}

//*Disengage the hammerstop

StatusClass hammergo() {
  if(DEBUG>1) Serial.println("HAMMERGO: Starts");
  HSsv.run(HS::OFF);
  HWdc.run(HW::SPEED);
  delay(1000/HW::RPM);
  HWdc.brake();
  delay(500);
  if(DEBUG>1) Serial.println("HAMMERGO: Done");
  return StatusClass(CompStatus::SUCCESS, FuncGroup::HW);
}

} //? namespace serv

namespace mathFunc{
uint8_t log2(uint8_t n){
  uint8_t result = 0;
  while(n >>= 1){
    result++;
  }
  return result;
}
} // namespace mathfunc

 