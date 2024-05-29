#### Overview

This module provides a set of functions that are useful for testing purposes. The functions are divided into the following categories:

### Test Components (`testComp::`)
This namespace contains functions that test the components of the machine. To call a function, add `testComp::` infront (e.g. `testComp::DC(DC-Motor, speed)` for testing a DC-Motor). Check the `config.hpp` if the tests are done with or without the RGB-Button at the front. The following table lists the functions in this namespace:
| Return-Type | Function | Input-Parameter | Description |
| --- | --- | --- | --- |
| void | `DC` | MP6550 DC, uint8_t speed | Tests the DC-Motor with the given speed |
| void | `Servo` | ServoExp srv, uint8_t pos1, uint8_t pos2 | Tests the Servo with the given positions |
| void | `Stepper` | StepperExp stp | Tests the Stepper |
| void | `Endstops` | Sensor::Endstops es | Tests a pair of endstops |
| void | `Hall` | Sensor::Hallswitch hall | Tests the Hallswitch |
| void | `Button` | none | Tests the RGB-Button (Without LED, if not defined in `config.hpp`) |
| void | `ButtonLED` | none | Tests the RGB LED of the RGB-Button |


### Test Functions (`testFunc::`)
This namespace contains functions that test the functions of the machine. To call a function, add `testFunc::` infront (e.g. `testFunc::Weight(Weight, speed)` for testing the weight). The following table lists the functions in this namespace:
| Return-Type | Function | Input-Parameter | Description |
| --- | --- | --- | --- |
| void | `findEndstops` | none | Returns the status of the endstops. It prints the name of the endstop in the serial monitor if the endstop is triggered |
| void | `WeightHammer` | none  | Tests the function of winding the weight up and down and turning the hammerwheel. It also checks the hammerstop |
| void | `Slider` | none | Tests the function of the slider |
| void | `Coupling` | none | Tests the coupling of the shafts |

