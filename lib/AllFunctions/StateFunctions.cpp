#include "AllFunctions.hpp"



//==========================================================================================================================
//*STATES:
namespace states {

//*IDLE:
void idle(bool firstExec) {
  if (firstExec) {
    Go.updateLED(LED::GREEN);
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
    ctime = millis();
    firstExec = 0;
  }
  if ((millis() - ctime) >= (SERIALSET::DELAY)) {
    Serial.print(".");
    ctime = millis();
  }
}

//===========================================================
void run(bool firstExec) {
  if (firstExec) {
    Go.updateLED(LED::BLUE);
    Serial.println("\nRUN:");
    Serial.println("Enabling all drivers.");
    HWdc.enable();
    SLdc.enable();
    SGst.enable();
    step::pos2();
    Serial.println("Running motors.");
    HWdc.run(HW::SPEED);
    SLdc.run(SL::SPEED);
    ctime = millis();
    firstExec = 0;
  }
  if (millis() - ctime >= SERIALSET::DELAY) {
    Serial.print(".");
    ctime = millis();
  }
}

//===========================================================
//*RESET:
void reset() {
  Go.updateLED(LED::YELLOW);
  // TODO: Add sign stepper
  Serial.println("\nRESET:");
  // 1. braking motors, because atleast one endstop is reached.
  Serial.println("Braking Motors.");
  HWdc.brake();
  SLdc.brake();
  // 2. Decoupling and engaging Hammerstop.
  serv::decouple();
  serv::hammerstop();
  // 3. Checking for endstops:
  if (SLes.read() == Slider::LEFT) { // Slider is left, so a full reset of both
                                     // weight and slider is needed.
    Serial.println("Slider is left. Going to full reset.");
    //4a. Running motors in reverse.
    HWdc.run(HW::RS_SPEED);
    SLdc.run(SL::RS_SPEED);
    bool reachedTop = false;
    bool reachedLeft = false;
    Serial.println("Waiting for Endstops.");
    //5a. Waiting for Endstops
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
    //4b. Running HR-motor in reverse.
    HWdc.run(HW::RS_SPEED);
    //5b. Waiting for Endstop
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
  //6. Coupling and releasing Hammerstop.
  serv::hammergo();
  serv::couple();
  //7. Going to IDLE
  return;
}
//===========================================================
//*ERROR:
void error(ErrCode erCode) {
  printError(erCode);
  while (true) {
    Go.updateLED(LED::RED);
    delay(500);
    Go.updateLED(LED::OFF);
    delay(500);
  }
}
}