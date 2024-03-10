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


void IdentifyES(){
  Serial.println("Identifying Endstops.");

  uint8_t es_1 = 0;
  uint8_t es_2 = 0;
  while(1){
  if(es_1 != WGes.read()){
    es_1 = WGes.read();
    switch(es_1){
      case 0:
        Serial.println("Weight: Untriggered");
        break;
      case 2:
        Serial.println("Weight: Bottom");
        break;
      case 1:
        Serial.println("Weight: Top");
        break;
    }
    delay(100);
  }
  if(es_2 != SLes.read()){
    es_2 = SLes.read();
    switch(es_2){
      case 0:
        Serial.println("Slider: Untriggered");
        break;
      case 1:
        Serial.println("Slider: Left");
        break;
      case 2:
        Serial.println("Slider: Right");
        break;
    }
    delay(100);
  }
  }
}

namespace step{

void home(){
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
  return;
}

void move(int steps){
  SGst.moveTo(steps);
  SGst.runToPosition();
  return;
}

}

void check(){
  if(ERROR_MANAGEMENT == false){
    return;
  }
  if(erCode != ErrCode::NO_ERROR){
    //TODO: Add write to EEPROM
    printError(erCode);
    while(1){
      Go.updateLED(LED::RED);
      delay(500);
      Go.updateLED(LED::OFF);
      delay(500);
    }
  }
  return;
}

void printError(ErrCode erCode) {
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
  case ErrCode::STPDRV_NOT_CONNECTED:
    Serial.println("CONNECTION-ERROR: Stepper-Driver");
    break;
  case ErrCode::WG_OVERCURRENT:
    Serial.println("OVERCURRENT: Weight");
    break;
  case ErrCode::SL_OVERCURRENT: 
    Serial.println("OVERCURRENT: Slider");
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