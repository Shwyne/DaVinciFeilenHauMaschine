#pragma once

#include <Arduino.h>
#include "pins.hpp"

//*Software-Settings:
//Serial Connection to Arduino (Check Device-Manager -> COM -> Arduino -> Settings -> Properties -> Port Settings)
namespace SERIALSET{
    constexpr int BAUTRATE = 9600;
    constexpr int DELAY = 1000;
}

//============================================================
//*Hardware-Settings:

//Go-Button:
namespace LED{
    enum color{
        OFF = 0,
        RED = 1,
        GREEN = 2,
        BLUE = 3,
        YELLOW = 4,
        CYAN = 5,
        MAGENTA = 6,
        WHITE = 7,
    };
}

//----------------------------------------------------
//Hall-Sensor:
namespace HALL{
    constexpr bool TRIGGER = 0;
    constexpr uint16_t TIMEOUT = 2000;
}

//----------------------------------------------------
//Hammerwheel Motor:
namespace HW {
    constexpr uint8_t MAXSPEED = 255;
    constexpr bool reversed = 0;
    constexpr int speed = 180;
    constexpr int rs_speed = 255;
    //Tooth Ab = 30
    //Tooth An = 60
}
//Endstops Weight:
namespace Weight {
    constexpr bool TRIGGER = 0;
    enum State {
        UNTRIGGERED = 0,
        TOP = 1,
        BOTTOM = 2,
};}
//----------------------------------------------------
//Slider Motor:
namespace SL {
    constexpr uint8_t MAXSPEED = 255;
    constexpr bool synchronized = 0;
    constexpr float sync_factor = 1;
    constexpr bool reversed = 0;
    constexpr int rs_speed = 255;
}
//Endstops Slider:
namespace Slider {
    constexpr bool TRIGGER = 0;
    enum State {
        UNTRIGGERED = 0,
        LEFT = 1,
        RIGHT = 2,
    };
}
//----------------------------------------------------
//Stepper Motor:
namespace STP{
    constexpr uint8_t INTERFACE = 1;
    constexpr uint8_t MODE = 16;
    constexpr uint8_t STEPS = 200;
    constexpr float i = 0.1;
    constexpr uint8_t RPM = 10;
}
//----------------------------------------------------
//Servo Hammerstop:
namespace HS {
    constexpr uint8_t OFF = 0;
    constexpr uint8_t ON = 80;
    constexpr uint8_t TOLERANCE = 5;
    constexpr uint8_t TriesBeforeError = 5;
    constexpr uint8_t MIN = 0;
    constexpr uint8_t MAX = 180;
}
//Servo Coupling:
namespace COUP {
    constexpr uint8_t DIS = 0;
    constexpr uint8_t EN = 110;
    constexpr uint8_t TOLERANCE = 3;
    constexpr uint8_t TriesBeforeError = 5;
    constexpr uint8_t MIN = 0;
    constexpr uint8_t MAX = 180;
}








//===========================================================================
//Dont change anything after! It's for the compiler to calculate the values.
//===========================================================================

namespace HW{
constexpr short SPEED = (reversed) ? -speed : speed;
constexpr short RS_SPEED = (reversed) ? rs_speed : -rs_speed;
}

namespace SL{
constexpr int speed = (synchronized) ? HW::speed * sync_factor : 255;
constexpr short SPEED = (reversed) ? -speed : speed;
constexpr short RS_SPEED = (reversed) ? rs_speed : -rs_speed;
}

namespace STP{
constexpr uint8_t POS = uint8_t(STP::STEPS / 3 * STP::i) * STP::MODE;
}
