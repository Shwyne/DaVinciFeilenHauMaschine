#include "AllFunctions.hpp"

//------------------------------------------------

namespace serv {

//*Decouple the coupling [HW <-> SL]

StatusClass decouple() {

  if(DEBUG>1) Serial.println("DECOUPLE: Starts");

  uint8_t tries = 0;
  while(COsv.attached() == false){
    COsv.attach();
    tries++;
    if(tries > COUP::ATTACH_TRIES){
      if(DEBUG>1) Serial.println("DECOUPLE: Not attached");
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::NOT_CONNECTED, FuncGroup::CO);
    }
    delay(100); 
  }

  
  COsv.run(COUP::DIS);
  auto waitResult = waitForTarget(COsv, COUP::TIMEOUT);
  if(waitResult == CompStatus::TIMEOUT){
    if(DEBUG>1) Serial.println("DECOUPLE: Not in Pos");
    if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::CO);
  }
  if(DEBUG>1) Serial.println("DECOUPLE: Done");
  return StatusClass(CompStatus::SUCCESS, FuncGroup::CO);
  
}

//*Couple the coupling [HW <-> SL]

StatusClass couple() {

  if(DEBUG>1) Serial.println("COUPLE: Starts");
  
  uint8_t tries = 0;
  while(COsv.attached() == false){
    COsv.attach();
    tries++;
    if(tries > COUP::ATTACH_TRIES){
      if(DEBUG>1) Serial.println("DECOUPLE: Not attached");
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::NOT_CONNECTED, FuncGroup::CO);
    }
    delay(100); 
  }

  COsv.run(COUP::EN);

  auto waitResult = waitForTarget(COsv, COUP::TIMEOUT);
  if(waitResult == CompStatus::TIMEOUT){
    if(DEBUG>1) Serial.println("DECOUPLE: Not in Pos");
    if(ERROR_MANAGEMENT) if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::CO);
  }
  if(DEBUG>1) Serial.println("COUPLE: Done");
  return StatusClass(CompStatus::SUCCESS, FuncGroup::CO);
}


//*Engage the hammerstop

StatusClass hammerstop() {
  //Debug Message
  if(DEBUG>1) Serial.println("HAMMERSTOP: Starts");

  //If Servo not attached -> try to attach (up to 3 times)
  /*uint8_t tries = 0;
  while(HSsv.attached() == false){
    HSsv.attach();
    tries++;
    if(tries > HS::ATTACH_TRIES){
      if(DEBUG>1) Serial.println("HAMMERSTOP: Not attached");
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::NOT_CONNECTED, FuncGroup::HS);
    }
    delay(100);
  }*/

  //Check if Servo is not in Off Position
  /*if(abs(HSsv.read() - HS::OFF) > HS::TOLERANCE){
    //If Servo not in Off (+ Tolerance) try to run to Off
    HSsv.run(HS::OFF);
    delay(400);
    //Check if that worked
    auto waitResult = waitForTarget(HSsv, HS::TIMEOUT);
    if(waitResult != CompStatus::SUCCESS){
      if(DEBUG>1) Serial.println("DECOUPLE: Not in Pos");
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::HS);
    }
  }*/
  //Start Motor to find engage position
  HWdc.run(HW::SPEED);
  //Set timer for Timeout Error Management
  //uint32_t ctime = millis();
  //Wait till Hall-Sensor detects Magnet
  while(HWha.read() == true){
    delay(1);
    /*if(millis() - ctime > HW::TIMEOUT){
      //TimeOut Error-Handling -> millis() - ctime = deltaT
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
    }*/
  }
  //Wait till magnet isnt detected anymore
  while(HWha.read() != true){
    delay(1);
    /*if(millis() - ctime > HW::TIMEOUT){
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::HW);
    }*/
  }
  //Delay to match position perfectly
  delay(450); //TODO: Calculate value based on RPM of Hammerwheel motor
  HWdc.brake();
  delay(100);
  //Motor in postion -> engage hammerstop
  HSsv.run(HS::ON);
  /*auto waitResult = waitForTarget(HSsv, HS::TIMEOUT);
  if(waitResult == CompStatus::TIMEOUT){
      if(DEBUG>1) Serial.println("DECOUPLE: Not in Pos");
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::HS);
    }
  if(DEBUG>1) Serial.println("HAMMERSTOP: Done");*/
  //delay to make sure the next action starts properly
  delay(300);
  return StatusClass(CompStatus::SUCCESS, FuncGroup::HS);
}

//*Disengage the hammerstop

StatusClass hammergo() {
  if(DEBUG>1) Serial.println("HAMMERGO: Starts");
  //If Servo not attached -> try to attach (up to 3 times)
  /*uint8_t tries = 0;
  while(HSsv.attached() == false){
    HSsv.attach();
    tries++;
    if(tries > HS::ATTACH_TRIES){
      if(DEBUG>1) Serial.println("HAMMERSTOP: Not attached");
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::NOT_CONNECTED, FuncGroup::HS);
    }
    delay(100);
  }*/

  HSsv.run(HS::OFF);
  /*auto waitResult = waitForTarget(HSsv, HS::TOLERANCE);
  if(waitResult == CompStatus::TIMEOUT){
      if(DEBUG>1) Serial.println("DECOUPLE: Not in Pos");
      if(ERROR_MANAGEMENT) return StatusClass(CompStatus::TIMEOUT, FuncGroup::HS);
  }*/
  delay(100);
  HWdc.run(HW::SPEED);
  delay(150);
  HWdc.brake();
  if(DEBUG>1) Serial.println("HAMMERGO: Done");
  return StatusClass(CompStatus::SUCCESS, FuncGroup::HS);
}


//*Wait for the Servo to reach its target

CompStatus waitForTarget(ServoExp srv, uint16_t timeout){
  uint32_t timer = millis();
  srv.attach();
  while(1){
    delay(1);
    if(srv.reachedTarget()){
      return CompStatus::SUCCESS;
    }
    if((millis() - timer > timeout) && ERROR_MANAGEMENT){
      return CompStatus::TIMEOUT;
    }
  }
}

} //? namespace serv


//------------------------------------------------
namespace step{

void setMicroSteps(uint8_t microSteps){
  // Calculate the logarithm base 2 of microSteps, rounding down
  uint8_t log2MicroSteps = (microSteps > 0) ? log2(microSteps) : 0;

  // Calculate the values for M0, M1, and M2
  bool M0 = log2MicroSteps & 1;
  bool M1 = log2MicroSteps & 2;
  bool M2 = log2MicroSteps & 4;

  // Write the values to the pins
  digitalWrite(pin::STP_M0, M0);
  digitalWrite(pin::STP_M1, M1);
  digitalWrite(pin::STP_M2, M2);
}

StatusClass home(){
  digitalWrite(pin::STP_SLP, HIGH);
  Serial.println("Homing");
  int homing = -1;
  while(SGha.read() == LOW){
    SGst.moveTo(homing);
    homing--;
    SGst.run();
    delayMicroseconds(500);
    if(STP::MICRO_STEPS * STP::SPR * 2 < abs(homing)){
      if(ERROR_MANAGEMENT){
        digitalWrite(pin::STP_SLP, LOW);
        return StatusClass(CompStatus::TIMEOUT, FuncGroup::SG);
      }
    }
  }
  Serial.println("Homed");
  SGst.setCurrentPosition(0);
  digitalWrite(pin::STP_SLP, LOW);
  return StatusClass(CompStatus::SUCCESS, FuncGroup::SG);
}

void move(int steps){
  digitalWrite(pin::STP_SLP, HIGH);
  SGst.moveTo(steps);
  SGst.runToPosition();
  digitalWrite(pin::STP_SLP, LOW);
  return;
}

} // namespace step

void ErrorState(StatusClass status){
  auto FuncGroup = status.getComps();
  auto CompStatus = status.getStatus();

  if(CompStatus == CompStatus::SUCCESS){
    if(DEBUG>0) Serial.println("No Error.");
    return;
  }
  //Disable Drivers, stop Motors
  else{
    HWdc.brake();
    SLdc.brake();
    HWdc.sleep();
    SLdc.sleep();
    digitalWrite(pin::STP_EN, LOW);
  }

  const char* compStatusStrings[] = {
    "NOT_CONNECTED: ",
    "SUCCESS: ",
    "TIMEOUT: ",
    "ERROR: ",
    "UNDEFINED: "
  };

  const char* funcGroupStrings[] = {
    "Hammerwheel",
    "Slider",
    "Coupling",
    "Hammerstop",
    "Weight",
    "Sign",
    "Undefined"
  };

  Serial.println(compStatusStrings[static_cast<int>(CompStatus)]);
  Serial.println(funcGroupStrings[static_cast<int>(FuncGroup)]);
  
  writeToEEPROM(status);  
  showErrorLED(); //Endless Loop
}

void showErrorLED(){
  uint32_t ctime = millis();
  uint32_t rtime = 0;
  LED::color LEDColor = LED::RED;
  while(1){
    if(millis() - ctime > ERROR_LED_DELAY){
      Go.updateLED(LEDColor);
      LEDColor = (LEDColor == LED::RED) ? LED::OFF : LED::RED;
      ctime = millis();
    }
    if(digitalRead(pin::CLEAR_ERROR) == LOW){
      Go.updateLED(LED::MAGENTA);
      rtime = millis();
      while(digitalRead(pin::CLEAR_ERROR) == LOW){
        if(millis() - rtime > CLEAR_ERROR_TIME){
          clearEEPROM();
          if(DEBUG>0) Serial.println("EEPROM cleared");
          Go.updateLED(LED::OFF);
          return;
        }
      }
    }
  }
}

void writeToEEPROM(StatusClass status){
  if(EEPROM_ENABLED == false){
    return;
  }
  else{
    EEPROM.write(EEPROM_ADDRESS, true);
    EEPROM.write(EEPROM_ADDRESS+1, static_cast<uint8_t>(status.getStatus()));
    EEPROM.write(EEPROM_ADDRESS+2, static_cast<uint8_t>(status.getComps()));
  }
  return;
}

StatusClass readFromEEPROM(){
  if(EEPROM_ENABLED == false){
    return StatusClass(CompStatus::SUCCESS, FuncGroup::UNDEFINED);
  }
  else{
    bool hasError = EEPROM.read(EEPROM_ADDRESS);
    if(hasError == true){
      uint8_t status = EEPROM.read(EEPROM_ADDRESS+1);
      uint8_t group = EEPROM.read(EEPROM_ADDRESS+2);
      return StatusClass(static_cast<CompStatus>(status), static_cast<FuncGroup>(group));
    }
    else{
      return StatusClass(CompStatus::SUCCESS, FuncGroup::UNDEFINED);
    }
  }
}

bool hasErrorEEPROM(){
  if(EEPROM_ENABLED == false){
    return false;
  }
  else{
    bool hasError = EEPROM.read(EEPROM_ADDRESS);
    if(hasError == true){
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
    EEPROM.write(EEPROM_ADDRESS, 0);
    EEPROM.write(EEPROM_ADDRESS+1, 0);
    EEPROM.write(EEPROM_ADDRESS+2, 0);
  }
  return;
}

uint8_t log2(uint8_t n){
  uint8_t result = 0;
  while(n >>= 1){
    result++;
  }
  return result;
}