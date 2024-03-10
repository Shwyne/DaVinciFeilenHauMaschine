#include "AllFunctions.hpp"    //All Functions includes all (custom) libraries and Arduino.h

//*Components:
MP6550 SLdc(pin::SL_IN1, pin::SL_IN2, pin::SL_SLP, SL::REVERSED, SL::AUTO_SLEEP); // Motor Slider (In1, In2, Sleep)
MP6550 HWdc(pin::HW_IN1, pin::HW_IN2, pin::HW_SLP, HW::REVERSED, HW::AUTO_SLEEP); // Motor Hammerwheel (In1, In2, Sleep)
ServoExp HSsv(pin::HS_SRV, HS::MIN, HS::MAX); // Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp COsv(pin::CO_SRV, COUP::MIN, COUP::MAX); // Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
AccelStepper SGst(AccelStepper::DRIVER, pin::STP_STP, pin::STP_DIR); // Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
//A4988 SGst(STP::SPR, pin::STP_DIR, pin::STP_STP, pin::STP_M0, pin::STP_M1,pin::STP_M2); // Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
Sensor::Endstops WGes(pin::WG_ES_T, pin::WG_ES_B); // Endschalter Weight (BottomPin, TopPin, TriggerState)
Sensor::Endstops SLes(pin::SL_ES_R, pin::SL_ES_L); // Endschalter Slider (LeftPin, RightPin, TriggerState)
Sensor::HallSwitch HWha(pin::HR_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Hammerwheel (Hallpin, TriggerState)
Sensor::HallSwitch SGha(pin::SG_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Sign (Hallpin, TriggerState)
Sensor::Button Go(pin::GO_BUT,pin::GO_LED_R,pin::GO_LED_G,pin::GO_LED_B); // Go-Button (Pin, TriggerState)

ErrCode erCode = ErrCode::NO_ERROR;
uint32_t ctime = 0;
uint32_t stime = 0; //Slider Timer, counts only, if Slider is moving and resets with full reset
uint32_t HWtime = 0;  //Timer for Hammerwheel + Hall-Sensor

bool fullReset = true;

void initStateOfMachine();

void inline dloop();
void inline idling();
void inline running();
void inline WaitingForEndstops(Weight::State WeightState, Slider::State SliderState, bool brakeAtFirst);
void inline resetting();

bool forward = true;

void setup() { 
  //TODO if eeprom error = true -> go back into error state.
  //TODO The eeprom has to be cleared by a maintainer. and shouldnt be easy to be cleared by the user.
  //Serial-Setup:
  Go.updateLED(LED::WHITE);
  if(DEBUG>0) {
  Serial.begin(SERIALSET::BAUTRATE); // Starts Serial Connection between Arduino and PC via USB -> Baudrate = Bits per second (Bps)
  Serial.println("Setup started.");
  }

  // Sign-Stepper Setup:
  SGst.setMaxSpeed(5000.0); // Increased from 5000.0 to 50000.0
  SGst.setAcceleration(1000.0);
  digitalWrite(pin::STP_SLP, HIGH);
  digitalWrite(pin::STP_RST, HIGH);
  digitalWrite(pin::STP_M0, HIGH);
  digitalWrite(pin::STP_M1, HIGH);
  digitalWrite(pin::STP_M2, HIGH);

  // Servo-Setup:
  HSsv.attach(); // Initializes Servo Hammerstop
  COsv.attach(); // Initializes Servo Kupplung
  HSsv.setTolerance(HS::TOLERANCE);
  COsv.setTolerance(COUP::TOLERANCE);
  HSsv.setPositions(HS::OFF, HS::ON);
  COsv.setPositions(COUP::DIS, COUP::EN);

  COsv.runToPos(SERVO::OFF);
  HSsv.runToPos(SERVO::OFF);

  initStateOfMachine();
  
  if(DEBUG>0) Serial.println("Setup done.");

}

void loop() {
  dloop();
}

void inline dloop() {

  if(DEBUG>0) Serial.println("IDLE: Waiting for Go-Signal.");
  idling();
  if(DEBUG>0) Serial.println("IDLE: Go-Signal received.");

  running();
  if(DEBUG>0) Serial.println("RUN: Motors started, waiting for endstops.");
  WaitingForEndstops(Weight::BOTTOM, Slider::LEFT, true);
  if(DEBUG>0) Serial.println("RUN: Endstops reached");
  
  resetting();
  if(DEBUG>0) Serial.println("RESET: Done");
  if(DEBUG>1) Serial.println("-------------------------------");
}


//--------------------Functions--------------------

//*IDLE: Waiting for Go-Signal

void inline idling(){
  if(SGst.currentPosition() != 0 && STP::ENABLED == true){
    Serial.println("Homing");
    int homing = -1;
    while(SGha.read() == LOW){
      SGst.moveTo(homing);
      homing--;
      SGst.run();
      delayMicroseconds(500);
    }
    Serial.println("Homed");
    SGst.setCurrentPosition(0);
    check();
  }
  Go.updateLED(LED::GREEN);
  while(Go.read() != true){
    delay(1);
  }
}

//*RUN: Running the machine

void inline running(){
  Go.updateLED(LED::CYAN);
  if(STP::ENABLED == true){
    SGst.moveTo(STP::POS);
    SGst.runToPosition();
    check();
  }
  HWdc.run(HW::SPEED);
  SLdc.run(SL::SPEED);
}

//*WAIT: Waiting for Endstops and then returns (brakeAtFirst = true: Brake at first reached endstop, brakeAtFirst = false: Brake when both endstops are reached)

void inline WaitingForEndstops(Weight::State WeightState, Slider::State SliderState, bool brakeAtFirst){
  bool ReachedWeightTarget = false;
  bool ReachedSliderTarget = false;
  while((ReachedWeightTarget == false) || (ReachedSliderTarget == false)){
    if(WGes.read() == WeightState && ReachedWeightTarget == false){
      if(DEBUG>1) Serial.print("Weight reached Target");
      ReachedWeightTarget = true;
      HWdc.brake();
      if(brakeAtFirst){
        fullReset = false;
        SLdc.brake();
        return;
      }
    }
    if(SLes.read() == SliderState && ReachedSliderTarget == false){
      if(DEBUG>1) Serial.print("Slider reached Target");
      ReachedSliderTarget = true;
      SLdc.brake();
      if(brakeAtFirst){
        fullReset = true;
        HWdc.brake();
        return;
      }
    }
  }
  return;
}

//*RESET: Resetting the System

void inline resetting(){

  Go.updateLED(LED::YELLOW);
  if(STP::ENABLED == true){
    SGst.moveTo(2*STP::POS);
    SGst.runToPosition();
    check();
  }

  if(DEBUG>0) Serial.println(fullReset ? "RESET: Full Reset" : "RESET: Weight Reset");
  serv::decouple();
  check();
  serv::hammerstop();
  check();

  if(DEBUG>0) Serial.println("RESET: Motors reversed, waiting for endstops.");
  if(fullReset == true){
    SLdc.run(-SL::RS_SPEED);
    HWdc.run(-HW::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false);
  }
  else {
    HWdc.run(-HW::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, true);
  }
  if(DEBUG>0) Serial.println("RESET: Endstops reached.");
  serv::hammergo();
  check();
  serv::couple();
  check();
  delay(1000);

}

//*Init: Initialize the System

void initStateOfMachine(){
  if(DEBUG>0) Serial.println("Press Go-Button to initialize the machine.");
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);
  if(DEBUG>0) Serial.print("INIT: Begin | ");
  if(SLes.read() != Slider::RIGHT || WGes.read() != Weight::TOP){
    serv::decouple();
    check();
    serv::hammerstop();
    check();
  }
  switch(SLes.read()){
    case 0:
      Serial.print("Slider -> Untriggered");
      break;
    case 1:
      Serial.print("Slider -> Right");
      break;
    case 2:
      Serial.print("Slider -> Left");
      break;
    case 3:
      Serial.print("Slider -> Both");
      break;
    default:
      Serial.print("Slider -> Error");
      break;
 }
 if(WGes.read()){
    switch(WGes.read()){
      case 0:
        Serial.print("Weight -> Untriggered");
        break;
      case 1:
        Serial.print("Weight -> Top");
        break;
      case 2:
        Serial.print("Weight -> Bottom");
        break;
      case 3:
        Serial.print("Weight -> Both");
        break;
      default:
        Serial.print("Weight -> Error");
        break;
    }
 }
  if(SLes.read() != Slider::RIGHT && WGes.read() != Weight::TOP){
    if(DEBUG>1) Serial.print("INIT: Full | ");
    SLdc.run(-SL::RS_SPEED);
    HWdc.run(-HW::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false);
  }
  else if(SLes.read() != Slider::RIGHT){
    if(DEBUG>1) Serial.print("Slider -> RIGHT | ");
    SLdc.run(-SL::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false);
    SLdc.brake();
  }
  else if(WGes.read() != Weight::TOP){
    if(DEBUG>1) Serial.print("Weight -> TOP | ");
    HWdc.run(-HW::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false);
    HWdc.brake();
  }
  
  serv::hammergo();
  check();
  serv::couple();
  check();
  Serial.println("Homing");
    int homing = -1;
    while(SGha.read() == LOW){
      SGst.moveTo(homing);
      homing--;
      SGst.run();
      delay(1);
    }
    Serial.println("Homed");
    SGst.setCurrentPosition(0);
    check();
  if(DEBUG>0) Serial.println("Done | INIT: End");
  return;
}
 