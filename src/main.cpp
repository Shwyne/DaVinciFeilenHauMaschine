#include <Arduino.h>             //Arduino Library
#include "DC_Motor_Driver.hpp"     //Driver MP6550 Library
#include "ServoExp.hpp"          //Custom Servo Library -> depends on Servo.h
#include "Sensor.hpp"           //Sensor Library
#include "config.hpp"            //Config file -> Pins, settings, etc.
//#include "Testfunctions.hpp"     //Testfunctions
#include <DRV8825.h>        //Stepper Driver Library
#include <A4988.h>


DC_Motor_Driver::MP6550 SLdc(pin::SL_IN1, pin::SL_IN2, pin::SL_SLP);  //Motor Slider (In1, In2, Sleep)
DC_Motor_Driver::MP6550 HRdc(pin::HR_IN1, pin::HR_IN2, pin::HR_SLP);  //Motor Hammerwheel (In1, In2, Sleep)
ServoExp HSsv(pin::HS_SRV, SRV_MIN, SRV_MAX);        //Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp COsv(pin::CO_SRV, SRV_MIN, SRV_MAX);        //Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
//DRV8825 SGst(STP_STEPS, pin::STP_DIR, pin::STP_STP, pin::STP_SLP, pin::STP_M0, pin::STP_M1, pin::STP_M2);    //Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin)
A4988 SGst(STP_STEPS, pin::STP_DIR, pin::STP_STP, pin::STP_M0, pin::STP_M1, pin::STP_M2);    //Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
Sensor::Endstops WGes(pin::WG_ES_B, pin::WG_ES_T, ES_TRIGGER);   //Endschalter Weight (BottomPin, TopPin, TriggerState)
Sensor::Endstops SLes(pin::SL_ES_L, pin::SL_ES_R, ES_TRIGGER);   //Endschalter Slider (LeftPin, RightPin, TriggerState)                          
Sensor::HallSwitch HRha(pin::HR_HALL, HALL_TRIGGER);                  //Hall-Sensor Hammerwheel (Hallpin, TriggerState)
Sensor::HallSwitch SGha(pin::SG_HALL, HALL_TRIGGER);                  //Hall-Sensor Sign (Hallpin, TriggerState)


int n = 0;
unsigned long ctime = 0;    
unsigned long itime = 0;    //Used for running interval

enum stateEnum : uint8_t {IDLE = 0, RUN = 1, RESET = 2};  //States for state machine
stateEnum currentState = IDLE;              //Current state of state machine
stateEnum lastState = IDLE;                 //Last state of state machine
bool firstExec = 1;                         //Used for first execution of state

void inline demoError(){
  Serial.println("DEMO-ERROR!");
  while(true){
    digitalWrite(pin::GO_LED, HIGH);
    delay(200);
    digitalWrite(pin::GO_LED, LOW);
    delay(200);
  }
}

namespace serv{
    void couple();      //coupling
    void decouple();    //decoupling
    void hammerstop();  //engaging Hammerstop
    void hammergo();    //releasing Hammerstop
}

namespace step{
    void pos1();        //Sign is going to position 1. The position is defined by a magnet on the sign.
    void pos2();        //Sign is going to position 2. The position is determined by steps.
    void pos3();        //Sign is going to position 3. The position is determined by steps.
}

namespace states{
  void idle();
  void run();
  void reset(); 
}

void dloop();
void dsetup();

stateEnum initState();
void stateLogic();
stateEnum stateTransition();
bool ReachedTimer();


void setup() {
  
  //* Serial-Setup:
  Serial.begin(SERIAL_BPS);           //Starts Serial Connection between Arduino and PC via USB -> Baudrate = Bits per second (Bps)
  Serial.println("Serial connection established.");
  
  //* Go-Button-Setup:
  pinMode(pin::GO_BUT, INPUT_PULLUP);      //Sets Go-Button to input with pullup resistor
  pinMode(pin::GO_LED, OUTPUT);            //Sets Go-LED to output

  Serial.print(SLes.read());
 // dsetup();
  
}

void loop() {
  //demoError();
  Serial.print("HALL HR: "); Serial.println(HRha.read());
  Serial.print("HALL SG: "); Serial.println(SGha.read());
  delay(1000);
  //dloop();
}

void dsetup(){
  //* Servo-Setup:
  Serial.println("Attaching Servos.");
  HSsv.attach();                          //Initializes Servo Hammerstop
  COsv.attach();                          //Initializes Servo Kupplung
  if(HSsv.attached() && COsv.attached()) Serial.println("Servos attached.");
  else Serial.println("Servos not attached.");  //TODO: Throw error
  
  //* State-Setup:
  currentState = initState();             //Sets currentState to initState
  
  //* Sign-Stepper Setup:
  SGst.begin(STP_RPM, STP_MODE);
}

void dloop(){   //!For prototyping - delete if not needed anymore
  stateLogic();
  currentState = stateTransition();
}

stateEnum initState(){
  if(WGes.read() == Weight::TOP && SLes.read() == Slider::RIGHT){   //Checks which endstops might be triggered.
    Serial.println("Weight is top and Slider is right.");
    //Checks if Hammerstop is engaged and releases it if needed.
    if(HSsv.read() - HS::OFF > HS::RANGE){
      Serial.println("Hammerstop is engaged. Releasing it.");
      serv::hammergo();         
    }
    //Checks if Servo is decoupled and couples it if needed. 
    if(abs(COsv.read() - COUP::EN) > COUP::RANGE){   
      Serial.println("Servo is decoupled. Coupling it.");
      serv::couple();         
    }
    Serial.println("Everything in Position -> IDLE.");
    return IDLE;
  }
  else {
    Serial.println("Not in starting position -> RESET.");
    return RESET;
  }
  firstExec = 1;
}

void stateLogic(){

  if(lastState != currentState) firstExec = 1;
  switch(currentState){
    case IDLE:
      states::idle();
      break;
    case RUN:
      states::run();
      break;
    case RESET:
      states::reset();
      break;
  lastState = currentState;
  }
}
stateEnum stateTransition(){
  if(SLes.read() == Slider::LEFT || WGes.read() == Weight::BOTTOM) return RESET;
  else if(!digitalRead(pin::GO_BUT)) return RUN;
  else if((SLes.read() == Slider::RIGHT && WGes.read() == Weight::TOP)) return IDLE; //TODO: Add Interval condition
  else return currentState;
}

bool ReachedTimer(){
  if(millis() - itime >= INTERVAL){
    Serial.println("Interval reached. Going to IDLE.");
    return true;
  }
  else return false;
}

//================================================

void states::idle() {
  if(firstExec){
    Serial.println("\nIDLE:");
    Serial.println("Sign to position 1.");
    step::pos1();
    Serial.println("Braking DC-Motors.");
    HRdc.brake();
    SLdc.brake();
    Serial.println("IDLE. Disabling all drivers.");
    HRdc.disable();
    SLdc.disable();
    SGst.disable();
    Serial.print("Waiting for Start-Button.");
    digitalWrite(pin::GO_LED, HIGH);
    ctime = millis();
    firstExec = 0;
  }
  if(millis() - ctime >= DELAY){
    Serial.print(".");
    ctime = millis();
  }

}

//------------------------------------------------

void states::run() {
  if(firstExec){
    digitalWrite(pin::GO_LED, LOW);
    Serial.println("\nRUN:");
    Serial.println("Enabling all drivers.");
    HRdc.enable();
    SLdc.enable();
    SGst.enable();
    step::pos2();
    Serial.println("Running motors.");
    HRdc.run(HR::SPEED);
    SLdc.run(SL::SPEED);
    itime = millis();
    ctime = millis();
    firstExec = 0;
  }
  if(millis() - ctime >= DELAY){
    Serial.print(".");
    ctime = millis();
  }
}

//------------------------------------------------
//TODO: Add sign stepper
void states::reset() {
  Serial.println("\nRESET:");
  //* 1. braking motors, because atleast one endstop is reached.
  Serial.println("Braking Motors.");
  HRdc.brake();
  SLdc.brake();
  //* 2. Decoupling and engaging Hammerstop.
  serv::decouple();
  serv::hammerstop();
  //* 3. Checking for endstops:
  if(SLes.read() == Slider::LEFT){    //Slider is left, so a full reset of both weight and slider is needed.
    Serial.println("Slider is left. Going to full reset.");
    //*4a. Running motors in reverse.
    HRdc.run(HR::RS_SPEED);
    SLdc.run(SL::RS_SPEED);
    bool reachedTop = false;
    bool reachedLeft = false;
    Serial.println("Waiting for Endstops.");
    //*5a. Waiting for Endstops
    ctime = millis();
    while(!reachedTop || !reachedLeft){
      if(WGes.read() == Weight::TOP && !reachedTop){
        reachedTop = true;
        Serial.print("Weight: Top");
        HRdc.brake();
      }
      if(SLes.read() == Slider::RIGHT && !reachedLeft){
        reachedLeft = true;
        Serial.print("Slider: Right");
        SLdc.brake();
      }
      if(millis()-ctime > 500){ ctime = millis(); Serial.print(".");}
      delay(1);
    }
    Serial.println("\nEndstops reached, going to Idle.");
  }
  else{
    Serial.println("Slider didn't reach left. Going to partial reset (only weight).");
    //*4b. Running HR-motor in reverse.
    HRdc.run(HR::RS_SPEED);
    //*5b. Waiting for Endstop
    Serial.println("Waiting for Weight to reach top.");
    ctime = millis();
    while(WGes.read() != Weight::TOP){
      delay(1);
      if(millis()-ctime > 500){ ctime = millis(); Serial.print(".");}
    }
    Serial.println("Weight reached top, motor is braking.");
    HRdc.brake();
  }
  //*6. Coupling and releasing Hammerstop.
  serv::hammergo();
  serv::couple();
  //*7. Going to IDLE  
  currentState = IDLE;
  firstExec = 1;
  return;
}

//------------------------------------------------

namespace serv{

  void decouple(){
    using namespace COUP;
    //* 1. Check if Servo was coupled to far before.
    Serial.println("Checking if Servo.read() > ENGAGED.");
    if(COsv.read() - EN > RANGE) {
      Serial.print("Servo is at Postion: "); Serial.println(COsv.read()); Serial.println(". Trying to get back to normal position.");
      uint8_t n = 0;
      //*2a. if yes, then trying n times to get back to normal coupling position. If it fails, throw error.
      while(true){
        COsv.write(EN);
        delay(DELAY_4);
        if(COsv.read() - EN <= RANGE) break;
        if(n >= TriesBeforeError){
          Serial.print("Couldnt recover Postion after ");
          Serial.print(TriesBeforeError);
          Serial.println(" tries.");
          break;
          //TODO: Throw error if not reached (maybe 5 times)
        }
        n++;
      }
    } 
    //* 2. Decouple
    Serial.println("Decoupling.");
    COsv.write(DIS);
    delay(DELAY_4);
    //* 3. Check if servo ran to position
    Serial.println("Checking if Coupling is disengaged.");
    if(COsv.read() - DIS > RANGE){
      uint8_t n = 0;
      //*4a. if not, then trying n times to get back to normal coupling position. If it fails, throw error.
      while(true){
        COsv.write(DIS);
        delay(DELAY_4);
        if(COsv.read() - DIS <= RANGE) break;
        if(n >= TriesBeforeError){
          Serial.print("Couldnt recover Postion after ");
          Serial.print(TriesBeforeError);
          Serial.println(" tries.");
          break; //TODO: Throw error if not reached (maybe 5 times)
        }
        n++;
      }
    }
    else Serial.println("Coupling is disengaged.");
    return;
  }

  void couple(){
    using namespace COUP;
    //* 1. Couple
    COsv.write(EN);
    //* 2. Check if servo ran to position
    if(abs(COsv.read() - EN) >= RANGE) {
      uint8_t n = 0;
      //*4a. if no, then trying n times to get back to normal coupling position. If it fails, throw error.
      while(true){
        COsv.write(EN);
        delay(DELAY_4);
        if(abs(COsv.read() - EN) <= RANGE) break;
        if(n >= TriesBeforeError){
          Serial.print("Couldnt recover Postion after ");
          Serial.print(TriesBeforeError);
          Serial.println(" tries.");
          break; //TODO: Throw error if not reached (maybe 5 times)
        }
        n++;
      }
    }
    return;
  }
  void hammerstop(){
    using namespace HS;
    //* 1. Check if Hammerstop is already engaged
    Serial.println("Checking if Hammerstop is engaged already.");
    if(abs((HSsv.read() - ON)) <= RANGE){
      Serial.println("ERROR: Hammerstop is already engaged.");
      return; //TODO: Throw error
    }
    // Loop for trying n (5) times before throwing an error.
    Serial.println("Hammerstop is not engaged. Finding correct postion to engage.");
    for(int n = 0; n < 5; n++){
      //* 2. Find position for hammerstop to engage
      HRdc.run(HR::SPEED);
      delay(200);
      ctime = millis();
      while(HRha.read()==HALL_TRIGGER){
        delay(4);
        if(millis() - ctime >= HALL_TIMEOUT){
          Serial.println("ERROR: Timeout Hall (1st flank).");
          break; //TODO: Throw error
        } 
      }
      ctime = millis();
      while(HRha.read()!=HALL_TRIGGER){
        delay(4);
        if(millis() - ctime >= HALL_TIMEOUT){
          Serial.println("ERROR: Timeout Hall in (2nd flank).");
          break; //TODO: Throw error
        }
      }
      Serial.println("Postion found. Braking.");
      //* 3. braking motor at correct position
      //TODO: Finetune the timing. (maybe 100ms)
      HRdc.brake();
      delay(DELAY_2);
      //* 4. Engage Hammerstop
      Serial.println("Engaging Hammerstop.");
      HSsv.write(ON);
      delay(DELAY_4);
      //* 5. Check if Hammerstop is engaged
      Serial.println("Checking if Hammerstop is engaged correctly.");
      if(abs(HSsv.read() - ON) <= RANGE){
        Serial.println("Hammerstop is engaged correctly. DONE.");
        return;
      }
      else if(n < 4) Serial.println("ERROR: Hammerstop is not engaged correctly. Trying again"); 
    }
    return; //TODO: Throw error if not reached (maybe 5 times
  }

  void hammergo(){
    using namespace HS;
    //* 1. Check if hammerstop is already disengaged
    Serial.println("Checking if Hammerstop is disengaged already.");
    if(abs(HSsv.read() - OFF) <= RANGE){
      Serial.println("Hammerstop is already disengaged. DONE.");
      return;     //?No Error needed, cuz there is no damage. Maybe throw warning.
    } 
    // Loop for trying n (5) times before throwing an error.
    Serial.println("Hammerstop is engaged. Disengaging.");
    HSsv.write(OFF);
    delay(DELAY_4);
    if(HSsv.read() - OFF <= RANGE){ 
      for(int n = 0; n < 5; n++){
        //* 2. Disengage Hammerstop
        HSsv.write(OFF);
        delay(DELAY_4);
        //* 3. Check if Hammerstop is disengaged (now)
        Serial.println("Checking if Hammerstop is disengaged correctly.");
        if(HSsv.read() - OFF <= RANGE) break; 
        else if(n < 4) Serial.println("ERROR: Hammerstop is not disengaged correctly. Trying again");
      }
    }
    //TODO: Throw error if not reached in tries.
    //* 4. Run motor for a short period of time to release flap (sometimes the flap is getting stuck)
    Serial.println("Running Motor to realease flap.");
    HRdc.run(HR::SPEED);
    delay(100);
    HRdc.brake();
    Serial.println("Hammerstop is disengaged. DONE.");
    return;
  }

} //namespace Servo

namespace step{

  uint8_t pos = 1;

  void pos1(){
    while(SGha.read() != HALL_TRIGGER){
      SGst.move(1);
    }
    pos = 1;
  }

  void pos2(){
    if(pos == 1) SGst.move(STP_POS);
    else if(pos == 3) SGst.move(-STP_POS);
    pos = 2;
  }

  void pos3(){
    if(pos == 2) SGst.move(STP_POS);
    else if(pos == 1) SGst.move(-STP_POS);
    pos = 3;
  }
}