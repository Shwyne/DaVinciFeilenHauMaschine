#include "DC_Motor_Driver.hpp" //Driver MP6550 Library
#include "Sensor.hpp"          //Sensor Library
#include "ServoExp.hpp"        //Custom Servo Library -> depends on Servo.h
#include "config.hpp"          //Config file -> Pins, settings, etc.
#include <Arduino.h>           //Arduino Library
#include "Testfunctions.hpp"     //Testfunctions
#include "Functions.hpp"        //Functions
#include <A4988.h>   //Stepper Driver Library
#include <DRV8825.h> //Stepper Driver Library
#include "Error_Handling.hpp" //Error Handling Library

//==========================================================================================================================

DC_Motor_Driver::MP6550 SLdc(pin::SL_IN1, pin::SL_IN2,
                             pin::SL_SLP); // Motor Slider (In1, In2, Sleep)
DC_Motor_Driver::MP6550
    HWdc(pin::HR_IN1, pin::HR_IN2,
         pin::HR_SLP); // Motor Hammerwheel (In1, In2, Sleep)
ServoExp HSsv(pin::HS_SRV, SRV_MIN,
              SRV_MAX); // Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp COsv(pin::CO_SRV, SRV_MIN,
              SRV_MAX); // Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
// DRV8825 SGst(STP_STEPS, pin::STP_DIR, pin::STP_STP, pin::STP_SLP,
// pin::STP_M0, pin::STP_M1, pin::STP_M2);    //Stepper Schild (Interface,
// Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration,
// Hall-Sensor-Pin)
A4988 SGst(STP_STEPS, pin::STP_DIR, pin::STP_STP, pin::STP_M0, pin::STP_M1,
           pin::STP_M2); // Stepper Schild (Interface, Step-Pin, Direction-Pin,
                         // Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin
Sensor::Endstops
    WGes(pin::WG_ES_B, pin::WG_ES_T,
         ES_TRIGGER); // Endschalter Weight (BottomPin, TopPin, TriggerState)
Sensor::Endstops
    SLes(pin::SL_ES_L, pin::SL_ES_R,
         ES_TRIGGER); // Endschalter Slider (LeftPin, RightPin, TriggerState)
Sensor::HallSwitch
    HRha(pin::HR_HALL,
         HALL_TRIGGER); // Hall-Sensor Hammerwheel (Hallpin, TriggerState)
Sensor::HallSwitch
    SGha(pin::SG_HALL,
         HALL_TRIGGER); // Hall-Sensor Sign (Hallpin, TriggerState)

//==========================================================================================================================

ErrCode erCode =
    ErrCode::NO_ERROR; // Error Code for Error Handling. Default: NO_ERROR

                                 // with a short description.

//==========================================================================================================================

uint32_t ctime = 0; // Used for several short use timers
uint32_t itime = 0; // Used for running interval
uint32_t Wtime = 0; // Used for Weight-Timeout calculations
uint32_t Stime = 0; // Used for Slider-Timeout calculations

enum MachineState : uint8_t {
  IDLE,
  RUN,
  RESET,
  ERROR
};                                // States for state machine
MachineState currentState = IDLE; // Current state of state machine
MachineState lastState = IDLE;    // Last state of state machine
bool firstExec = 1;               // Used for first execution of state

MachineState initState(); // Initializes the system. Checks if everything is in
                          // position and throws an error if not.

//==========================================================================================================================

namespace serv {
void couple();     // coupling
void decouple();   // decoupling
void hammerstop(); // engaging Hammerstop
void hammergo();   // releasing Hammerstop
} // namespace serv

namespace step {
void pos1(); // Sign is going to position 1. The position is defined by a magnet
             // on the sign.
void pos2(); // Sign is going to position 2. The position is determined by
             // steps.
void pos3(); // Sign is going to position 3. The position is determined by
             // steps.
} // namespace step

namespace states {
void idle();  // Idle state: Waiting for start button
void run();   // Run state: Running motors till interval is or endstops are
              // reached
void reset(); // Reset state: Resetting the system if an endstop is reached
void error(
    ErrCode erCode); // Error state: Throwing an error and trapping the system
                     // in an endless loop while blinking the go-led (or
                     // changing it to red, if RGB-LED is used)
} // namespace states
//! Just for prototyping - delete in final version
void dloop();
void dsetup();

bool ReachedTimer() {
  if (millis() - itime >= INTERVAL) {
    itime = millis();
    return true;
  }
  return false;
}

//==========================================================================================================================

void setup() {
  
  //* Serial-Setup:
  Serial.begin(SERIAL_BPS); // Starts Serial Connection between Arduino and PC
                            // via USB -> Baudrate = Bits per second (Bps)
  delay(100); // Waits for Serial connection to be established
  Serial.println("\nSerial connection established.");
  Serial.println("Starting Setup.");

  //* Go-Button-Setup:
  pinMode(pin::GO_BUT,
          INPUT_PULLUP);        // Sets Go-Button to input with pullup resistor
  pinMode(pin::GO_LED, OUTPUT); // Sets Go-LED to output
  //dsetup(); //! Replace with code of dsetup() in final version
  Serial.println("Setup finished.");
  Serial.println("------------------------------------");
}

void loop() {
  Serial.print("Waiting for Go-Button.");
  ctime = millis();
  while(digitalRead(pin::GO_BUT) != GO_TRIGGER) {
    delay(10);
    if(millis() - ctime > 500) {
      ctime = millis();
      Serial.print(".");
    }
  }
  Serial.println();
  measure::MagnetsHR(HWdc, HRha, 3, 255);   //3 Turns at 255 speed
  //dloop(); //! Replace with code of dloop() in final version
}

void inline dsetup() { //! For prototyping - delete if not needed anymore and
                       //! move code to setup()
  //* Sign-Stepper Setup:
  SGst.begin(STP_RPM, STP_MODE);
  //* Servo-Setup:
  Serial.println("Attaching Servos.");
  HSsv.attach(); // Initializes Servo Hammerstop
  if (HSsv.attached() == false)
    erCode = ErrCode::HS_NOT_ATTACHED;
  else
    Serial.println("Servo Hammerstop attached.");
  COsv.attach(); // Initializes Servo Kupplung
  if (COsv.attached() == false)
    erCode = ErrCode::COUP_NOT_ATTACHED;
  else
    Serial.println("Servo Coupling attached.");

  if (erCode != ErrCode::NO_ERROR) {
    currentState = initState();
  } else
    currentState = ERROR;
}

void inline dloop() { //! For prototyping - delete if not needed anymore and
                      //! move code
                      //! to loop()

  if (lastState != currentState) {
    firstExec = 1;
    lastState = currentState;
  }
  switch (currentState) {
  case IDLE:
    states::idle();
    if (digitalRead(pin::GO_BUT) == GO_TRIGGER)
      currentState = RUN;
    break;
  case RUN:
    states::run();
    if (SLes.read() == Slider::LEFT || WGes.read() == Weight::BOTTOM)
      currentState = RESET;
    else if (ReachedTimer())
      currentState = IDLE;
    break;
  case RESET:
    states::reset();
    break;
  case ERROR:
    states::error(erCode);
    break;
  }
  if (erCode != ErrCode::NO_ERROR) {
    //printError(erCode);
    currentState = ERROR;
  }
}

//==========================================================================================================================

MachineState initState() {
  if (WGes.read() == Weight::TOP &&
      SLes.read() ==
          Slider::RIGHT) { // Checks which endstops might be triggered.
    Serial.println("Weight is top and Slider is right.");
    // Checks if Hammerstop is engaged and releases it if needed.
    if (HSsv.read() - HS::OFF > HS::RANGE) {
      Serial.println("Hammerstop is engaged. Releasing it.");
      serv::hammergo();
    }
    // Checks if Servo is decoupled and couples it if needed.
    if (abs(COsv.read() - COUP::EN) > COUP::RANGE) {
      Serial.println("Servo is decoupled. Coupling it.");
      serv::couple();
    }
    Serial.println("Everything in Position -> IDLE.");
    return IDLE;
  } else {
    Serial.println("Not in starting position -> RESET.");
    return RESET;
  }
  firstExec = 1;
}

//==========================================================================================================================

namespace states {
void idle() {
  if (firstExec) {
    Serial.println("\nIDLE:");
    Serial.println("Sign to position 1.");
    step::pos1();
    Serial.println("Braking DC-Motors.");
    HWdc.brake();
    SLdc.brake();
    Serial.println("IDLE. Disabling all drivers.");
    HWdc.disable();
    SLdc.disable();
    SGst.disable();
    Serial.print("Waiting for Start-Button.");
    digitalWrite(pin::GO_LED, HIGH);
    ctime = millis();
    firstExec = 0;
  }
  if (millis() - ctime >= DELAY) {
    Serial.print(".");
    ctime = millis();
  }
}

//------------------------------------------------

void run() {
  if (firstExec) {
    digitalWrite(pin::GO_LED, LOW);
    Serial.println("\nRUN:");
    Serial.println("Enabling all drivers.");
    HWdc.enable();
    SLdc.enable();
    SGst.enable();
    step::pos2();
    Serial.println("Running motors.");
    HWdc.run(HR::SPEED);
    SLdc.run(SL::SPEED);
    itime = millis();
    ctime = millis();
    firstExec = 0;
  }
  if (millis() - ctime >= DELAY) {
    Serial.print(".");
    ctime = millis();
  }
}

//------------------------------------------------
// TODO: Add sign stepper
void reset() {
  Serial.println("\nRESET:");
  //* 1. braking motors, because atleast one endstop is reached.
  Serial.println("Braking Motors.");
  HWdc.brake();
  SLdc.brake();
  //* 2. Decoupling and engaging Hammerstop.
  serv::decouple();
  serv::hammerstop();
  //* 3. Checking for endstops:
  if (SLes.read() == Slider::LEFT) { // Slider is left, so a full reset of both
                                     // weight and slider is needed.
    Serial.println("Slider is left. Going to full reset.");
    //*4a. Running motors in reverse.
    HWdc.run(HR::RS_SPEED);
    SLdc.run(SL::RS_SPEED);
    bool reachedTop = false;
    bool reachedLeft = false;
    Serial.println("Waiting for Endstops.");
    //*5a. Waiting for Endstops
    ctime = millis();
    while (!reachedTop || !reachedLeft) {
      if (WGes.read() == Weight::TOP && !reachedTop) {
        reachedTop = true;
        Serial.print("Weight: Top");
        HWdc.brake();
      }
      if (SLes.read() == Slider::RIGHT && !reachedLeft) {
        reachedLeft = true;
        Serial.print("Slider: Right");
        SLdc.brake();
      }
      if (millis() - ctime > 500) {
        ctime = millis();
        Serial.print(".");
      }
      delay(1);
    }
    Serial.println("\nEndstops reached, going to Idle.");
  } else {
    Serial.println(
        "Slider didn't reach left. Going to partial reset (only weight).");
    //*4b. Running HR-motor in reverse.
    HWdc.run(HR::RS_SPEED);
    //*5b. Waiting for Endstop
    Serial.println("Waiting for Weight to reach top.");
    ctime = millis();
    while (WGes.read() != Weight::TOP) {
      delay(1);
      if (millis() - ctime > 500) {
        ctime = millis();
        Serial.print(".");
      }
    }
    Serial.println("Weight reached top, motor is braking.");
    HWdc.brake();
  }
  //*6. Coupling and releasing Hammerstop.
  serv::hammergo();
  serv::couple();
  //*7. Going to IDLE
  currentState = IDLE;
  firstExec = 1;
  return;
}

void error(ErrCode erCode) {
  //printError(erCode);
  while (true) {
    digitalWrite(pin::GO_LED, HIGH);
    delay(200);
    digitalWrite(pin::GO_LED, LOW);
    delay(200);
  }
}
} // namespace states

