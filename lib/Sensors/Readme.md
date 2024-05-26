# Sensors.hpp

## Overview

The `sensors.hpp` file contains the implementation of various sensor classes used in the DaVinciFeilenHauMaschine project.

## Table of Contents

- [Sensors.hpp](#sensorshpp)
  - [Overview](#overview)
  - [Sensor Class](#sensor-class)
    - [Properties](#properties)
    - [Methods](#methods)
  - [Temperature Sensor Class](#temperature-sensor-class)
    - [Properties](#properties-1)
    - [Methods](#methods-1)
  - [Pressure Sensor Class](#pressure-sensor-class)
    - [Properties](#properties-2)
    - [Methods](#methods-2)
  - [Usage](#usage)

## Sensor Class

The `Sensor` class is the base class for all sensor types. It provides common functionality and properties that are shared among different sensors.

### Properties

- `id` (int): The unique identifier of the sensor.
- `name` (string): The name of the sensor.
- `type` (string): The type of the sensor.

### Methods

- `read()`: Reads the sensor data and returns the measured value.
- `calibrate()`: Calibrates the sensor for accurate readings.
- `reset()`: Resets the sensor to its default state.

## Temperature Sensor Class

The `TemperatureSensor` class extends the `Sensor` class and provides additional functionality specific to temperature sensors.

### Properties

- `unit` (string): The unit of temperature measurement (e.g., Celsius, Fahrenheit).

### Methods

- `convertToFahrenheit()`: Converts the temperature reading from Celsius to Fahrenheit.
- `convertToCelsius()`: Converts the temperature reading from Fahrenheit to Celsius.

## Pressure Sensor Class

The `PressureSensor` class extends the `Sensor` class and provides additional functionality specific to pressure sensors.

### Properties

- `unit` (string): The unit of pressure measurement (e.g., Pascal, Bar).

### Methods

- `convertToPascal()`: Converts the pressure reading to Pascal.
- `convertToBar()`: Converts the pressure reading to Bar.

## Usage

To use the `sensors.hpp` library in your project, include the following line at the top of your source file:
