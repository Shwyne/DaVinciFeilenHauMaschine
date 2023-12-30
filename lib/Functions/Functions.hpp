#pragma once

#include "DC_Motor_Driver.hpp"
#include "Sensor.hpp"
#include "ServoExp.hpp"
#include "config.hpp"
#include <Arduino.h>
#include <DRV8825.h>
#include "Error_Handling.hpp"

extern DC_Motor_Driver::MP6550 SLdc;
extern DC_Motor_Driver::MP6550 HWdc;
extern ServoExp HSsv;
extern ServoExp COsv;
extern A4988 SGst;
extern Sensor::Endstops WGes;
extern Sensor::Endstops SLes;
extern Sensor::HallSwitch HRha;
extern Sensor::HallSwitch SGha;

extern ErrCode erCode;
extern uint32_t ctime;

namespace serv {
    void decouple();
    void couple();
    void hammerstop();
    void hammergo();
}

namespace step {
    void pos1();
    void pos2();
    void pos3();
}