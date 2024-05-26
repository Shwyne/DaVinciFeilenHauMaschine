# TestFunctions

The `TestFunctions` module, defined in `TestFunctions.cpp` and `TestFunctions.hpp`, provides a set of functions for testing various components of the system. These functions are useful for debugging and ensuring that all components are functioning as expected.

## TestFunctions.hpp

This header file includes the declarations of all the test functions. It also includes the necessary libraries and external objects that are used in the test functions. 

Here's a brief overview of the functions declared in this file:

- `void DC(MP6550 DC, uint8_t speed = 255)`: This function tests a Motor-Driver. It takes two parameters: an `MP6550` object representing the motor driver to be tested, and an optional speed parameter that defaults to 255.

- `void Servo(ServoExp srv, uint8_t pos1 = 0, uint8_t pos2 = 180)`: This function tests a Servo. It takes three parameters: a `ServoExp` object representing the servo to be tested, and two optional position parameters that default to 0 and 180 respectively.

- `void Stepper(AccelStepper stp)`: This function tests a Stepper. It takes one parameter: an `AccelStepper` object representing the stepper to be tested.

- `void Endstops(Sensor::Endstops es)`: This function tests a pair of Endstops. It takes one parameter: a `Sensor::Endstops` object representing the pair of endstops to be tested.

- `void Hall(Sensor::HallSwitch hall)`: This function tests a Hall-Switch. It takes one parameter: a `Sensor::HallSwitch` object representing the hall switch to be tested.

- `void Button()`: This function tests a Button. It doesn't take any parameters.

- `void ButtonLED()`: This function tests a Button with LED. It doesn't take any parameters.

## TestFunctions.cpp

This source file includes the definitions of all the test functions declared in `TestFunctions.hpp`. Each function contains the logic for testing a specific component.

For example, the `ButtonLED()` function tests an RGB LED by shifting through the colors. It first sets the delay time, then starts the test by printing a message to the serial console (if debugging is enabled). It then shifts through the colors, updating the LED with each color and delaying for the specified delay time. After shifting through all the colors, it updates the LED to white and prints a message to the serial console indicating that the test is finished.

The other functions in this file follow a similar pattern: they set up the test, perform the test, and then print a message to the serial console indicating that the test is finished.