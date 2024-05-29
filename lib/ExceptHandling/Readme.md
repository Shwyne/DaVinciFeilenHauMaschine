# Excepthandling.hpp

## Description
The `Excepthandling.hpp` file provides exception handling functionality for this machine. The file contains a class called `StatusClass` that can save and restore a status message about the machine. The status contains of two enum classes `Compstatus` and `FuncGroup` to specify where the exception had occured.

This can help with debugging and error handling while working with the machine.

Also the file contains several functions to interact with the `EEPROM` for saving and restoring the status of the machine even after a power loss.

And for debugging purposes a `printError()` function is provided to print the error message to the serial monitor for accessing the error message.

**Enum Class CompStatus:**
| Name | Value | Description |
| --- | --- | --- |
| `NOT_CONNECTED` | 0 | The component is not connected to the arduino |
| `SUCCESS` | 1 | The component is connected and did the task successfully |
| `TIMEOUT` | 2 | The component did not accomplish the task in the expected timeframe |
| `ERROR` | 3 | The component encountered an error while doing the task |
| `UNDEFINED` | 255 | The component encountered an undefined error |

**Enum Class FuncGroup:**
| Name | Value | Description |
| --- | --- | --- |
| `HW` | 0 | Exceptions regarding the hammerwheel (DC-Motor Hammerwheel, Hallswitch Hammerwheel)|
| `SL` | 1 | Exceptions regarding the slider (DC-Motor Slider, Endstops Slider)|
| `WG` | 2 | Exceptions regarding the weight (DC-Motor Hammerwheel, Endstops Weight)|
| `SG` | 3 | Exceptions regarding the Sign (Stepper Sign, Hallswitch Sign)|
| `UNDEFINED` | 255 | No Component defined |

**Class StatusClass:**

The class `StatusClass` contains the enum classes `Compstatus` and `FuncGroup` as private attributes. The class provides the following functions:
| Return-Type | Function | Input-Parameter | Description |
| --- | --- | --- | --- |
| none | `StatusClass()` | CompStatus status, FuncGroup group | Constructor of the class |
| none | `StatusClass()` | uint8_t status, uint8_t group | Constructor of the class |
| FuncGroup | `getComps()` | none | Returns the `FuncGroup` of the status as a FuncGroup |
| Compstatus | `getStatus()` | none | Returns the `Compstatus` of the status as a Compstatus |
| uint8_t | `getCompsInt()` | none | Returns the `FuncGroup` of the status as a uint8_t |
| uint8_t | `getStatusInt()` | none | Returns the `Compstatus` of the status as a uint8_t |

**PrintError() Function:**

The `printError()` function prints the error message to the serial monitor. It takes a `StatusClass` object as input and prints the error message to the serial monitor.

**EEPROM Functions:**
| Return-Type | Function | Input-Parameter | Description |
| --- | --- | --- | --- |
| void | `writeToEEPROM()` | StatusClass status | Writes the status to the EEPROM |
| bool | `hasErrorEEPROM()` | none | Checks if an error is stored in the EEPROM and returns true if an error is stored |
| void | `clearEEPROM()` | none | Clears the EEPROM (3 bytes starting from address 0 will be set to 0) |
