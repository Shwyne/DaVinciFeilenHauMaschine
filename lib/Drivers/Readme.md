#### Overview

The `drivers.hpp` file contains the declaration of various driver classes used in the DaVinciFeilenHauMaschine project.

#### Table of Contents
- [MP6500 Class](#mp6500-class)
- [ServoExp Class](#servoexp-class)
- [StepperExp Class](#stepperexp-class)

### MP6500 Class

The class `MP6500` stores data regarding a MP6500 stepper motor driver. It stores the following `attributes`:
| Datatype | Name | Description |
|------|------|-------------|
| uint8_t | `IN1pin, IN2pin` | Motor control pins of the driver |
| uint8_t | `SLPpin` | Sleep pin of the driver, default 255 = deactivated -> Pull SLP to 5 Volt then |
| bool | `reversed` | Can be used to reverse the motor direction, if needed |
| bool | `autoSleep` | If true, the driver will go to sleep after braking and wake up when about to run |
| int | `speed` | The current speed of the motor (PWM: -255 to 255 = -100 % to 100 %) |

Also it comes with the following `methods`:
| Output | Method | Input | Description |
|--------|--------|-------|-------------|
| none | `MP6500` | uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin, bool reversed, bool autoSleep | Constructor of the class |
| void | `run` | int speed | Runs the motor with the given speed (-speed = backwards) |
| void | `brake` | none | Brakes the motor, blocks for around 500 ms |
| void | `coast` | none | Coasts the motor, doesn't block |
| int | `getSpeed` | none | Returns the current speed of the motor |
| bool | `isSleeping` | none | Returns true if the driver is in sleep mode |
| void | `sleep` | none | Puts the driver to sleep |
| void | `wake` | none | Wakes the driver up |

The Definitions can be found in `MP6500.cpp`.

### ServoExp Class

The class `ServoExp` stores data regarding a PCA9685 servo driver. It stores the following `attributes`:
| Datatype | Name | Description |
|------|------|-------------|
| uint8_t | `MIN, MAX` | The minimum and maximum Angle possible for the servo |
| uint8_t | `pin` | The pin number where the Servo-PWM is connected |
| uint8_t | `pos1, pos2` | storing positions for easier control |
| uint32_t | `blockingTime` | The time the programm will be blocked by `write()` in milliseconds for the servo to reach its target |

Also it comes with the following `methods`:
| Output | Method | Input | Description |
|--------|--------|-------|-------------|
| none | `ServoExp` | uint8_t pin, uint8_t MIN, uint8_t MAX, uint32_t blockingTime | Constructor of the class |	
| void | `attach`   | none | Attaches the servo to the pin |
| void | `setPosition` | uint8_t pos1, uint8_t pos2 | Saves positions for later access |
| void | `write` | uint8_t angle | Writes the angle to the servo |
| void | `runToPos` | uint8_t pos | Runs the servo to the given position (0 = pos1, 1 = pos2) |
| none | `~ServoExp` | none | Destructor of the class, detaches the servo |

The Definitions can be found in `ServoExp.cpp`.

### StepperExp Class

The class `StepperExp` stores data regarding a PCA9685 stepper driver. It stores the following `attributes`:
| Datatype | Name | Description |
|------|------|-------------|
| uint8_t | `M0pin, M1pin, M2pin` | Microstep control pins of the driver |
| uint8_t | `SLPpin` | Sleep pin of the driver, default 255 = deactivated -> Pull SLP to 5 Volt then |
| uint8_t | `microSteps` | The number of microsteps the driver should work in |
| uint32_t | `StepsPerRev` | The number of steps the motor needs to make for one revolution |
| uint32_t | `StepsPerPos` | The number of steps the motor needs to make for the next/prev position |
| uint32_t | `currentPos` | The current position of the motor |
| bool | `autoSleep` | If true, the driver will go to sleep after reaching the position |

Also it comes with the following `methods`:
| Output | Method | Input | Description |
|--------|--------|-------|-------------|
| none | `StepperExp` | uint8_t stepPin, uint8_t dirPin | Constructor of the class |
| void | `setupMS` | uint8_t M0pin, uint8_t M1pin, uint8_t M2pin | Sets up the microstep pins |
| void | `setMS` | uint8_t microSteps | Sets the number of microsteps |
| void | `setPos` | uint16_t steps, uint8_t angle | Sets the position of the motor for `nextPos()` and `prevPos()` |
| void | `setSleep` | uint8_t SLPpin, bool autoSleep | Sets the sleep pin of the driver and autoSleep (default = false) |
| uint8_t | `getMS` | none | Returns the number of microsteps |
| void | `nextPos` | none | Moves the motor to the next position |
| void | `prevPos` | none | Moves the motor to the previous position |
| void | `sleep` | none | Puts the driver to sleep |
| void | `wake` | none | Wakes the driver up |
| void | `zeroPos` | none | Sets the current position to 0 |
| uint8_t | `log2` | uint8_t n | Returns the logarithm to the base 2 of n for setting up the microstep mode|
