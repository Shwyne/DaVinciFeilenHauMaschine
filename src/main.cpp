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

ErrCode erCode = ErrCode::NO_ERROR;
uint32_t ctime = 0;
uint32_t stime = 0; //Slider Timer, counts only, if Slider is moving and resets with full reset
uint32_t hwtime = 0; //Hammerwheel Timer, counts only, if Hammerwheel is moving
uint32_t wtime = 0; //Weight Timer for resetting manners

bool fullReset = true;

void initStateOfMachine();

void inline dloop();
void inline idling();
void inline running();
void inline WaitingForEndstops(Weight::State WeightState, Slider::State SliderState, bool brakeAtFirst, bool reset = false);
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
    printError();
    showErrorLED();
  }

  pinMode(pin::FAN, OUTPUT); //Initializes the fan (if available)

  Go.updateLED(LED::WHITE);

  //* Sign-Stepper Setup:
  SGst.setMaxSpeed(5000.0);
  SGst.setAcceleration(1000.0);
  digitalWrite(pin::STP_RST, HIGH);
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

  initStateOfMachine();   // Initializes the machine and resets to the initial state if necessary
  
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
      //delayMicroseconds(500);   //TODO: Check if this is okay to comment out.
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
  digitalWrite(pin::FAN, HIGH); //Turns on the fan (if available)
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

void inline WaitingForEndstops(Weight::State WeightState, Slider::State SliderState, bool brakeAtFirst, bool reset){
  bool ReachedWeightTarget = false;
  bool ReachedSliderTarget = false;
  bool detectedMagnet = false;
  ctime = millis();
  hwtime = millis();
  while((ReachedWeightTarget == false) || (ReachedSliderTarget == false)){
    if(WGes.read() == WeightState && ReachedWeightTarget == false){
      if(DEBUG>1) Serial.print("Weight reached Target");
      ReachedWeightTarget = true;
      HWdc.brake();
      if(brakeAtFirst){
        fullReset = false;
        wtime = millis() - ctime;
        stime += wtime;
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
    if(reset){
      if(wtime != 0 && (millis() - ctime) > wtime){
        erCode = ErrCode::WG_TIMEOUT;
        check();
      }
      if(stime != 0 && (millis() - ctime) > stime){
        erCode = ErrCode::SL_TIMEOUT;
        check();
      }
    }
    else{
      if(HWha.read() == HIGH && detectedMagnet == false){
      detectedMagnet = true;
       hwtime = millis();
      }
      else if(HWha.read() == LOW && detectedMagnet == true){
        detectedMagnet = false;
      }
      if((millis() - hwtime) > HW::TIMEOUT){
        erCode = ErrCode::HW_TIMEOUT;
        check();
      }
      if((millis() - wtime + stime) > SL::TIMEOUT && ReachedSliderTarget == false){
        erCode = ErrCode::SL_TIMEOUT;
        check();
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
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false, true);
    stime = 0;
  }
  else {
    HWdc.run(-HW::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, true, true);
  }
  if(DEBUG>0) Serial.println("RESET: Endstops reached.");
  serv::hammergo();
  check();
  serv::couple();
  check();
  delay(1000);
  digitalWrite(pin::FAN, LOW); //Turns off the fan (if available)
}

//*Init: Initialize the System

void initStateOfMachine(){
  if(DEBUG>0){
    Serial.println("Press Go-Button to initialize the machine.");
    Go.waitForPress();
  }
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
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false, true);
  }
  else if(SLes.read() != Slider::RIGHT){
    if(DEBUG>1) Serial.print("Slider -> RIGHT | ");
    SLdc.run(-SL::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false, true);
    SLdc.brake();
  }
  else if(WGes.read() != Weight::TOP){
    if(DEBUG>1) Serial.print("Weight -> TOP | ");
    HWdc.run(-HW::RS_SPEED);
    WaitingForEndstops(Weight::TOP, Slider::RIGHT, false, true);
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
 