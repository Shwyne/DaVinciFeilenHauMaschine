#pragma once

#include <Arduino.h>
#include "pins.hpp"

//*Software-Settings:
//Serial Connection to Arduino (Check Device-Manager -> COM -> Arduino -> Settings -> Properties -> Port Settings)
namespace SERIALSET{

constexpr int BAUTRATE = 9600;
constexpr int DELAY = 1000; 
}
//Debugging and Error-Management:
constexpr uint8_t DEBUG = 2;    //0 = off, 1 = basic info, 2 = detailed info
constexpr bool ERROR_MANAGEMENT = true;   // 0 or true = off, 1 or false = on

//----------------------------------------------------
//Hammerwheel Motor:
namespace HW {

constexpr float RPM_255 = 19;
constexpr uint8_t MAXSPEED = 255;
constexpr bool reversed = 0;
constexpr int speed = 180;
constexpr int rs_speed = 255;
constexpr uint8_t nMagnets = 6;
constexpr float i = 0.5;
//Tooth Ab = 30
//Tooth An = 60
}

//----------------------------------------------------
//Slider Motor:
namespace SL {

constexpr float RPM_255 = 19;
constexpr uint16_t L_mm = 108; 
constexpr uint8_t pitch = 5;
constexpr uint8_t MAXSPEED = 255;
constexpr uint8_t speed_ = 180;     //Used only, if synchronized = 0
constexpr bool synchronized = 1;
constexpr float sync_factor = 0.5;
constexpr bool reversed = 0;
constexpr int rs_speed = 255;
constexpr float i = 0.5;
}

//----------------------------------------------------
//Stepper Motor:
namespace STP{

constexpr uint8_t MICRO_STEPS = 16;     //1,2,4,8,16,32
constexpr uint8_t SPR = 200;            //Steps per Revolution
constexpr float i = 0.1;                //Gear Ratio
constexpr uint8_t RPM = 10;             //Speed
constexpr uint16_t ANGLE = 120;         //Angle of Sign
}

//----------------------------------------------------
//Servo Hammerstop:
namespace HS {

constexpr float TPD = 0.14;     //Timeout per TIMEOUT_DEGREE
constexpr uint8_t TIMEOUT_DEGREE = 60;
constexpr uint8_t OFF = 0;
constexpr uint8_t ON = 80;
constexpr uint8_t TOLERANCE = 5;
constexpr uint8_t MIN = 0;
constexpr uint8_t MAX = 180;
}
//Servo Coupling:
namespace COUP {

constexpr float TPD = 0.14;     //Timeout per TIMEOUT_DEGREE
constexpr uint8_t TIMEOUT_DEGREE = 60;
constexpr uint8_t DIS = 0;
constexpr uint8_t EN = 110;
constexpr uint8_t TOLERANCE = 3;
constexpr uint8_t MIN = 0;
constexpr uint8_t MAX = 180;
}

namespace HALL{

constexpr bool TRIGGERED_IF = 0; //0 = LOW, 1 = HIGH
}








//===========================================================================
//Dont change anything after! It's for the compiler to calculate the values.
//===========================================================================

namespace HW{

constexpr float RPM = (speed / 255.0) * RPM_255 * i;        //
constexpr short SPEED = (reversed) ? -speed : speed;
constexpr short RS_SPEED = (reversed) ? rs_speed : -rs_speed;
constexpr uint16_t TIMEOUT = (RPM>0 && i>0 && nMagnets>0) ? 60000/(RPM*nMagnets) * 2: 0;
}

namespace SL{

constexpr int speed = (synchronized) ? HW::RPM/(RPM_255*i) * 255 * sync_factor : speed_;
constexpr short SPEED = (reversed) ? -speed : speed;
constexpr short RS_SPEED = (reversed) ? rs_speed : -rs_speed;
constexpr float RPM = (synchronized) ? HW::RPM : (speed / 255.0) * RPM_255 * i;
constexpr float time_min = (RPM>0 && pitch>0) ? L_mm/(RPM*pitch) : 0;
constexpr uint32_t TIMEOUT = time_min * 60000 * 1.2;
}

namespace STP{

constexpr uint16_t POS = (STP::SPR * STP::MICRO_STEPS * (STP::ANGLE/360.0)) / STP::i;
}

namespace HS{

constexpr uint16_t TIMEOUT = abs(MAX - MIN) * TPD * TIMEOUT_DEGREE * 2;
}

namespace COUP{

constexpr uint16_t TIMEOUT = abs(MAX - MIN) * TPD * TIMEOUT_DEGREE * 2;
}
