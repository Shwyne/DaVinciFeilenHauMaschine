#include "ExceptHandling.hpp"

void printError(StatusClass status){
  uint8_t StatusInt = status.getStatusInt();
  uint8_t GroupInt = status.getCompsInt();

  switch(StatusInt){
    case 0:
      Serial.print("NOT_CONNECTED: ");
      break;
    case 1:
      Serial.print("SUCCESS: ");
      break;
    case 2:
      Serial.print("TIMEOUT: ");
      break;
    case 3:
      Serial.print("ERROR: ");
      break;
    default:
      Serial.print("UNDEFINED: ");
      break;
  }

  switch(GroupInt){
    case 0:
      Serial.println("Hammerwheel");
      break;
    case 1:
      Serial.println("Slider");
      break;
    case 2:
      Serial.println("Weight");
      break;
    case 3:
      Serial.println("Sign");
      break;
    default:
      Serial.println("Undefined");
      break;
  }

  return;
}

void writeToEEPROM(StatusClass status){
  if(EEPROM_ENABLED == false){
    return;
  }
  else{
    EEPROM.write(EEPROM_ADDRESS, true);
    EEPROM.write(EEPROM_ADDRESS+1, status.getStatusInt());
    EEPROM.write(EEPROM_ADDRESS+2, status.getCompsInt());
  }
  return;
}

bool hasErrorEEPROM(){
    bool hasError = EEPROM.read(EEPROM_ADDRESS);
    if(hasError == true){
        return true;
    }
    else{
        return false;
    }
}

void clearEEPROM(){
    EEPROM.write(EEPROM_ADDRESS, 0);
    EEPROM.write(EEPROM_ADDRESS+1, 0);
    EEPROM.write(EEPROM_ADDRESS+2, 0);
    return;
}

