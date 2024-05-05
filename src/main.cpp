//#include "TestFunctions.hpp"    //All Functions includes all (custom) libraries and Arduino.h
#include "ExceptHandling.hpp"  //Error-Handling
#include "drivers.hpp"  //Drivers
#include "Sensors.hpp"  //Sensors
#include "config.hpp"   //Configurations
#include <AccelStepper.h>   //Stepper-Library

//*----------------- Variables ----------------------

uint32_t ctime = 0; //Current Time, used for general and temporal Timeout-Calculations
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
void inline initStateOfMachine();
void inline checkState(StatusClass status);

//*----------------------- Setup -------------------------------------------------------------------------------

void setup() {
  bool skipInit = false; 
  //*Serial-Setup:
  if(DEBUG>0) {
  Serial.begin(SERIALSET::BAUTRATE); // Starts Serial Connection between Arduino and PC via USB -> Baudrate = Bits per second (Bps)
  Serial.println("Setup started.");
  }

  //*EEPROM Error-Checking:
  pinMode(pin::CLEAR_ERROR, INPUT_PULLUP);
  if(hasErrorEEPROM()){
    if(DEBUG>0) Serial.println("EEPROM-Error detected.");
    MachineStatus = StatusClass(EEPROM.read(EEPROM_ADDRESS+1), EEPROM.read(EEPROM_ADDRESS+2));
    checkState(MachineStatus);
    if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS) skipInit = true;
  }

  //*Fan-Setup:
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

  //* Servo-Setup:
  HSsv.attach(); // Initializes Servo Hammerstop
  COsv.attach(); // Initializes Servo Kupplung
  COsv.write(COUP::DIS); // Sets the Coupling to the disenaged position
  HSsv.write(HS::OFF); // Sets the Hammerstop to the off position

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
  while(Go.read() != true){                             //Waiting for someone to press the button
    delay(1);
  }
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
  SLdc.run(SL::SPEED);
  if(DEBUG>0) Serial.println("RUN: Motors started, waiting for endstops.");
  //*5. Setting the timers to the current time
  ctime = millis();
  hwtime = millis();
  bool detectedMagnet = false;
  //*6. Waiting for ONE endstop to be triggered
  while(WGes.read() != Weight::BOTTOM && SLes.read() != Slider::LEFT){
    if(ERROR_MANAGEMENT){
      if(HWha.read() == HIGH && detectedMagnet == false){
        detectedMagnet = true;
        hwtime = millis();
      }
      else if(HWha.read() == LOW && detectedMagnet == true){
        detectedMagnet = false;
      }
      if((millis() - hwtime) >= HW::TIMEOUT){
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
        checkState(MachineStatus);
        return;
      }
      if((millis() - ctime + stime) >= SL::TIMEOUT){  //Timeout occurs, if the sum of time from when the slider started (stime) and the time from the current run (millis() - ctime) is greater than SL::TIMEOUT
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);
        checkState(MachineStatus);
        return;
      }
    }
  }
  
  if(DEBUG>1) Serial.print("RUN: Hit Endstops, starting ");
  //*7. Updating Slider Timer:
  stime += millis()-ctime;  //Adding the time span till an endstop was hit to the slider-timer
  //*8. Braking the motors
  HWdc.brake();
  SLdc.brake();
  if(DEBUG>0) Serial.println("RUN: Endstops reached");


  //* ----------------- RESET ---------------------------
  
  //*1. update the LED to yellow
  Go.updateLED(LED::YELLOW);

  //*2. Stepper to Pos 3:
  if(STP::ENABLED) step::move(2*STP::POS); //Moves the stepper to the next position (config) -> Sign pos 3

  //*3. Decoupling the Slider and Hammer shafts
  COsv.write(COUP::DIS);
  //*4. Blocking the Hammerwheel
  MachineStatus = serv::hammerstop();
  checkState(MachineStatus);
  
  if(DEBUG>0) Serial.println("RESET: Motors reversed, waiting for endstops.");
  //*5. Running either both or just the weight motor back (depending on the condition of the Slider Endstops)
  if(SLes.read() == Slider::LEFT){
    SLdc.run(-SL::RS_SPEED);
    HWdc.run(-HW::RS_SPEED);
    if(DEBUG > 1) Serial.println("RESET: Endstops waiting...");
    bool ReachedWeightTarget = false;
    bool ReachedSliderTarget = false;
    //*1. Setting the timer to the current time
    ctime = millis();
    //*2. Waiting for BOTH endstops to be triggered
    while((ReachedWeightTarget == false) || (ReachedSliderTarget == false)){
      if(WGes.read() == Weight::TOP && ReachedWeightTarget == false){
        if(DEBUG>1) Serial.print("Weight reached TOP");
        ReachedWeightTarget = true;
        HWdc.brake();   //Reaching Weight top brakes the HW-Motor
      }
      if(SLes.read() == Slider::RIGHT && ReachedSliderTarget == false){
        if(DEBUG>1) Serial.print("Slider reached RIGHT");
        ReachedSliderTarget = true;
        SLdc.brake();   //Reaching Slider right brakes the SL-Motor
      }
      if(ERROR_MANAGEMENT){
        if(HW::wtime_calc != 0 && millis() - ctime >= HW::wtime_calc && ReachedWeightTarget == false){
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
          checkState(MachineStatus);
        }
        if(SL::stime_calc != 0 && millis() - ctime >= SL::stime_calc && ReachedSliderTarget == false){
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);
          checkState(MachineStatus);
        }
      }
    }
    //*3. Resetting the slider-timer
    stime = 0;
  } 
  else{
    HWdc.run(-HW::RS_SPEED);
    if(DEBUG > 1) Serial.println("RESET: Endstops waiting...");
    //*1. Setting the timer to the current time
    ctime = millis();
    //*2. Waiting for the Weight-Endstop to be triggered
    while(WGes.read() != Weight::TOP){
      if(ERROR_MANAGEMENT){
        if(HW::wtime_calc != 0 && millis() - ctime >= HW::wtime_calc){
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
        checkState(MachineStatus);
        }
      }
  }
  //*3. Braking the motor
  HWdc.brake();
  } 

  if(DEBUG>0) Serial.println("RESET: Endstops reached.");
  //*6. Releasing the Hammerwheel
  MachineStatus = serv::hammergo();
  checkState(MachineStatus);
  //*7. Coupling the Slider and Hammer shafts
  COsv.write(COUP::EN);
  delay(1000);
  //*8. Disabling the fan
  if(FAN) digitalWrite(pin::FAN, LOW); //Turns off the fan (if available)
  
  if(DEBUG>0) Serial.println("RESET: Done");

  //* ----------------- IDLE --------------------------- LOOP END
  if(DEBUG>1) Serial.println("-------------------------------");
}

//*----------------- States --------------------------------------------------------------------------------

void inline initStateOfMachine(){
  //* INIT State of the Machine:
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
  //*4. Checking Position (Slider, Weight), if not in Start position
  if(SLes.read() != Slider::RIGHT || WGes.read() != Weight::TOP){
    COsv.write(COUP::DIS);
    //*4b Blocking the Hammerwheel
    MachineStatus = serv::hammerstop();
    if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
      checkState(MachineStatus);
    }
    //*4c Running the motors back to the start position
    SLdc.run(-SL::RS_SPEED);
    HWdc.run(-HW::RS_SPEED);
    if(DEBUG > 1) Serial.println("RESET: Endstops waiting...");
    bool ReachedWeightTarget = false;
    bool ReachedSliderTarget = false;
    //*1. Setting the timer to the current time
    ctime = millis();
    //*2. Waiting for BOTH endstops to be triggered
    while((ReachedWeightTarget == false) || (ReachedSliderTarget == false)){
      if(WGes.read() == Weight::TOP && ReachedWeightTarget == false){
        if(DEBUG>1) Serial.print("Weight reached TOP");
        ReachedWeightTarget = true;
        HWdc.brake();   //Reaching Weight top brakes the HW-Motor
      }
      if(SLes.read() == Slider::RIGHT && ReachedSliderTarget == false){
        if(DEBUG>1) Serial.print("Slider reached RIGHT");
        ReachedSliderTarget = true;
        SLdc.brake();   //Reaching Slider right brakes the SL-Motor
      }

      if(ERROR_MANAGEMENT){
        if((millis() - ctime) > HW::wtime_calc && HW::wtime_calc != 0 &&  ReachedWeightTarget == false){
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
          checkState(MachineStatus);
        }
        if((millis() - ctime) > SL::stime_calc && SL::stime_calc != 0 &&  ReachedSliderTarget == false){
          MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);
          checkState(MachineStatus);
        }
      }
    }
  }
  //*5. Releasing the Hammerwheel
  MachineStatus = serv::hammergo();
  if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
    checkState(MachineStatus);
  }
  //*6. Coupling the Slider and Hammer shafts
  COsv.write(COUP::EN);

  if(DEBUG>0) Serial.println("Done | INIT: End");
  return;
}


void inline checkState(StatusClass status){
  //*1.Check for Error
  //*1a: If No Error-Management -> Return
  if(ERROR_MANAGEMENT == false){
    if(DEBUG>1) Serial.println("Check inactive");
    return;
  }
  //*1b: If Status == Success -> Return
  if(status.getStatus() == CompStatus::SUCCESS){
    if(DEBUG>1) Serial.println("No Error.");
    return;
  }
  //*1c: If Error -> Disable Drivers, stop Motors
  else{
    HWdc.brake();
    SLdc.brake();
    HWdc.sleep();
    SLdc.sleep();
    digitalWrite(pin::STP_EN, LOW);
  }
  //*2. (optional) Print Error to Serial Monitor and write Error to EEPROM
  if(DEBUG>0) printError(status);
  if(EEPROM_ENABLED) writeToEEPROM(status);

  //*3. Starting Endless Loop
  uint32_t blinkTime = millis();
  uint32_t resetTime = 0;
  LED::color LEDColor = LED::RED;
  while(1){
    if(millis() - blinkTime > ERROR_LED_DELAY){
      Go.updateLED(LEDColor);
      LEDColor = (LEDColor == LED::RED) ? LED::OFF : LED::RED;
      blinkTime = millis();
    }
    //*4. Check for Clear Error Button
    if(digitalRead(pin::CLEAR_ERROR) == LOW){
      Go.updateLED(LED::MAGENTA);
      resetTime = millis();
      while(digitalRead(pin::CLEAR_ERROR) == LOW){
        //*5. If Clear Button is pressed for 5 seconds -> Clear EEPROM and initialize Machine again
        if(millis() - resetTime > CLEAR_ERROR_TIME){
          clearEEPROM();
          if(DEBUG>0) Serial.println("EEPROM cleared");
          Go.updateLED(LED::OFF);
          initStateOfMachine();
          return;
        }
      }
    }
  }
}


//*----------------- Functions --------------------------------------------------------------------------------

//*------------- Stepper Functions -------------------

namespace step{

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

StatusClass home(){
  digitalWrite(pin::STP_SLP, HIGH);
  Serial.println("Homing");
  int homing = -1;
  uint32_t errtime = millis();
  while(SGha.read() == LOW){
    SGst.moveTo(homing);
    homing--;
    SGst.run();
    delayMicroseconds(500);
    //Throwing error after 2 full turns without finding the Magnet (Hall-Sensor not sensing)
    if(STP::MICRO_STEPS * STP::SPR * 2 < abs(homing)){
      if(ERROR_MANAGEMENT){
        digitalWrite(pin::STP_SLP, LOW);
        return StatusClass(CompStatus::TIMEOUT, FuncGroup::SG);
      }
    }
    //Throwing error after 5 seconds without finding the Magnet (Hall-Sensor not sensing or Stepper not turning)
    if(millis() - errtime > 5000){
      if(ERROR_MANAGEMENT){
        digitalWrite(pin::STP_SLP, LOW);
        return StatusClass(CompStatus::TIMEOUT, FuncGroup::SG);
      }
    }
  }
  Serial.println("Homed");
  SGst.setCurrentPosition(0);
  digitalWrite(pin::STP_SLP, LOW);
  return StatusClass(CompStatus::SUCCESS, FuncGroup::SG);
}

void move(int steps){
  digitalWrite(pin::STP_SLP, HIGH);
  SGst.moveTo(steps);
  SGst.runToPosition();
  digitalWrite(pin::STP_SLP, LOW);
  return;
}

//* ----------------- Servo Functions -----------------------

}

namespace serv {

//*Engage the hammerstop

StatusClass hammerstop() {
  //Debug Message
  if(DEBUG>1) Serial.println("HAMMERSTOP: Starts");
  HSsv.attach();
  HWdc.run(HW::SPEED);
  uint32_t errtime = millis();
  while(HWha.read() == true){
    delay(1);//TODO: Add error-management and test it
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
  delay(4500/HW::RPM);
  HWdc.brake();
  delay(100);
  //Motor in postion -> engage hammerstop
  HSsv.run(HS::ON);
  
  if(DEBUG>1) Serial.println("HAMMERSTOP: Done");
  return StatusClass(CompStatus::SUCCESS, FuncGroup::HW);
}

//*Disengage the hammerstop

StatusClass hammergo() {
  if(DEBUG>1) Serial.println("HAMMERGO: Starts");
  HSsv.run(HS::OFF);
  HWdc.run(HW::SPEED);
  delay(5000/HW::RPM);
  HWdc.brake();
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

 