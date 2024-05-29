#### Overview

The `config.hpp` file contains various settings for the DaVinciFeilenHauMaschine project. It also includes some basic calculations that can be done before uploading the firmware to the arduino, e.g. timing calculations.

The `Enumerator.hpp` file contains the declaration of various enumerators used in the DaVinciFeilenHauMaschine project. 

The `pins.hpp` file contains the declaration of all the pins connected to the arduino

#### Table of Contents
- [Config](#config)
- [Enumerator](#enumerator)
- [Pins](#pins)

### Config

The `config.hpp` file contains the following `settings`:

**Generel Settings:**
| Datatype | Setting | Description |
|------|------|-------------|
| bool | `FAN` | If true, the fan will be activated. Make sure the FAN pin is correct |
| uint32_t | `BAUTRATE` | The Baudrate of the Serial Connection -> Device-Manager -> COM-Port -> Properties -> Port Settings -> Bits per second |

**Debug and Error Settings:**
| Datatype | Setting | Description |
|------|------|-------------|
| uint8_t | `DEBUG` | The Debug Level of the Serial Connection (0 = off, 1 = basic, 2 = detailled) |
| bool | `TEST_BUTTON` | If true, the testfunctions need the front button to work properly |
| bool | `TEST_LED` | If true, the testfunctions will use the LED to show the progress/status |
| bool | `ERROR_MANAGEMENT` | If true, the error management will be activated, otherwise an error won't interrupt the machine |
| bool | `EEPROM_ENABLED` | If true, the EEPROM will be scanned for stored errors and will safe errors, if they occur |
| uint16_t | `EEPROM_ADDRESS` | The address where the EEPROM will start storing errors (3 bytes used)
| uint16_t | `ERROR_LED_DELAY` | Defines the blinking interval of the Error-LED in milliseconds |
| uint16_t | `CLEAR_ERROR_TIME` | Defines the length of the Clear-Button press needed for clearing an error in milliseconds |

**Hammerwheel(`HW::`):**
| Datatype | Setting | Description |
|------|------|-------------|
| float | `RPM_MAX` | The maximum RPM of the DC-Motor driving the hammerwheel |
| bool | `REVERSED` | If true, the motor will turn in the opposite direction |
| bool | `AUTO_SLEEP` | If true, the motor will go to sleep after braking and wake up when about to run |
| uint8_t | `SPEED` | The default speed of the motor running forwards (0 to 255 = 0 to 100 %) |
| uint8_t | `RS_SPEED` | The default speed of the motor running backwards (0 to 255 = 0 to 100 %) |
| float | `i` | The gear ratio inbetween the motor and the hammerwheel shaft |
| uint8_t | `nMAGNETS` | The number of magnets on the hammerwheel |
| uint16_t | `HEIGHT_mm` | The height of the weight travelling distance |
| uint16_t | `RADIUS_mm` | The radius of the hammerwheel |

**Slider(`SL::`):**
| Datatype | Setting | Description |
|------|------|-------------|
| float | `RPM_MAX` | The maximum RPM of the DC-Motor driving the slider |
| bool | `REVERSED` | If true, the motor will turn in the opposite direction |
| bool | `AUTO_SLEEP` | If true, the motor will go to sleep after braking and wake up when about to run |
| uint8_t | `RS_SPEED` | The default speed of the motor running backwards (0 to 255 = 0 to 100 %) |
| float | `i` | The gear ratio inbetween the motor and the slider shaft |
| uint16_t | `L_mm` | The length of the slider to move from one endstop to the other |
| uint8_t | `PITCH` | The pitch of the screw driving the slider |
| bool | `SYNCHRONIZED` | If true, the slider will be synchronized with the hammerwheel |
| float | `SYNC_FAC` | The transmisson factor for the synchronization of the slider with the hammerwheel |
| uint8_t | `speed_` | The speed thats taken, if the slider is not synchronized with the hammerwheel |

**Stepper for Sign (`STP::`):**
| Datatype | Setting | Description |
|------|------|-------------|
| bool | `ENABLED` | If true, the stepper motor will be used |
| uint8_t | `MICRO_STEPS` | The microsteps of the stepper driver |
| uint16_t | `SPR` | The steps per revolution of the stepper motor (Datasheet) |
| float | `i` | The gear ratio inbetween the stepper motor and the sign shaft |
| uint8_t | `RPM` | The desired RPM of the stepper motor |
| uint16_t | `ANGLE` | The angle the stepper motor has to turn to show the next side of the sign |
| uint32_t | `TIMEOUT` | The time after the stepper will throw an error, if the home position wasnt found till then. |
| uint16_t | `ACCEL` | The acceleration of the stepper motor - See AccelStepper for more details |
| uint16_t | `MAX_SPEED` | The maximum speed of the stepper motor - See AccelStepper for more details |

**Hammerstop Servo (`HS::`):**
| Datatype | Setting | Description |
|------|------|-------------|
| uint8_t | `OFF` | The angle the servo has to turn to deactivate the hammerstop |
| uint8_t | `ON` | The angle the servo has to turn to activate the hammerstop |
| uint8_t | `MIN` | The minimum angle the servo can turn to |
| uint8_t | `MAX` | The maximum angle the servo can turn to |
| float | `TPR` | The time the servo needs to turn 180° |

**Coupling Servo (`COUP::`):**
| Datatype | Setting | Description |
|------|------|-------------|
| uint8_t | `DIS` | The angle the servo has to turn to disenage the coupling |
| uint8_t | `EN` | The angle the servo has to turn to engage the coupling |
| uint8_t | `MIN` | The minimum angle the servo can turn to |
| uint8_t | `MAX` | The maximum angle the servo can turn to |
| float | `TPR` | The time the servo needs to turn 180° |

**Hall-Switch (`HALL::`):**
| Datatype | Setting | Description |
|------|------|-------------|
| bool | `TRIGGERED_IF` | Defines, if the Hall-Switch signals a detected magnetic field with a HIGH or LOW signal (0 = LOW, 1 = HIGH)|

At the bottom are various calculations for the following values:
| Datatype | Name | Description
|------|------|-------------|
| float | `HW::RPM` | The actual RPM at the shaft of the hammerwheel |
| uint32_t | `HW::TIMEOUT` | The time after an timeout error occurs, because no magnets where detected in time (only while running forwards -> spinning the hammerhweel) |
| uint32_t | `HW::wtime_calc` | The time the weight needs to reach the bottom + Tolerance |
| float | `HW::RS_TO_FACTOR` | The factor of speed/rs_speed |
|  |  |  |
| int | `SL::SPEED` | The speed of the slider running forwards (= speed_ or = sync) |
| float | `SL::RPM` | The actual RPM at the shaft of the slider |
| float | `SL::time_min` | The time the slider needs to reach the endstop in minutes |
| uint32_t | `SL::TIMEOUT` | The time after an timeout error occurs, because the slider didnt reach the endstop in time in ms + Tolerance|
| uint32_t | `SL::stime_calc` | = `SL::TIMEOUT` |
| float | `SL::RS_TO_FACTOR` | The factor of speed/rs_speed |
|  |  |  |
| uint16_t | `STP::FULL_REV` | The number of steps the stepper motor needs to make for one full revolution |
| uint16_t | `STP::POS` | The number of steps the stepper motor needs to make for the next/prev position |
|  |  |  |
| uint16_t | `HS::BLOCKTIME` | The time the programm will be blocked by `write()` in milliseconds for the servo to reach its target |
|  |  |  |
| uint16_t | `COUP::BLOCKTIME` | The time the programm will be blocked by `write()` in milliseconds for the servo to reach its target |

### Enumerator

The `Enumerator.hpp` file contains the following `enumerators`:

**State of the Weight (`Weight::State`):**
| State | Value | Description |
|------|------|-------------|
| `UNTRIGGERED` | 0 | The weight is somewhere inbetween the top and the bottom |
| `TOP` | 1 | The weight is at the top endstop|
| `BOTTOM` | 2 | The weight is at the bottom endstop |

**State of the Slider (`Slider::State`):**
| State | Value | Description |
|------|------|-------------|
| `UNTRIGGERED` | 0 | The slider is somewhere inbetween the endstops |
| `RIGHT` | 1 | The slider is at the right endstop |
| `LEFT` | 2 | The slider is at the left endstop |

**Postion of the Servo (`SERVO::Position`):**
| Position | Value | Description |
|------|------|-------------|
| `OFF` | 0 | The servo is in the off position |
| `ON` | 1 | The servo is in the on position |

**Color of the RGB-Button (`LED::color`):**
| Color | Value | Description |
|------|------|-------------|
| `OFF` | 0 | The LED is off |
| `RED` | 1 | The LED is red |
| `GREEN` | 2 | The LED is green |
| `BLUE` | 3 | The LED is blue |
| `YELLOW` | 4 | The LED is yellow |
| `CYAN` | 5 | The LED is cyan |
| `MAGENTA` | 6 | The LED is magenta |
| `WHITE` | 7 | The LED is white |


### Pins

The `pins.hpp` file contains all the pins connected to the arduino. To avoid naming conflicts the pins are defined in the namespace pin. So to access the pins you have to use `pin::` followed by the pin name.

**DC-Motor for Hammerwheel:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `HW_IN1` | PWM |MP6550 IN1 | Motor Control Pin 1 |
| `HW_IN2` | PWM |MP6550 IN2 | Motor Control Pin 2 |
| `HW_SLP` | I/O |MP6550 SLP | Sleep Pin, if not defined -> pull SLP to 5V to activate driver |
| `HW_VIS` | Analog | MP6550 VIS | Voltage Indicator Sense Pin [not implemented] |

**DC-Motor for Slider:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `SL_IN1` | PWM |MP6550 IN1 | Motor Control Pin 1 |
| `SL_IN2` | PWM |MP6550 IN2 | Motor Control Pin 2 |
| `SL_SLP` | I/O |MP6550 SLP | Sleep Pin, if not defined -> pull SLP to 5V to activate driver |
| `SL_VIS` | Analog | MP6550 VIS | Voltage Indicator Sense Pin [not implemented] |

**Servo-Motors for Coupling and Hammerstop:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `CO_SRV` | PWM | SG90 Control-Pin (typically orange) | Coupling Servo PWM controlling (datasheet)|
| `HS_SRV` | PWM | SG90 Control-Pin (typically orange) | Hammerstop Servo PWM controlling (datasheet) |

**Stepper-Motor for Sign:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `STP_DIR` | I/O | DRV8825 DIR | Direction Pin (0 = CW, 1 = CCW) |
| `STP_STP` | I/O | DRV8825 STEP | Step Pin (rising edge) |
| `STP_SLP` | I/O | DRV8825 SLP | Sleep Pin, if not defined -> pull SLP to 5V to activate driver |
| `STP_RST` | I/O | DRV8825 RST | Reset Pin, if not defined -> pull RST to 5V to avoid resetting the driver constantly |
| `STP_FLT` | I/O | DRV8825 FLT | Fault Pin, is LOW if an error occurs |
| `STP_EN` | I/O | DRV8825 EN | Enable Pin, if not defined -> its pulled down and is enabled then |
| `STP_M0` | I/O | DRV8825 M0 | Microstep Resolution Pin 0 |
| `STP_M1` | I/O | DRV8825 M1 | Microstep Resolution Pin 1 |
| `STP_M2` | I/O | DRV8825 M2 | Microstep Resolution Pin 2 |

**Endstops for Slider and Weight:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `WG_ES_T` | I/O | Endstop Top | Endstop for the weight at the top |
| `WG_ES_B` | I/O | Endstop Bottom | Endstop for the weight at the bottom |
| `SL_ES_R` | I/O | Endstop Right | Endstop for the slider at the right |
| `SL_ES_L` | I/O | Endstop Left | Endstop for the slider at the left |

**Hall-Switch for Hammerwheel and Sign:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `HW_HALL` | I/O | Hall-Switch | Hall-Switch for the hammerwheel |
| `SG_HALL` | I/O | Hall-Switch | Hall-Switch for the sign |

**RGB-Button for Interaction:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `GO_BUT` | I/O | RGB-Button Button-Pin | Button to start the machine or interact in other purposes|
| `GO_LED_R` | I/O or PWM | RGB-Button Red-Pin | Red LED of the RGB-Button, if PWM then more colors are possible |
| `GO_LED_G` | I/O or PWM | RGB-Button Green-Pin | Green LED of the RGB-Button, if PWM then more colors are possible |
| `GO_LED_B` | I/O or PWM | RGB-Button Blue-Pin | Blue LED of the RGB-Button, if PWM then more colors are possible |

**Others:**
| Pin-Name | Arduino Pin-Type | Device | Description |
|------|------------ | ----------------| -------------|
| `FAN` | I/O | Fan | Fan to cool the arduino while the machine is running |
| `CLEAR_ERROR` | I/O | Button | Button to clear an error |
 

