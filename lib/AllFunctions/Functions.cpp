#include "AllFunctions.hpp"

//------------------------------------------------

namespace serv {

void decouple() {
  if(DEBUG>1) Serial.println("DECOUPLE: Starts");
  COsv.run(COUP::DIS);
  ctime = millis();
  while(COsv.reachedTarget() == false){
    if(millis() - ctime > COUP::TIMEOUT){
      erCode = ErrCode::COUP_NOT_IN_POS;
      if(ERROR_MANAGEMENT) return;
    }
    delay(1);
  }
  if(DEBUG>1) Serial.println("DECOUPLE: Done");
  return;
}

void couple() {
  if(DEBUG>1) Serial.println("COUPLE: Starts");
  COsv.run(COUP::EN);
  ctime = millis();
  while(COsv.reachedTarget() == false){
    if(millis() - ctime > COUP::TIMEOUT){
      erCode = ErrCode::COUP_NOT_IN_POS;
      if(ERROR_MANAGEMENT) return;
    }
    delay(1);
  }
  if(DEBUG>1) Serial.println("COUPLE: Done");
  return;
}

void hammerstop() {
  //Debug Message
  if(DEBUG>1) Serial.println("HAMMERSTOP: Starts");
  //If Servo not attached -> attach
  if(HSsv.attached() == false){
    HSsv.attach();
  }
  //Check if Servo is not in Off Position
  if(abs(HSsv.read() - HS::OFF) > HS::TOLERANCE){
    //If Servo not in Off (+ Tolerance) try to run to Off
    HSsv.run(HS::OFF);
    delay(400);
    //Check if that worked
    if(HSsv.reachedTarget() == false){
      //If not, error code -> Error Management
      erCode = ErrCode::HS_NOT_IN_POS;
      if(ERROR_MANAGEMENT) return;
    }
  }
  //Start Motor to find engage position
  HWdc.run(HW::SPEED);
  //Set timer for Timeout Error Management
  ctime = millis();
  //Wait till Hall-Sensor detects Magnet
  while(HWha.read() == true){
    delay(1);
    if(millis() - ctime > HW::TIMEOUT){
      //TimeOut Error-Handling -> millis() - ctime = deltaT
      erCode = ErrCode::HW_TIMEOUT;
      if(ERROR_MANAGEMENT) return;
    }
  }
  //Wait till magnet isnt detected anymore
  while(HWha.read() != true){
    delay(1);
    if(millis() - ctime > HW::TIMEOUT){
      erCode = ErrCode::HW_TIMEOUT;
      if(ERROR_MANAGEMENT) return;
    }
  }
  //Delay to match position perfectly
  delay(450); //TODO: Calculate value based on RPM of Hammerwheel motor
  HWdc.brake();
  delay(100);
  //Motor in postion -> engage hammerstop
  HSsv.run(HS::ON);
  //Reset timer for Timeout Error Management
  ctime = millis();
  //Wait for Servo to reach Target (+ Tolerance)
  while(HSsv.reachedTarget() == false){
    delay(1);
    if(millis() - ctime > HS::TIMEOUT){
      //If delta T > Timeout -> Error
      erCode = ErrCode::HS_NOT_IN_POS;
      if(ERROR_MANAGEMENT) return;
    }
  }
  if(DEBUG>1) Serial.println("HAMMERSTOP: Done");
  //delay to make sure the next action starts properly
  delay(300);
  return;
}

void hammergo() {
  if(DEBUG>1) Serial.println("HAMMERGO: Starts");
  if(HSsv.attached() == false){
    HSsv.attach();
  }
  using namespace HS;
  HSsv.run(OFF);
  ctime = millis();
  while(HSsv.reachedTarget() == false){
    if(millis() - ctime > HS::TIMEOUT){
      erCode = ErrCode::HS_NOT_IN_POS;
      if(ERROR_MANAGEMENT) return;
    }
    delay(1);

  }
  delay(100);
  HWdc.run(HW::SPEED);
  delay(50);
  HWdc.brake();
  if(DEBUG>1) Serial.println("HAMMERGO: Done");
  return;
}

} // namespace serv

namespace step{

void setMicroSteps(uint8_t microSteps){
  switch(microSteps){
    case 1:
      digitalWrite(pin::STP_M0, LOW);
      digitalWrite(pin::STP_M1, LOW);
      digitalWrite(pin::STP_M2, LOW);
      break;
    case 2:
      digitalWrite(pin::STP_M0, HIGH);
      digitalWrite(pin::STP_M1, LOW);
      digitalWrite(pin::STP_M2, LOW);
      break;
    case 4:
      digitalWrite(pin::STP_M0, LOW);
      digitalWrite(pin::STP_M1, HIGH);
      digitalWrite(pin::STP_M2, LOW);
      break;
    case 8:
      digitalWrite(pin::STP_M0, HIGH);
      digitalWrite(pin::STP_M1, HIGH);
      digitalWrite(pin::STP_M2, LOW);
      break;
    case 16:
      digitalWrite(pin::STP_M0, LOW);
      digitalWrite(pin::STP_M1, LOW);
      digitalWrite(pin::STP_M2, HIGH);
      break;
    case 32:
      digitalWrite(pin::STP_M0, HIGH);
      digitalWrite(pin::STP_M1, HIGH);
      digitalWrite(pin::STP_M2, HIGH);
      break;
    default:
      digitalWrite(pin::STP_M0, LOW);
      digitalWrite(pin::STP_M1, LOW);
      digitalWrite(pin::STP_M2, LOW);
      break;
    }
  return;
}

void home(){
  digitalWrite(pin::STP_SLP, HIGH);
  Serial.println("Homing");
  int homing = -1;
  while(SGha.read() == LOW){
    SGst.moveTo(homing);
    homing--;
    SGst.run();
    delayMicroseconds(500);
    if(STP::MICRO_STEPS * STP::SPR * 2 < abs(homing)){
      erCode = ErrCode::SG_TIMEOUT;
      if(ERROR_MANAGEMENT){
        digitalWrite(pin::STP_SLP, LOW);
        return;
      }
    }
  }
  Serial.println("Homed");
  SGst.setCurrentPosition(0);
  digitalWrite(pin::STP_SLP, LOW);
  return;
}

void move(int steps){
  digitalWrite(pin::STP_SLP, HIGH);
  SGst.moveTo(steps);
  SGst.runToPosition();
  digitalWrite(pin::STP_SLP, LOW);
  return;
}

} // namespace step

void check(){
  if(ERROR_MANAGEMENT == false){
    return;
  }
  if(erCode != ErrCode::NO_ERROR){
    HWdc.brake();
    SLdc.brake();
    writeToEEPROM();
    printError();
    showErrorLED();
  }
  return;
}

void showErrorLED(){
  ctime = millis();
  LED::color lastColor = LED::OFF;
  while(1){
    if(millis() - ctime > ERROR_LED_DELAY){
      if(lastColor == LED::OFF){
        Go.updateLED(LED::RED);
        lastColor = LED::RED;
      }
      else{
        Go.updateLED(LED::OFF);
        lastColor = LED::OFF;
      }
      ctime = millis();
    }
    if(digitalRead(pin::CLEAR_ERROR) == LOW){
      Go.updateLED(LED::MAGENTA);
      clearEEPROM();
      erCode = ErrCode::NO_ERROR;
      delay(2000);
      Go.updateLED(LED::OFF);
      return;
    }
  }
}

void printError() {
  switch (erCode) {
  case ErrCode::NO_ERROR: 
    Serial.println("No Error."); 
    break;
  case ErrCode::HW_TIMEOUT: 
    Serial.println("TIMEOUT: Hammerwheel"); 
    break;
  case ErrCode::SG_TIMEOUT: 
    Serial.println("TIMEOUT: Sign"); 
    break;
  case ErrCode::WG_TIMEOUT: 
    Serial.println("TIMEOUT: Weight"); 
    break;
  case ErrCode::SL_TIMEOUT: 
    Serial.println("TIMEOUT: Slider");
    break;
  case ErrCode::COUP_NOT_ATTACHED: 
    Serial.println("CONNECTION-ERROR: Coupling");
    break;
  case ErrCode::COUP_NOT_IN_POS: 
    Serial.println("POSITION-ERROR: Coupling");
    break;
  case ErrCode::HS_NOT_ATTACHED: 
    Serial.println("CONNECTION-ERROR: Hammerstop");
    break;
  case ErrCode::HS_NOT_IN_POS: 
    Serial.println("POSITION-ERROR: Hammerstop");
    break;
  case ErrCode::UNDEFINED: Serial.println("UNDEFINED ERROR."); 
    break;
  }
}

void writeToEEPROM(){
  if(EEPROM_ENABLED == false){
    return;
  }
  else{
    EEPROM.write(EEPROM_ADDRESS, true);
    EEPROM.write(EEPROM_ADDRESS+1, static_cast<uint8_t>(erCode));
  }
  return;
}

bool hasErrorEEPROM(){
  if(EEPROM_ENABLED == false){
    return false;
  }
  else{
    bool hasError = EEPROM.read(EEPROM_ADDRESS);
    uint8_t ErrorCode = EEPROM.read(EEPROM_ADDRESS+1);
    if(hasError == true){
      erCode = static_cast<ErrCode>(ErrorCode);
      return true;
    }
    else{
      return false;
    }
  }
}

void clearEEPROM(){
  if(EEPROM_ENABLED == false){
    return;
  }
  else{
    EEPROM.write(EEPROM_ADDRESS, false);
    EEPROM.write(EEPROM_ADDRESS+1, 0);
  }
  return;
}