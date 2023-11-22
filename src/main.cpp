#include <Arduino.h>    //Arduino Library
#include "MP6550_driver.hpp"     //Driver MP6550 Library
#include "ServoExp.hpp"      //Custom Servo Library -> depends on Servo.h
#include "Sensors.hpp"          //Sensor Library
#include "config.hpp"           //Config file -> Pins, settings, etc.
#include "Testfunctions.hpp"    //Testfunctions
#include <AccelStepper.h>       //Stepper Library
#include "CustomStepper.hpp"    //Custom Stepper Library -> depends on AccelStepper.h

//* functions:
void inits();        //initialisation function
void defaults();     //default function
void DC2reset();  //reset function for both DC-Motors
bool DC2run();      //run function for both DC-Motors
void couple();      //coupling and engaging Hammerstop
void decouple();    //decoupling and releasing Hammerstop

//* objects:
MP6550 moHr(pin::HR_IN1, pin::HR_IN2, pin::HR_SLP);  //Motor Hammerrad (In1, In2, Sleep)
MP6550 moSl(pin::SL_IN1, pin::SL_IN2, pin::SL_SLP);  //Motor Schlitten (In1, In2, Sleep)
ServoExp srvHs(pin::HS_SRV, SRV_MIN, SRV_MAX);  //Servo Hammerstop (Pin, Min, Max, Pos1, Pos2)
ServoExp srvKu(pin::KU_SRV, SRV_MIN, SRV_MAX);  //Servo Kupplung (Pin, Min, Max, Pos1, Pos2)
CustomStepper stp(STP_INTERFACE, pin::STP_STP, pin::STP_DIR, pin::STP_SLP, STP_MAXSPEED, STP_ACCEL, pin::SH_HALL, HALL_TRIGGER);    //Stepper Schild (Interface, Step-Pin, Direction-Pin, Enable-Pin, Max.Speed, Acceleration, Hall-Sensor-Pin)
Endstop esGe(pin::GE_ES_U, pin::GE_ES_O, ES_MODE);   //Endschalter Gewicht: State 2 = Oben, State 1 = Unten
Endstop esSl(pin::SL_ES_L, pin::SL_ES_R, ES_MODE);    //Endschalter Schlitten: State 2 = Rechts, State 1 = Links
Hall hallHr(pin::HR_HALL);                      //Hall-Sensor Hammerrad 

//* Testobjects:

int n = 10;

void setup() {

    pinMode(pin::GO_BUT, INPUT_PULLUP);    //Start-Button

    //* Serial-Setup:
    Serial.begin(SERIAL_BPS);

    //* Servo-Setup:
    srvHs.attach();
    srvKu.attach();
    srvHs.setPosData(pos::HS_1, pos::HS_2);
    srvKu.setPosData(pos::KU_1, pos::KU_2);
    //*Servo-Data to Serial:
    //srvHs.printData();
    //srvKu.printData();

    //inits();      //initialisation function
}

//*Testfunctiones-List:
//
//*     ~~ using namepspace testing -> i.e.: > testing::TestDC(moHr); ~~
//  Einzeltests:
//      - TestDC(motor mdc);
//      - TestServo(ServoExp srv, Endstop esGe);
//      - TestServo(ServoExp srv);
//      - TestStepper();
//      - TestES(Endstop es);
//      - TestHall(Hall hall);
//      - TestButton(uint8_t pin);
//
//  Funktionsgruppen:
//      - TestHammer(motor mdc, ServoExp srvHs, Endstop esGe, Hall hallHr);
//      - TestSchlitten(motor moSl, Servo srvKu, Endstop esSl);
//      - TestSchild(AccelStepper stp, Hall hallSh);


void loop() {
    //defaults();       //default function
    //testing::TestDCbrake(moHr, 16, 18, 15);
    //testing::TestDCBrake2(moHr, 15);
}



void inits(){
    if(esGe.read() != 1 && esSl.read() !=1){        //*  1.     Check status of relevant sensors
        decouple();                                 //* (2.)     Decouple and then Hammerstop 
        DC2reset();                              //* (3.)      Move Schlitten and Weight to start position
    }                               
    else couple();                                  //*  2.|(4.)   Couple and release Hammerstop
    stp.home(100);                                  //*  3.|(5.)    Move Schild to start position
}

void defaults(){
    //* 0. Wait for User Input (Start Button)
    while(digitalRead(pin::GO_BUT) == HIGH){
        delay(10);
    };
    //* 1. Check status of all sensors and servos
    //TODO: Check status of servos -> decoupled/coupled?, Hammer stopped/released? (Position of Sign? <- doesnt have to be here)
    //* 2. Start both Motors     
    //* 3. Wait for Endstop Weights to be reached     
    //* 4. Stop both Motors
    //* 5. Decouple and then Hammerstop
    //* 6. Move Weight to top position
    //* 7. Couple and release Hammerstop
    //* repeat (2-7 until Schlitten-Endstop is reached)
    while(DC2run()){
        delay(5);
    }
    //* 8. init();
    delay(DELAY);
    inits();
    
}

void DC2reset(){

    int speed = DC_RS_SPEED;
    int state = 1;

    if(esGe.read() != state && esSl.read() != state){
        moHr.run(speed);
        moSl.run(speed);
        while(esGe.read() != state && esSl.read() != state){
            delay(10);
        }
        if(esGe.read() == state && esSl.read() == state){
            moHr.brake();
            moSl.brake();
            return;
        }
        else if(esGe.read() == state && esSl.read() != state){
            moHr.brake();
            moSl.run(speed);
            esSl.waitUntil(state);
            moSl.brake();
            return;
        }
        else if(esGe.read() != state && esSl.read() == state){
            moHr.run(speed);
            moSl.brake();
            esGe.waitUntil(state);
            moHr.brake();
            return;
        }
    }
    else if(esGe.read() != state && esSl.read() == state){
        moHr.run(speed);
        moSl.brake();
        esGe.waitUntil(state);
        moHr.brake();
        return;
    }
    else if(esGe.read() == state && esSl.read() != state){
        moHr.brake();
        moSl.run(speed);
        esSl.waitUntil(state);
        moSl.brake();
        return;
    }
    else{
        moHr.brake();
        moSl.brake();
        return;
    }
}

bool DC2run(){
    int speed = DC_RUN_SPEED;
    int revSpeed = DC_RS_SPEED;

    if(esSl.read() == 2){
        moHr.brake();
        moSl.brake();
        return false;
    }
    else{
        if(esGe.read() != 2){
            moSl.run(speed);
            moHr.run(speed);
            return true;
        }
        else{
            moSl.brake();
            moHr.brake();
            delay(DELAY_2);
            decouple();
            moHr.run(revSpeed);
            esGe.waitUntil(1);
            moHr.brake();
            delay(DELAY);
            couple();
            return true;
        }
    }


}

void couple(){
        //Check if servos are already in position
        if(srvKu.read() == pos::KU_2 && srvHs.read() == pos::HS_1){
            return;
        }
        //If hammerstop is in place but it isnt coupled
        else if(srvKu.read() != pos::KU_2 && srvHs.read() == pos::HS_1){
            srvKu.write(pos::KU_2);
            delay(DELAY);
            return;
        }
        //If hammerstop is not in place but it is coupled
        else if(srvKu.read() == pos::KU_2 && srvHs.read() != pos::HS_1){
            
            if(hallHr.read() == HALL_TRIGGER){
            moHr.run(-255);
            hallHr.waitUntil(!HALL_TRIGGER);
            moHr.brake();
            }
    
            else{
                moHr.run(-255);
                hallHr.waitUntil(HALL_TRIGGER);
                hallHr.waitUntil(!HALL_TRIGGER);
                moHr.brake();
            }
            delay(DELAY_4);
            srvKu.write(pos::KU_2);
        }
        delay(DELAY);
        return;
}

void decouple(){

    if(srvKu.read() == pos::KU_1 && srvHs.read() == pos::HS_2){
        return;
    }

    if(srvKu.read() != pos::KU_1){
        srvKu.write(pos::KU_1);
        delay(DELAY);
    } 

    if(hallHr.read() == HALL_TRIGGER){
        moHr.run(255);
        hallHr.waitUntil(!HALL_TRIGGER);
        moHr.brake();
        
    }

    else{
        moHr.run(255);
        hallHr.waitUntil(HALL_TRIGGER);
        hallHr.waitUntil(!HALL_TRIGGER);
        moHr.brake();
    }

    delay(DELAY_4);
    srvHs.write(pos::HS_2);
    delay(DELAY);
    return;
}