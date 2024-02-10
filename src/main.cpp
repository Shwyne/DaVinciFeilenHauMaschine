#include "AllFunctions.hpp"    //All Functions includes all (custom) libraries and Arduino.h

//*Components:
MP6550 SLdc(pin::SL_IN1, pin::SL_IN2,pin::SL_SLP, SL::REVERSED, SL::AUTO_SLEEP); // Motor Slider (In1, In2, Sleep)
MP6550 HWdc(pin::HW_IN1, pin::HW_IN2,pin::HW_SLP, HW::REVERSED, HW::AUTO_SLEEP); // Motor Hammerwheel (In1, In2, Sleep)
ServoExp HSsv(pin::HS_SRV, HS::MIN, HS::MAX); // Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp COsv(pin::CO_SRV, COUP::MIN, COUP::MAX); // Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
StepExp SGst(STP::SPR, pin::STP_DIR, pin::STP_STP, pin::STP_EN, pin::STP_M0, pin::STP_M1, pin::STP_M2, pin::SG_HALL); // Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
//A4988 SGst(STP::SPR, pin::STP_DIR, pin::STP_STP, pin::STP_M0, pin::STP_M1,pin::STP_M2); // Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
Sensor::Endstops WGes(pin::WG_ES_T, pin::WG_ES_B); // Endschalter Weight (BottomPin, TopPin, TriggerState)
Sensor::Endstops SLes(pin::SL_ES_R, pin::SL_ES_L); // Endschalter Slider (LeftPin, RightPin, TriggerState)
Sensor::HallSwitch HWha(pin::HR_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Hammerwheel (Hallpin, TriggerState)
Sensor::HallSwitch SGha(pin::SG_HALL,HALL::TRIGGERED_IF); // Hall-Sensor Sign (Hallpin, TriggerState)
Sensor::Button Go(pin::GO_BUT,pin::GO_LED_R,pin::GO_LED_G,pin::GO_LED_B); // Go-Button (Pin, TriggerState)

ErrCode erCode = ErrCode::NO_ERROR;
uint32_t ctime = 0;
uint32_t stime = 0; //Slider Timer, counts only, if Slider is moving and resets with full reset
uint32_t HWtime = 0;

bool fullReset = true;

void initStateOfMachine();

void inline dloop();

void setup() {
  
  //Serial-Setup:
  if(DEBUG>0) {
  Serial.begin(SERIALSET::BAUTRATE); // Starts Serial Connection between Arduino and PC via USB -> Baudrate = Bits per second (Bps)
  Serial.println("Setup started.");
  }

  // Sign-Stepper Setup:
  SGst.begin(STP::RPM, STP::MICRO_STEPS);
  digitalWrite(pin::STP_SLP, HIGH);
  digitalWrite(pin::STP_RST, HIGH);

  // Servo-Setup:
  HSsv.attach(); // Initializes Servo Hammerstop
  COsv.attach(); // Initializes Servo Kupplung
  HSsv.run(HS::OFF);
  COsv.run(COUP::EN);
  HSsv.setTolerance(HS::TOLERANCE);
  COsv.setTolerance(COUP::TOLERANCE);
  //initStateOfMachine();
  
  if(DEBUG>0) Serial.println("Setup done.");
}

void loop() {
  //dloop();
}

void inline dloop() {
  //*IDLE: Waiting for Go-Signal
  Go.updateLED(LED::GREEN);
  if(DEBUG>0) Serial.println("IDLE: Waiting for Go-Signal.");
  while(Go.read() != true){
    //TODO: Figure out if Error-Checking is useful here
    delay(1);
  }
  if(DEBUG>0) Serial.println("IDLE: Go-Signal received.");
  sleepDrivers(false);
  Go.updateLED(LED::CYAN);
  
  //*RUN: Running the machine
  SGst.run(STP::POS);
  check();
  HWdc.run(HW::SPEED);
  SLdc.run(SL::SPEED);
  if(DEBUG>0) Serial.println("RUN: Motors started, waiting for endstops.");
  //TODO: Implement Estimating distance to endstop for error-checking
  bool reachedBot = false;
  bool reachedLeft = false;
  bool DetectingMagnet = false;
  bool fullReset = false;
  ctime = millis();
  HWtime = millis();
  while((reachedBot == false) && (reachedLeft == false)){
    if(SLes.read() == Slider::LEFT && reachedLeft == false){
      if(DEBUG>1) Serial.println("RUN: Slider LEFT");
      stime = stime + (millis()-ctime);
      reachedLeft = true;
      fullReset = true;
      SLdc.brake();
      HWdc.brake();
    }
    if(WGes.read() == Weight::BOTTOM && reachedBot == false){
      if(DEBUG>1) Serial.println("RUN: Weight BOTTOM");
      stime = stime + (millis()-ctime);
      reachedBot = true;
      HWdc.brake();
      SLdc.brake();
    }
    if(WGes.read() != Weight::BOTTOM){
      if(HWha.read() && DetectingMagnet == false){
        HWtime = millis();                           //TODO: Add another timer cuz its clashing with L79
        DetectingMagnet = true;
      }
      else if(HWha.read() == false && DetectingMagnet == true){
        DetectingMagnet = false;
      }
      if(millis() - HWtime > HW::TIMEOUT){
        erCode = ErrCode::HW_TIMEOUT;
        if(ERROR_MANAGEMENT) break;       //TODO: Check if it returns to while or not
      }
      //TODO: Implement function to calculate the time to the endstop (RPM, Steigung, etc.)
      if(stime+(millis()-ctime) >= SL::TIMEOUT){
        erCode = ErrCode::SL_TIMEOUT;
        if(ERROR_MANAGEMENT) break;
      }
    }
    delay(1);
  }
  check();
  HWdc.brake();
  SLdc.brake();
  Go.updateLED(LED::YELLOW);
  if(DEBUG>0) Serial.println("RUN: Endstops reached");
  
  //*RESET: Resetting the System
  SGst.run(STP::POS);
  check();
  if(DEBUG>0) Serial.println(fullReset ? "RESET: Full Reset" : "RESET: Weight Reset");
  serv::decouple();
  check();
  serv::hammerstop();
  check();
  if(DEBUG>0) Serial.println("RESET: Motors reversed, waiting for endstops.");
  //TODO: Implement Estimating distance to endstop for error-checking
  ctime = millis();
  HWtime = millis();
  DetectingMagnet = false;
  if(fullReset == true){
    stime = 0;              //TODO: Reset at end, cuz then if(stime-(ctime-millis()) >= SL::TIMEOUT) will be true
    SLdc.run(SL::RS_SPEED);
    HWdc.run(HW::RS_SPEED);
    bool ReachedTop = false;
    bool ReachedRight = false;
    while((ReachedTop == false) || (ReachedRight == false)){
      if(SLes.read() == Slider::RIGHT && ReachedRight == false){
        if(DEBUG>1) Serial.println("RESET: Slider RIGHT");
        ReachedRight = true;
        SLdc.brake();
      }
      if(WGes.read() == Weight::TOP && ReachedTop == false){
        if(DEBUG>1) Serial.println("RESET: Weight TOP");
        ReachedTop = true;
        HWdc.brake();
      }
      //TODO: See weight reset (no fullreset)
      /*if(WGes.read() != Weight::TOP){
        if(HWha.read() && DetectingMagnet == false){
          HWtime = millis();
          DetectingMagnet = true;
        }
        else if(HWha.read() == false && DetectingMagnet == true){
          DetectingMagnet = false;
        }
        if(millis() - HWtime > HW::TIMEOUT){
          erCode = ErrCode::HW_TIMEOUT;
          if(ERROR_MANAGEMENT) break;
        }
      }*/
      /*if(SLes.read() != Slider::RIGHT){
        if(stime+(millis()-ctime) >= SL::TIMEOUT){
          erCode = ErrCode::SL_TIMEOUT
          if(ERROR_MANAGEMENT) break;
        }*/
      }
      check();
  }
  else {
    HWdc.run(HW::RS_SPEED);
    HWtime = millis();
    DetectingMagnet = false;
    while(WGes.read() != Weight::TOP){
      //TODO: Add Counter for last complete weight run, use the ration speed/RS_speed and calc a timer. If timer is reached, break and set error
      delay(1);
    }
    check();
    Serial.println("RESET: Weight TOP");
    HWdc.brake();
  }
  serv::hammergo();
  check();
  serv::couple();
  check();
  SGst.home();
  check();
  sleepDrivers(true);
  delay(1000);
  if(DEBUG>0) Serial.println("RESET: Done");
  if(DEBUG>1) Serial.println("-------------------------------");
}

void initStateOfMachine(){
  if(SLes.read() != Slider::RIGHT && WGes.read() != Weight::TOP){
    if(DEBUG>1) Serial.println("INIT: Full Reset");
    HWdc.run(HW::RS_SPEED);
    SLdc.run(SL::RS_SPEED);
    bool ReachedTop = false;
    bool ReachedRight = false;
    while((ReachedTop == false) || (ReachedRight == false)){
      if(SLes.read() == Slider::RIGHT && ReachedRight == false){
        if(DEBUG>1) Serial.println("INIT: Slider RIGHT");
        ReachedRight = true;
        SLdc.brake();
      }
      if(WGes.read() == Weight::TOP && ReachedTop == false){
        if(DEBUG>1) Serial.println("INIT: Weight TOP");
        ReachedTop = true;
        HWdc.brake();
      }
    }
  }
  else if(SLes.read() != Slider::RIGHT){
    if(DEBUG>1) Serial.println("INIT: Slider");
    SLdc.run(SL::RS_SPEED);
    while(SLes.read() != Slider::RIGHT){
      delay(1);
    }
    SLdc.brake();
  }
  else if(WGes.read() != Weight::TOP){
    if(DEBUG>1) Serial.println("INIT: Weight");
    HWdc.run(HW::RS_SPEED);
    while(WGes.read() != Weight::TOP){
      delay(1);
    }
    HWdc.brake();
  }
  if(DEBUG>0) Serial.println("INIT: Start Position");
  return;
}
