#include "AllFunctions.hpp"    //All Functions includes all (custom) libraries and Arduino.h

//*Components:
MP6550 SLdc(pin::SL_IN1, pin::SL_IN2, pin::SL_SLP, SL::REVERSED, SL::AUTO_SLEEP); // Motor Slider (In1, In2, Sleep)
MP6550 HWdc(pin::HW_IN1, pin::HW_IN2, pin::HW_SLP, HW::REVERSED, HW::AUTO_SLEEP); // Motor Hammerwheel (In1, In2, Sleep)
ServoExp HSsv(pin::HS_SRV, HS::MIN, HS::MAX); // Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp COsv(pin::CO_SRV, COUP::MIN, COUP::MAX); // Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
AccelStepper SGst(AccelStepper::DRIVER, pin::STP_STP, pin::STP_DIR); // Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
Sensor::Endstops WGes(pin::WG_ES_T, pin::WG_ES_B); // Endschalter Weight (BottomPin, TopPin, TriggerState)
Sensor::Endstops SLes(pin::SL_ES_R, pin::SL_ES_L); // Endschalter Slider (LeftPin, RightPin, TriggerState)
Sensor::HallSwitch HWha(pin::HR_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Hammerwheel (Hallpin, TriggerState)
Sensor::HallSwitch SGha(pin::SG_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Sign (Hallpin, TriggerState)
Sensor::Button Go(pin::GO_BUT,pin::GO_LED_R,pin::GO_LED_G,pin::GO_LED_B); // Go-Button (Pin, TriggerState)

uint32_t ctime = 0; //Current Time, used for general and temporal Timeout-Calculations
uint32_t stime = 0; //Slider Timer, counts only, if Slider is moving and resets with full reset
uint32_t hwtime = 0; //Hammerwheel Timer, counts only, if Hammerwheel is moving
uint32_t wtime = 0; //Weight Timer for resetting manners

StatusClass MachineStatus = StatusClass(CompStatus::SUCCESS, FuncGroup::UNDEFINED);

bool fullReset = true;

void initStateOfMachine();

void inline idling();
void inline running();
void inline EndstopsRun();
void inline FullReset();
void inline WeightReset();
void inline resetting();

void setup() { 

  //*Serial-Setup:
  if(DEBUG>0) {
  Serial.begin(SERIALSET::BAUTRATE); // Starts Serial Connection between Arduino and PC via USB -> Baudrate = Bits per second (Bps)
  Serial.println("Setup started.");
  }

  //*EEPROM Error-Checking:
  pinMode(pin::CLEAR_ERROR, INPUT_PULLUP);
  if(hasErrorEEPROM() && ERROR_MANAGEMENT){
    if(DEBUG>0) Serial.println("EEPROM-Error detected.");
    MachineStatus = readFromEEPROM();
    ErrorState(MachineStatus);
  }

  //*Fan-Setup:
  if(FAN) pinMode(pin::FAN, OUTPUT); //Initializes the fan, if enabled (config)

  //* Sign-Stepper Setup:
  SGst.setMaxSpeed(5000.0);
  SGst.setAcceleration(1000.0);
  digitalWrite(pin::STP_RST, HIGH);
  digitalWrite(pin::STP_SLP, LOW);
  step::setMicroSteps(STP::MICRO_STEPS);

  //* Servo-Setup:
  HSsv.attach(); // Initializes Servo Hammerstop
  COsv.attach(); // Initializes Servo Kupplung
  HSsv.setTolerance(HS::TOLERANCE);     // Sets the tolerance for the servo
  COsv.setTolerance(COUP::TOLERANCE);   // Sets the tolerance for the servo
  HSsv.setPositions(HS::OFF, HS::ON);   // Sets the positions for the servo (OFF = pos1, ON = pos2)
  COsv.setPositions(COUP::DIS, COUP::EN);   // Sets the positions for the servo (OFF = DIS = pos1, ON = EN = pos2)
  COsv.runToPos(SERVO::OFF);              // Runs the servo to the position OFF
  HSsv.runToPos(SERVO::OFF);              // Runs the servo to the position OFF

  //* INIT:
  initStateOfMachine();   // Initializes the machine and resets to the initial state if necessary
  
  if(DEBUG>0) Serial.println("Setup done.");
  if(FAN) digitalWrite(pin::FAN, LOW);  
}

void loop() {

  //* IDLE:
  if(DEBUG>0) Serial.println("IDLE: Waiting for Go-Signal.");
  idling();
  if(DEBUG>0) Serial.println("IDLE: Go-Signal received.");

  //* RUN:
  running();
  if(DEBUG>0) Serial.println("RUN: Motors started, waiting for endstops.");
  EndstopsRun();
  if(DEBUG>0) Serial.println("RUN: Endstops reached");

  //* RESET:
  resetting();
  if(DEBUG>0) Serial.println("RESET: Done");
  if(DEBUG>1) Serial.println("-------------------------------");
}

//--------------------Functions--------------------

//*IDLE: Waiting for Go-Signal

void inline idling(){
  //*1. Sign to Pos 1:
  if(SGst.currentPosition() != 0 && STP::ENABLED == true){  //If the sign is not at the home position and the stepper is enabled (config)
    MachineStatus = step::home();                           //Homes the stepper and returns the status -> Sign Pos 1 (Hall-Sensor, Magnet)
    if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){   //If the status is not successful
      ErrorState(MachineStatus);                          //Error-Handling                
    }
  }
  //*2 Button LED to green:
  Go.updateLED(LED::GREEN);                              //Updates the LED to green, showing the visitor the machine is ready
  //*3. Waiting for User Input:
  while(Go.read() != true){                             //Waiting for someone to press the button
    delay(1);
  }
}

//*RUN: Running the machine

void inline running(){
  //*1. (optional) start the fan
  if(FAN) digitalWrite(pin::FAN, HIGH); //Turns on the fan if enabled (config)
  //*2. Button LED to cyan:
  Go.updateLED(LED::CYAN);        //Updates the LED to cyan, showing the visitor the machine is running
  //*3. Stepper to Pos 2:
  if(STP::ENABLED) SGst.move(STP::POS); //Moves the stepper to the position (config) -> Sign pos 2
  //*4. Running the Motors:
  HWdc.run(HW::SPEED);             //Runs the Hammerwheel with the given speed (config)
  SLdc.run(SL::SPEED);            //Runs the Slider with the given speed (config)
}


//*Function for running till an endstop is triggered

void inline EndstopsRun(){
  if(DEBUG>1) Serial.println("RUN: Endstops waiting...");
  //*1. Setting the timers to the current time
  ctime = millis();
  hwtime = millis();
  bool detectedMagnet = false;
  //*2. Waiting for ONE endstop to be triggered
  while(WGes.read() != Weight::BOTTOM && SLes.read() != Slider::LEFT){
    if(HWha.read() == HIGH && detectedMagnet == false){
        detectedMagnet = true;
        hwtime = millis();
      }
      else if(HWha.read() == LOW && detectedMagnet == true){
        detectedMagnet = false;
      }
      if(ERROR_MANAGEMENT && (millis() - hwtime) >= HW::TIMEOUT){
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
        ErrorState(MachineStatus);
      }
      if(ERROR_MANAGEMENT && (millis() - wtime + stime) > SL::TIMEOUT){
        MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);
        ErrorState(MachineStatus);
      }
  }
  
  if(DEBUG>1) Serial.print("RUN: Hit Endstops, starting ");
  //*3. Calculating timers for reset 
  wtime = millis() - ctime;   //Weight Timer for resetting purposes
  stime += wtime; //Slider timer gets reset after Full reset
  //*4. Braking the motors
  HWdc.brake();
  SLdc.brake();
}

void inline FullReset(){

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
    if(ERROR_MANAGEMENT && wtime != 0 && (millis() - ctime) > (wtime * HW::RS_TO_FACTOR)){
      MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
      ErrorState(MachineStatus);
    }
    if(ERROR_MANAGEMENT && stime != 0 && (millis() - ctime) > (stime * SL::RS_TO_FACTOR)){
      MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::SL);
      ErrorState(MachineStatus);
    }
  }
  //*3. Resetting the slider-timer
  stime = 0;
}

void inline WeightReset(){
  if(DEBUG > 1) Serial.println("RESET: Endstops waiting...");
  //*1. Setting the timer to the current time
  ctime = millis();
  //*2. Waiting for the Weight-Endstop to be triggered
  while(WGes.read() != Weight::TOP){
    if(ERROR_MANAGEMENT && wtime != 0 && (millis() - ctime) > (wtime * HW::RS_TO_FACTOR)){
      MachineStatus = StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
      ErrorState(MachineStatus);
    }
  }
  //*3. Braking the motor
  HWdc.brake();
}


//*RESET: Resetting the System

void inline resetting(){

  //*1. update the LED to yellow
  Go.updateLED(LED::YELLOW);

  //*2. Stepper to Pos 3:
  if(STP::ENABLED) SGst.move(STP::POS); //Moves the stepper to the next position (config) -> Sign pos 3

  if(DEBUG>0) Serial.println(fullReset ? "RESET: Full Reset" : "RESET: Weight Reset");

  //*3. Decoupling the Slider and Hammer shafts
  MachineStatus = serv::decouple();
  if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
    ErrorState(MachineStatus);
  }
  //*4. Blocking the Hammerwheel
  MachineStatus = serv::hammerstop();
  if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
    ErrorState(MachineStatus);
  }

  if(DEBUG>0) Serial.println("RESET: Motors reversed, waiting for endstops.");
  //*5. Running either both or just the weight motor back (depending on the condition of the Slider Endstops)
  if(SLes.read() == Slider::LEFT){
    SLdc.run(-SL::RS_SPEED);
    HWdc.run(-HW::RS_SPEED);
    FullReset();
  } 
  else{
    HWdc.run(-HW::RS_SPEED);
    WeightReset();
  } 

  if(DEBUG>0) Serial.println("RESET: Endstops reached.");
  //*6. Releasing the Hammerwheel
  MachineStatus = serv::hammergo();
  if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
    ErrorState(MachineStatus);
  }
  //*7. Coupling the Slider and Hammer shafts
  MachineStatus = serv::couple();
  if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
    ErrorState(MachineStatus);
  }
  delay(1000);
  //*8. Disabling the fan
  if(FAN) digitalWrite(pin::FAN, LOW); //Turns off the fan (if available)
}

//*Init: Initialize the System

void initStateOfMachine(){

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
    MachineStatus = serv::decouple();
    //*4a Decoupling the Slider and Hammer shafts
    if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
      ErrorState(MachineStatus);
    }
    //*4b Blocking the Hammerwheel
    MachineStatus = serv::hammerstop();
    if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
      ErrorState(MachineStatus);
    }
    //*4c Running the motors back to the start position
    SLdc.run(-SL::RS_SPEED);
    HWdc.run(-HW::RS_SPEED);
    FullReset();
  }
  //*5. Releasing the Hammerwheel
  MachineStatus = serv::hammergo();
  if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
    ErrorState(MachineStatus);
  }
  //*6. Coupling the Slider and Hammer shafts
  MachineStatus = serv::couple();
  if(ERROR_MANAGEMENT && MachineStatus.getStatus() != CompStatus::SUCCESS){
    ErrorState(MachineStatus);
  }

  if(DEBUG>0) Serial.println("Done | INIT: End");
  return;
}
 