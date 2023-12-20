#pragma once

#include <Arduino.h>

/**
 * @file Sensor.hpp
 * @brief Contains the declarations of various sensor classes.
 */

namespace Sensor{

    /**
     * @class HallSwitch
     * @brief Represents a Hall switch sensor.
     */
    class HallSwitch {
        private:
            uint8_t pin_; /**< The pin number to which the Hall switch is connected. */
            bool triggered_; /**< Attribute for storing the triggered State (LOW, HIGH) */
        public:
            /**
             * @brief Constructs a HallSwitch object.
             * @param pin The pin number to which the Hall switch is connected.
             * @param triggeredIf The logic level at which the Hall switch is considered triggered. Defaults to HIGH.
             */
            HallSwitch(uint8_t pin, bool triggeredIf = HIGH);

            /**
             * @brief Sets the logic level at which the Hall switch is considered triggered.
             * @param triggeredIf The logic level at which the Hall switch is considered triggered.
             */
            void setTriggeredIf(bool triggeredIf);

            /**
             * @brief Reads the current state of the Hall switch.
             * @return true if the Hall switch is triggered, false otherwise.
             */
            bool read();

            /**
             * @brief Prints the data of the Hall switch.
             */
            void printData();
    };

    /**
     * @class Endstops
     * @brief Represents a pair of endstops.
     */
    class Endstops {
        private:
            uint8_t pin1_; /**< The first pin number to which first the endstop is connected. */
            uint8_t pin2_; /**< The second pin number to which the second endstop is connected. */
            bool triggered_; /**< Attribute for storing the triggered State (LOW, HIGH) */
        public:
            /**
             * @brief Constructs an Endstop object.
             * @param pin1 The first pin number to which the endstop is connected.
             * @param pin2 The second pin number to which the endstop is connected.
             * @param triggeredIf The logic level at which the endstop is considered triggered. Defaults to LOW.
             */
            Endstops(uint8_t pin1,uint8_t pin2, bool triggeredIf = LOW);

            /**
             * @brief Reads the current state of the endstop.
             * @return 0 if no endstops are triggered, 1 if first Endstop is triggered, and 2 if second Endstop is triggered.
             */
            int8_t read();

            /**
             * @brief Prints the data of the endstop.
             */
            void printData();
    };

    /**
     * @class Button
     * @brief Represents a button sensor. Theres the possibilty to add the Button-LED if there's one.
     */
    class Button{
        private:
            uint8_t pin_; /**< The pin number to which the button is connected. */
            bool triggered_; /**< Attribute for storing the triggered State (LOW, HIGH) */
        public:
            /**
             * @brief Constructs a Button object.
             * @param pin The pin number to which the button is connected.
             * @param LED The pin number to which the LED is connected. Defaults to 255 (no LED).
             * @param triggeredIf The logic level at which the button is considered triggeredIf. Defaults to LOW.
             */
            Button(uint8_t pin, uint8_t LED = 255, bool triggeredIf = LOW);

            /**
             * @brief Reads the current state of the button.
             * @return true if the button is triggeredIf, false otherwise.
             */
            bool read();

            /**
             * @brief Prints the data of the button.
             */
            void printData();

            /**
             * @brief Toggles the state of the LED (if connected).
             */
            void toggleLED();
    };
}
