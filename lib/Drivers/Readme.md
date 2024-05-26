# Drivers.hpp

This header file contains the implementation of the following driver classes:

## MP6550

The `MP6550` class provides the driver for the MP6550 motor controller. It includes the following attributes and methods:

#### Attributes

- `speed`: The current speed of the motor.
- `direction`: The current direction of the motor.

#### Methods

- `setSpeed(speed: int)`: Sets the speed of the motor.
- `setDirection(direction: MotorDirection)`: Sets the direction of the motor.

## ServoExp

The `ServoExp` class provides the driver for the ServoExp servo controller. It includes the following attributes and methods:

#### Attributes

- `positions`: An array of servo positions.

#### Methods

- `setPosition(index: int, position: int)`: Sets the position of a servo at the specified index.
- `setSpeed(index: int, speed: int)`: Sets the speed of a servo at the specified index.
- `setAcceleration(index: int, acceleration: int)`: Sets the acceleration of a servo at the specified index.

### Usage

To use these drivers in your project, follow these steps:

1. Include the "Drivers.hpp" header file in your source code:

    ```cpp
    #include "Drivers.hpp"
    ```

2. Create an instance of the desired driver class:

    ```cpp
    MP6550 motorController;
    ServoExp servoController;
    ```

3. Use the available attributes and methods to control the motors or servos:

    ```cpp
    // Example: Set motor speed and direction
    motorController.setSpeed(100);
    motorController.setDirection(MotorDirection::FORWARD);

    // Example: Set servo position
    servoController.setPosition(0, 90); // Set servo at index 0 to 90 degrees
    ```

For more detailed information on the usage of each attribute and method, refer to the respective source code files.

## Dependencies

These driver classes may have dependencies on other libraries or modules. Make sure to install and include any required dependencies in your project.

## Contributing

If you find any issues or have suggestions for improvements, feel free to contribute by submitting a pull request or creating an issue in the project repository.

## License

This code is licensed under the [MIT License](https://opensource.org/licenses/MIT). See the [LICENSE](LICENSE) file for more details.