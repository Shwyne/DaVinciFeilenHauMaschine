#include "ExceptHandling.hpp"

// Function to print error message
void printError(StatusClass status){
  uint8_t StatusInt = status.getStatusInt();  // Get status integer from status object for switch case
  uint8_t GroupInt = status.getCompsInt();    // Get component integer from status object for switch case

  switch(StatusInt){
    case 0:
      Serial.print("NOT_CONNECTED: "); // Print error message for NOT_CONNECTED status
      break;
    case 1:
      Serial.print("SUCCESS: "); // Print error message for SUCCESS status
      break;
    case 2:
      Serial.print("TIMEOUT: "); // Print error message for TIMEOUT status
      break;
    case 3:
      Serial.print("ERROR: "); // Print error message for ERROR status
      break;
    default:
      Serial.print("UNDEFINED: "); // Print error message for UNDEFINED status
      break;
  }

  switch(GroupInt){
    case 0:
      Serial.println("Hammerwheel"); // Print component name for Hammerwheel group
      break;
    case 1:
      Serial.println("Slider"); // Print component name for Slider group
      break;
    case 2:
      Serial.println("Weight"); // Print component name for Weight group
      break;
    case 3:
      Serial.println("Sign"); // Print component name for Sign group
      break;
    default:
      Serial.println("Undefined"); // Print component name for Undefined group
      break;
  }

  return;
}

// Function to write error to EEPROM
void writeToEEPROM(StatusClass status){
  if(EEPROM_ENABLED == false){
    return; // If EEPROM is not enabled, return without writing
  }
  else{
    EEPROM.write(EEPROM_ADDRESS, true); // Write true to EEPROM address to indicate error
    EEPROM.write(EEPROM_ADDRESS+1, status.getStatusInt()); // Write status integer to EEPROM address
    EEPROM.write(EEPROM_ADDRESS+2, status.getCompsInt()); // Write component integer to EEPROM address
  }
  return;
}

// Function to read error from EEPROM
bool hasErrorEEPROM(){
    bool hasError = EEPROM.read(EEPROM_ADDRESS); // Read EEPROM address to check if error is present
    if(hasError == true){
        return true; // Return true if error is present
    }
    else{
        return false; // Return false if error is not present
    }
}

// Function to clear error from EEPROM
void clearEEPROM(){
    EEPROM.write(EEPROM_ADDRESS, 0); // Clear EEPROM address
    EEPROM.write(EEPROM_ADDRESS+1, 0); // Clear EEPROM address
    EEPROM.write(EEPROM_ADDRESS+2, 0); // Clear EEPROM address
    return;
}
