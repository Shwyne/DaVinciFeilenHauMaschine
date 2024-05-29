#### Overview

The `sensors.hpp` file contains the declaration of various sensor classes used in the DaVinciFeilenHauMaschine project.

#### Table of Contents
- [Hall-Switch Class](#hall-switch-class)
- [Endstop Class](#endstop-class)
- [Button Class](#button-class)

### Hall-Switch Class

The class `HallSwitch` stores data regarding a hall-switch sensor. It stores the following `attributes`:

| Datatype | Name | Description |
|------|------|-------------|
| uint8_t | `pin` | The pin number of the hall-switch sensor. |
| bool | `detectsMF` | True, if the last read of the sensor detected a magnetic field |
| bool | `MagnetSignal` | Defines if the sensor creates `HIGH` or `LOW` while detecting a magnetic field strong enough to trigger |

Also it comes with the following `methods`:

| Output | Method | Input | Description |
|--------|--------|-------|-------------|
| none | `HallSwitch` | uint8_t pin, bool MagnetSignal | Constructor of the class |
| bool | `read` | none | Reads the current state of the hall-switch sensor, true if magnet |
| bool | `changed` | none | Returns true if the state of the hall-switch sensor has changed |

The Definitions can be found in `HallSwitch.cpp`.

### Endstop Class

The class `Endstop` stores data regarding an endstop sensor. It stores the following `attributes`:

| Datatype | Name | Description |
|------|------|-------------|
| uint8_t | `pin1` | The pin number of the first endstop sensor. |
| uint8_t | `pin2` | The pin number of the second endstop sensor. |
| uint8_t | `state` | The last state of the endstop sensor. (0 = no endstop, 1 = first endstop, 2 = second endstop) |

Also it comes with the following `methods`:

| Output | Method | Input | Description |
|--------|--------|-------|-------------|
| none | `Endstops` | uint8_t pin1, uint8_t pin2 | Constructor of the class |
| uint8_t | `read` | none | Reads the current state of the endstop sensor and returns the state as uint8_t (see attribute state)|
| bool | `changed` | none | Returns true if the state of the endstop sensor has changed |

The Definitions can be found in `Endstops.cpp`.

### Button Class

The class `Button` stores data regarding a RGB-button. It stores the following `attributes`:

| Datatype | Name | Description |
|------|------|-------------|
| uint8_t | `Butpin` | The pin number of the button. |
| uint8_t | `RPin` | The pin number of the red LED. |
| uint8_t | `GPin` | The pin number of the green LED. |
| uint8_t | `BPin` | The pin number of the blue LED. |
| bool | `isPressed` | The last state of the button. |

Also it comes with the following `methods`:

| Output | Method | Input | Description |
|--------|--------|-------|-------------|
| bool | `read` | none | Reads the current state of the button and returns true if pressed |
| bool | `changed` | none | Returns true if the state of the button has changed |
| none | `waitForPress` | none | Waits until the button is pressed AND released again |
| none | `updateLED` | uint8_t mode | Updates the LED of the button according to the mode (0 = off, 1 = red, 2 = green, 3 = blue, 4 = yellow, 5 = cyan, 6 = magenta, 7 = white) |
| none | `updateLED` | uint8_t red, uint8_t green, uint8_t blue | Updates the LED of the button according to the RGB values (needs `PWM` pins)! |
| none | `Button` | uint8_t Butpin, uint8_t RPin, uint8_t GPin, uint8_t BPin | Constructor of the class |

The Definitions can be found in `Button.cpp`.