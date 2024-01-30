#include <Arduino.h>            //Arduino Library
#include "AllFunctions.hpp"    //All Functions including all custom libraries

//==========================================================================================================================
//*Objects:
DC_Motor_Driver::MP6550 SLdc(pin::SL_IN1, pin::SL_IN2,pin::SL_SLP); // Motor Slider (In1, In2, Sleep)
DC_Motor_Driver::MP6550 HWdc(pin::HR_IN1, pin::HR_IN2,pin::HR_SLP); // Motor Hammerwheel (In1, In2, Sleep)
ServoExp HSsv(pin::HS_SRV, HS::MIN, HS::MAX); // Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp COsv(pin::CO_SRV, COUP::MIN, COUP::MAX); // Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
//DRV8825 SGst(STP_STEPS, pin::STP_DIR, pin::STP_STP);
A4988 SGst(STP::STEPS, pin::STP_DIR, pin::STP_STP, pin::STP_M0, pin::STP_M1,pin::STP_M2); // Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
Sensor::Endstops WGes(pin::WG_ES_B, pin::WG_ES_T,Weight::TRIGGER); // Endschalter Weight (BottomPin, TopPin, TriggerState)
Sensor::Endstops SLes(pin::SL_ES_L, pin::SL_ES_R,Slider::TRIGGER); // Endschalter Slider (LeftPin, RightPin, TriggerState)
Sensor::HallSwitch HRha(pin::HR_HALL,HALL::TRIGGER); // Hall-Sensor Hammerwheel (Hallpin, TriggerState)
Sensor::HallSwitch SGha(pin::SG_HALL,HALL::TRIGGER); // Hall-Sensor Sign (Hallpin, TriggerState)
Sensor::Button Go(pin::GO_BUT,pin::GO_LED_R,pin::GO_LED_G,pin::GO_LED_B); // Go-Button (Pin, TriggerState)


//==========================================================================================================================
//*Error Handling:
ErrCode erCode = ErrCode::NO_ERROR; // Error Code for Error Handling. Default: NO_ERROR with a short description.

//==========================================================================================================================
//*Output:
uint32_t ctime = 0; // Used for several short use timers

//==========================================================================================================================
//*State-Machine:
enum MachineState : uint8_t {
  IDLE,
  RUN,
  RESET,
  ERROR
};                                // States for state machine

MachineState currentState = IDLE; // Current state of state machine
MachineState lastState = IDLE;    // Last state of state machine
bool firstExec = 1;               // Used for first execution of state


//==========================================================================================================================
//*PROTOTYPE
void inline dsetup();
void inline demorun();
void inline dloop();
//! Delete if not needed anymore and migrate code to loop() and setup()

//==========================================================================================================================
//*Setup:
void setup() {
  
  //* Serial-Setup:
  Serial.begin(SERIALSET::BAUTRATE); // Starts Serial Connection between Arduino and PC via USB -> Baudrate = Bits per second (Bps)
  delay(100); // Waits for Serial connection to be established
  Serial.println("\nSerial connection established.");
  dsetup(); //! Replace with code of dsetup() in final version
}
//*LOOP:

void loop() {
  demorun();
}


//==========================================================================================================================
//*DEFAULT LOOP:
void inline dloop() { //! For prototyping - delete if not needed anymore and move code to loop()

  if (lastState != currentState) {
    firstExec = 1;
    lastState = currentState;
  }
  switch (currentState) {
  case IDLE:
    states::idle(firstExec);
    if (erCode != ErrCode::NO_ERROR)
      currentState = RUN;
    break;
  case RUN:
    states::run(firstExec);
    if (erCode != ErrCode::NO_ERROR)
      currentState = RESET;
    break;
  case RESET:
    states::reset();
    currentState = IDLE;
    break;
  case ERROR:
    states::error(erCode);
    break;
  }
  if (erCode != ErrCode::NO_ERROR) {
    currentState = ERROR;
  }
}

void inline dsetup() { //! For prototyping - delete if not needed anymore and move code to setup()
  // Sign-Stepper Setup:
  SGst.begin(STP::RPM, STP::MODE);
  // Servo-Setup:
  Serial.println("Attaching Servos.");
  HSsv.attach(); // Initializes Servo Hammerstop
  COsv.attach(); // Initializes Servo Kupplung
  HSsv.write(HS::OFF);
  COsv.write(COUP::EN);
  /*digitalWrite(pin::STP_SLP, HIGH);
  digitalWrite(pin::STP_RST, HIGH);
  //step::pos1();
  HSsv.setTolerance(HS::TOLERANCE);
  COsv.setTolerance(COUP::TOLERANCE);*/
 /* if (erCode != ErrCode::NO_ERROR) {
    currentState = initState();
  } else
    currentState = ERROR;*/
  //currentState = initState();
}

//*DEMORUN: 
void inline demorun(){
  if(SLes.read() == Slider::LEFT){
    //step::pos3();
    Go.updateLED(LED::YELLOW);
    serv::decouple();
    serv::hammerstop();
    delay(200);
    bool reachedTop = true;
    if(WGes.read() != Weight::TOP){
      HWdc.run(HW::RS_SPEED);
      reachedTop = false;
    }
    SLdc.run(SL::RS_SPEED);
    bool reachedRight = false;
    while(!reachedRight || !reachedTop){
      if(WGes.read() == Weight::TOP && !reachedTop){
        reachedTop = true;
        HWdc.brake();
      }
      if(SLes.read() == Slider::RIGHT && !reachedRight){
        reachedRight = true;
        SLdc.brake();
      }
      delay(1);
    }
    HWdc.brake();
    SLdc.brake();
    serv::hammergo();
    serv::couple();
  }
  else if(WGes.read() != Weight::TOP){
   // step::pos3();
    Go.updateLED(LED::YELLOW);
    serv::decouple();
    serv::hammerstop();
    delay(200);
    HWdc.run(HW::RS_SPEED);
    while(WGes.read() != Weight::TOP){
      delay(1);
    }
    HWdc.brake();
    serv::hammergo();
    serv::couple();
  }
  Go.updateLED(LED::GREEN);
  Serial.println("Go-Button!");
  //step::pos1();
  while(Go.read() == false){
    delay(1);
    }
  
  //step::pos2();
  Go.updateLED(LED::CYAN);
  HWdc.run(HW::SPEED);
  SLdc.run(SL::SPEED);
  while((WGes.read() != Weight::BOTTOM) && (SLes.read() != Slider::LEFT)){
    delay(1);
  }
  HWdc.brake();
  SLdc.brake();
  delay(1000);
}

