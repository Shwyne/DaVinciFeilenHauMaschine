#pragma once

#include <Arduino.h>

//Used for the Weight-Endstops to make code more readable.
//Example: if(WGes.read() == Weight::TOP) -> If the weight is at the top.
namespace Weight{
enum State : uint8_t {
    UNTRIGGERED = 0,    //Untriggered
    TOP = 1,            //Weight triggered at the top
    BOTTOM = 2,         //Weight triggered at the bottom
};
}

//*Endstops Slider:

//Used for the Slider-Endstops to make code more readable.
//Example: if(SLes.read() == Slider::RIGHT) -> If the slider is at the right endstop.
namespace Slider{
enum State : uint8_t {
    UNTRIGGERED = 0,    //Untriggered
    RIGHT = 1,          //Slider reached the right endstop
    LEFT = 2,           //Slider reached the left endstop
};
}

//*Positions of the Servo (OFF = pos1, ON = pos2):
//Can be used for functions that should work with both servos. e.g. TestServo()
//Its important that pos1 and pos2 got set with myServo.setPositions(pos1, pos2) before using this enum.
//Otherwise the positions are not defined and the servo will not move.
//Example: myServo.runToPos(SERVO::OFF); -> Moves the servo to position 1.
namespace SERVO{
enum Position : uint8_t {
    OFF = 0,    //Position 1
    ON = 1,     //Position 2
};
}

//*Go-Button:
//If the Go-Button has RGB-LEDs, it can be updated using this enum.
//Example: Go.updateLED(LED::RED); -> Updates the LED to red.
namespace LED{
enum color : uint8_t {
    OFF = 0,        //LED off
    RED = 1,        //LED red
    GREEN = 2,      //LED green
    BLUE = 3,       //LED blue
    YELLOW = 4,     //LED yellow
    CYAN = 5,       //LED cyan
    MAGENTA = 6,    //LED magenta
    WHITE = 7,      //LED white
};
}



