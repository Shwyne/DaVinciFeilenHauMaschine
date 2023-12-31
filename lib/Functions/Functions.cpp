#include "Functions.hpp"

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
    // If it fails, throw error.
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
    // If it fails, throw error.
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
    // If it fails, throw error.
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
    while (HRha.read() == true) {
      delay(4);
      if (millis() - ctime >= HALL_TIMEOUT) {
        Serial.println("ERROR: Timeout Hall (1st flank).");
        erCode = ErrCode::HW_TIMEOUT;
        return;
      }
    }
    ctime = millis();
    while (HRha.read() != true) {
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
  // flap is getting stuck)
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
  while (SGha.read() == false) {
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


