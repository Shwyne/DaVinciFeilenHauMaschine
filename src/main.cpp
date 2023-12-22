#include "DC_Motor_Driver.hpp" //Driver MP6550 Library
#include "Sensor.hpp"          //Sensor Library
#include "ServoExp.hpp"        //Custom Servo Library -> depends on Servo.h
#include "config.hpp"          //Config file -> Pins, settings, etc.
#include <Arduino.h>           //Arduino Library

// #include "Testfunctions.hpp"     //Testfunctions
#include <A4988.h>   //Stepper Driver Library
#include <DRV8825.h> //Stepper Driver Library


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

enum class ErrCode : uint8_t {
  NO_ERROR,   // No Error
  HW_TIMEOUT, // Timeout Hammerwheel: Occurs if the Hall cant detect the magnets
              // for engaging the hammerstop. Hardware to check: Hall-Sensor,
              // DC-Motor, DC-Driver, Arduino, Connections
  SG_TIMEOUT, // Timeout Sign: Occurs if the Hall cant detect the magnets for
              // nulling the sign-stepper. Hardware to check: Hall-Sensor,
              // Stepper-Driver, Stepper, Arduino, Connections
  WG_TIMEOUT, // Timeout Weight: Occurs if one of the Endstops should've been
              // triggered but is not. Hardware to check: Endstops, DC-Motor,
              // DC-Driver, Arduino, Connections
  SL_TIMEOUT, // Timeout Slider: Occurs if one of the Endstops should've been
              // triggered but is not. Hardware to check: Endstops, DC-Motor,
              // DC-Driver, Arduino, Connections
  STPDRV_NOT_CONNECTED, // Stepper-Driver not connected: The DRV8825 is pulling
                        // the ?-Pin to high. If the driver isnt present the
                        // arduino reads low. Hardware to check: Stepper-Driver,
                        // Arduino, Connections
  WG_OVERCURRENT, // Overcurrent Weight-Motor: Occurs if the current of the
                  // DC-Motor is too high. (Only implemented, if a VISEN-Pin is
                  // given): Hardware to check: DC-Motor, DC-Driver
  SL_OVERCURRENT, // Overcurrent Slider-Motor: Occurs if the current of the
                  // DC-Motor is too high. (Only implemented, if a VISEN-Pin is
                  // given): Hardware to check: DC-Motor, DC-Driver
  COUP_NOT_ATTACHED, // Servo for coupling not attached: Occurs if the servo is
                     // not attached. Hardware to check: Servo, Arduino,
                     // Connections
  COUP_NOT_IN_POS, // Servo for coupling not in expected position: Occurs if the
                   // servo is not in the position it should be. Hardware to
                   // check: Servo, Arduino, Connections
  HS_NOT_ATTACHED, // Servo for hammerstop not attached: Occurs if the servo is
                   // not attached. Hardware to check: Servo, Arduino,
                   // Connections
  HS_NOT_IN_POS, // Servo for hammerstop not in expected position: Occurs if the
                 // servo is not in the position it should be. Hardware to
                 // check: Servo, Arduino, Connections
  UNDEFINED // Undefined Error: Occurs if an error is thrown but not defined in
            // the enum class.
};

ErrCode erCode =
    ErrCode::NO_ERROR; // Error Code for Error Handling. Default: NO_ERROR
void printError(ErrCode erCode); // Prints the error code to the serial monitor
                                 // with a short description.

//==========================================================================================================================

unsigned long ctime = 0; // Used for several short use timers
unsigned long itime = 0; // Used for running interval
unsigned long Wtime = 0; // Used for Weight-Timeout calculations
unsigned long Stime = 0; // Used for Slider-Timeout calculations

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
void idle(); // Idle state: Waiting for start button
void run();  // Run state: Running motors till interval is or endstops are
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

bool ReachedTimer();

//==========================================================================================================================

void setup() {

  //* Serial-Setup:
  Serial.begin(SERIAL_BPS); // Starts Serial Connection between Arduino and PC
                            // via USB -> Baudrate = Bits per second (Bps)
  Serial.println("Serial connection established.");

  //* Go-Button-Setup:
  pinMode(pin::GO_BUT,
          INPUT_PULLUP);        // Sets Go-Button to input with pullup resistor
  pinMode(pin::GO_LED, OUTPUT); // Sets Go-LED to output

  Serial.print(SLes.read());
  dsetup();
}

void loop() { dloop(); }

void dsetup() {
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

void dloop() { //! For prototyping - delete if not needed anymore and move code
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
    printError(erCode);
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
  printError(erCode);
  while (true) {
    digitalWrite(pin::GO_LED, HIGH);
    delay(200);
    digitalWrite(pin::GO_LED, LOW);
    delay(200);
  }
}
} // namespace states

//------------------------------------------------

namespace serv {

void decouple() {
  using namespace COUP;
  //* 1. Check if Servo was coupled to far before.
  Serial.println("Checking if Servo.read() > ENGAGED.");
  if (COsv.read() - EN > RANGE) {
    Serial.print("Servo is at Postion: ");
    Serial.println(COsv.read());
    Serial.println(". Trying to get back to normal position.");
    uint8_t n = 0;
    //*2a. if yes, then trying n times to get back to normal coupling position.
    //If it fails, throw error.
    while (true) {
      COsv.write(EN);
      delay(DELAY_4);
      if (COsv.read() - EN <= RANGE)
        break;
      if (n >= TriesBeforeError) {
        Serial.print("Couldnt recover Postion after ");
        Serial.print(TriesBeforeError);
        Serial.println(" tries.");
        erCode = ErrCode::COUP_NOT_IN_POS;
        return;
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
  if (COsv.read() - DIS > RANGE) {
    uint8_t n = 0;
    //*4a. if not, then trying n times to get back to normal coupling position.
    //If it fails, throw error.
    while (true) {
      COsv.write(DIS);
      delay(DELAY_4);
      if (COsv.read() - DIS <= RANGE)
        break;
      if (n >= TriesBeforeError) {
        Serial.print("Couldnt recover Postion after ");
        Serial.print(TriesBeforeError);
        Serial.println(" tries.");
        erCode = ErrCode::COUP_NOT_IN_POS;
        return;
      }
      n++;
    }
  } else
    Serial.println("Coupling is disengaged.");
  return;
}

void couple() {
  using namespace COUP;
  //* 1. Couple
  COsv.write(EN);
  //* 2. Check if servo ran to position
  if (abs(COsv.read() - EN) >= RANGE) {
    uint8_t n = 0;
    //*4a. if no, then trying n times to get back to normal coupling position.
    //If it fails, throw error.
    while (true) {
      COsv.write(EN);
      delay(DELAY_4);
      if (abs(COsv.read() - EN) <= RANGE)
        break;
      if (n >= TriesBeforeError) {
        Serial.print("Couldnt recover Postion after ");
        Serial.print(TriesBeforeError);
        Serial.println(" tries.");
        erCode = ErrCode::COUP_NOT_IN_POS;
        return;
      }
      n++;
    }
  }
  return;
}
void hammerstop() {
  using namespace HS;
  //* 1. Check if Hammerstop is already engaged
  Serial.println("Checking if Hammerstop is engaged already.");
  if (abs((HSsv.read() - ON)) <= RANGE) {
    Serial.println("ERROR: Hammerstop is already engaged.");
    erCode = ErrCode::HS_NOT_IN_POS;
    return;
  }
  // Loop for trying n (5) times before throwing an error.
  Serial.println(
      "Hammerstop is not engaged. Finding correct postion to engage.");
  for (int n = 0; n < 5; n++) {
    //* 2. Find position for hammerstop to engage
    HWdc.run(HR::SPEED);
    delay(200);
    ctime = millis();
    while (HRha.read() == HALL_TRIGGER) {
      delay(4);
      if (millis() - ctime >= HALL_TIMEOUT) {
        Serial.println("ERROR: Timeout Hall (1st flank).");
        erCode = ErrCode::HW_TIMEOUT;
        return;
      }
    }
    ctime = millis();
    while (HRha.read() != HALL_TRIGGER) {
      delay(4);
      if (millis() - ctime >= HALL_TIMEOUT) {
        Serial.println("ERROR: Timeout Hall in (2nd flank).");
        erCode = ErrCode::HW_TIMEOUT;
        return;
      }
    }
    Serial.println("Postion found. Braking.");
    //* 3. braking motor at correct position
    // TODO: Finetune the timing. (maybe 100ms)
    HWdc.brake();
    delay(DELAY_2);
    //* 4. Engage Hammerstop
    Serial.println("Engaging Hammerstop.");
    HSsv.write(ON);
    delay(DELAY_4);
    //* 5. Check if Hammerstop is engaged
    Serial.println("Checking if Hammerstop is engaged correctly.");
    if (abs(HSsv.read() - ON) <= RANGE) {
      Serial.println("Hammerstop is engaged correctly. DONE.");
      return;
    } else if (n < 4)
      Serial.println(
          "ERROR: Hammerstop is not engaged correctly. Trying again");
  }
  erCode = ErrCode::HS_NOT_IN_POS;
  return;
}

void hammergo() {
  using namespace HS;
  //* 1. Check if hammerstop is already disengaged
  Serial.println("Checking if Hammerstop is disengaged already.");
  if (abs(HSsv.read() - OFF) <= RANGE) {
    Serial.println("Hammerstop is already disengaged. DONE.");
    return; //?No Error needed, cuz there is no damage. Maybe throw warning.
  }
  // Loop for trying n (5) times before throwing an error.
  Serial.println("Hammerstop is engaged. Disengaging.");
  HSsv.write(OFF);
  delay(DELAY_4);
  if (HSsv.read() - OFF <= RANGE) {
    for (int n = 0; n < 5; n++) {
      //* 2. Disengage Hammerstop
      HSsv.write(OFF);
      delay(DELAY_4);
      //* 3. Check if Hammerstop is disengaged (now)
      Serial.println("Checking if Hammerstop is disengaged correctly.");
      if (HSsv.read() - OFF <= RANGE)
        break;
      else if (n < 4)
        Serial.println(
            "ERROR: Hammerstop is not disengaged correctly. Trying again");
      else if (n == 4) {
        erCode = ErrCode::HS_NOT_IN_POS;
        return;
      }
    }
  }
  //* 4. Run motor for a short period of time to release flap (sometimes the
  //flap is getting stuck)
  Serial.println("Running Motor to realease flap.");
  HWdc.run(HR::SPEED);
  delay(100);
  HWdc.brake();
  Serial.println("Hammerstop is disengaged. DONE.");
  return;
}

} // namespace serv

namespace step {

uint8_t pos = 1;

void pos1() {
  ctime = millis();
  while (SGha.read() != HALL_TRIGGER) {
    SGst.move(1);
    if (millis() - ctime >= HALL_TIMEOUT) {
      Serial.println("ERROR: Timeout Hall (1st flank).");
      erCode = ErrCode::SG_TIMEOUT;
      return;
    }
  }
  pos = 1;
}

void pos2() {
  if (pos == 1)
    SGst.move(STP_POS);
  else if (pos == 3)
    SGst.move(-STP_POS);
  pos = 2;
}

void pos3() {
  if (pos == 2)
    SGst.move(STP_POS);
  else if (pos == 1)
    SGst.move(-STP_POS);
  pos = 3;
}
} // namespace step

bool ReachedTimer() {
  if (millis() - itime >= INTERVAL) {
    Serial.println("Interval reached. Going to IDLE.");
    return true;
  } else
    return false;
}

void printError(ErrCode erCode) {
  switch (erCode) {
  case ErrCode::NO_ERROR:
    Serial.println("No Error.");
    break;
  case ErrCode::HW_TIMEOUT:
    Serial.println("TIMEOUT - Hammerwheel: Occurs if the Hall cant detect the "
                   "magnets for engaging the hammerstop. Hardware to check: "
                   "Hall-Sensor, DC-Motor, DC-Driver, Arduino, Connections");
    break;
  case ErrCode::SG_TIMEOUT:
    Serial.println(
        "TIMEOUT - Sign: Occurs if the Hall cant detect the magnets for "
        "nulling the sign-stepper. Hardware to check: Hall-Sensor, "
        "Stepper-Driver, Stepper, Arduino, Connections");
    break;
  case ErrCode::WG_TIMEOUT:
    Serial.println("TIMEOUT - Weight: Occurs if one of the Endstops should've "
                   "been triggered but is not. Hardware to check: Endstops, "
                   "DC-Motor, DC-Driver, Arduino, Connections");
    break;
  case ErrCode::SL_TIMEOUT:
    Serial.println("TIMEOUT - Slider: Occurs if one of the Endstops should've "
                   "been triggered but is not. Hardware to check: Endstops, "
                   "DC-Motor, DC-Driver, Arduino, Connections");
    break;
  case ErrCode::STPDRV_NOT_CONNECTED:
    Serial.println(
        "NOT CONNECTED - Stepper-Driver: The DRV8825 is pulling the ?-Pin to "
        "high. If the driver isnt present the arduino reads low. Hardware to "
        "check: Stepper-Driver, Arduino, Connections");
    break;
  case ErrCode::WG_OVERCURRENT:
    Serial.println("OVERCURRENT - Weight-Motor: Occurs if the current of the "
                   "DC-Motor is too high. (Only implemented, if a VISEN-Pin is "
                   "given): Hardware to check: DC-Motor, DC-Driver");
    break;
  case ErrCode::SL_OVERCURRENT:
    Serial.println("OVERCURRENT - Slider-Motor: Occurs if the current of the "
                   "DC-Motor is too high. (Only implemented, if a VISEN-Pin is "
                   "given): Hardware to check: DC-Motor, DC-Driver");
    break;
  case ErrCode::COUP_NOT_ATTACHED:
    Serial.println(
        "NOT ATTACHED - Servo for coupling : Occurs if the servo is not "
        "attached. Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::COUP_NOT_IN_POS:
    Serial.println("NOT IN POS - Servo for coupling: not in expected position "
                   "- Occurs if the servo is not in the position it should be. "
                   "Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::HS_NOT_ATTACHED:
    Serial.println(
        "NOT ATTACHED - Servo for hammerstop: Occurs if the servo is not "
        "attached. Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::HS_NOT_IN_POS:
    Serial.println("NOT IN POS - Servo for hammerstop: not in expected "
                   "position - Occurs if the servo is not in the position it "
                   "should be. Hardware to check: Servo, Arduino, Connections");
    break;
  case ErrCode::UNDEFINED:
    Serial.println("UNDEFINED ERROR. Check manual for more help.");
    break;
  }
}
