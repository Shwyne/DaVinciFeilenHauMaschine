#include "Testfunctions.hpp"

#define DELAY 2000  //Default delay for testing purposes

namespace testing{  //!Namespace for testing functions

    void TestDC(MP6550 mdc){

        Serial.println("Testing MP6550");
        Serial.println("Testing run");
        mdc.run(255);
        delay(DELAY);
        mdc.run(-255);
        delay(DELAY);

        Serial.println("Testing brake");
        mdc.brake();
        delay(DELAY);
        mdc.run(255);
        delay(DELAY);
        mdc.brake();
        delay(DELAY);
        mdc.run(-255);
        delay(DELAY);
        mdc.brake();

        Serial.println("Testing sleep");
        mdc.sleep();
        delay(2*DELAY);
    }     

    void TestServo(ServoExp srv){
        if(srv.attached() == false){
            srv.attach();
        }
        Serial.println("Testing Servo");
        Serial.println("Servo to pos1");
        srv.run(pos::HS_1);
        delay(2*DELAY);
        Serial.println("Servo to pos2");
        srv.run(pos::HS_2);
        delay(2*DELAY);
    }

    void TestServo(ServoExp srv, Endstop es){
        if(srv.attached() == false){
            srv.attach();
        }
        Serial.println("Testing Servo");
        Serial.println("Servo to pos1 - Waiting for Signal");
        while(es.read() != 1){
            delay(10);
        }
        srv.run(pos::HS_1);
        delay(2*DELAY);
        Serial.println("Servo to pos2 - Waiting for Signal");
        while(es.read() != 1){
            delay(10);
        }
        srv.run(pos::HS_2);
        delay(2*DELAY);
    }

    void TestHammer(motor moHr, CustomServo srvHs, Endstop esGe, Hall hallHr){
        if(srvHs.attached() == false){
            srvHs.attach();
        }
        Serial.println("Testing Hammerfunction");
        
        Serial.println("Servo to pos1");
        srvHs.write(pos::HS_1);
        delay(DELAY);
        
        Serial.println("MP6550 running");
        moHr.run(255);
        delay(DELAY);
        
        Serial.println("MP6550 waiting for stop signal");
        esGe.waitUntil(1);
        
        Serial.println("MP6550 breaking");
        moHr.brake();
        delay(DELAY);
        
        Serial.println("Servo to pos2");
        srvHs.write(pos::HS_2);
        delay(3*DELAY);
        
        Serial.println("Servo to pos1");
        srvHs.write(pos::HS_1);
        delay(DELAY);
        
        Serial.println("MP6550 running in other direction");
        moHr.run(-255);
        delay(DELAY);
        
        Serial.println("MP6550 braking");
        moHr.brake();
    }

    void TestSchlitten(MP6550 moSl, ServoExp srvKu, Endstop esSl){
        if(srvKu.attached() == false){
            srvKu.attach();
        }
        Serial.println("Testing Schlitten");
        
        int speed = 255;
        
        if(esSl.read() != 0){
            if(esSl.read() == 1){
                Serial.println("Schlitten ist links");
                delay(DELAY);
                moSl.run(speed);
            }
            else if(esSl.read() == 2){
                Serial.println("Schlitten ist rechts");
                delay(DELAY);
                moSl.run(-speed);
            }
        }
        else{
            Serial.println("Kein Endschalter erreicht");
            delay(DELAY);
            moSl.run(speed);
            while(esSl.read() == 0){
                delay(10);
            }
            moSl.brake();
            delay(DELAY);
        }

    }*/

    void TestSchild(AccelStepper step, Hall hallSh){
        int speed = 100;
        Serial.println("Testing Schild");

        Serial.println("Stepper to Home");
        while(hallSh.read() != 0){
            step.setSpeed(speed);
            step.run();
        }
        step.setCurrentPosition(0);
        delay(DELAY);

        Serial.println("Stepper to 2");
        while(step.currentPosition() != pos::STP_2){
            step.setSpeed(speed);
            step.run();
        }
        delay(DELAY);
        
        Serial.println("Stepper to 3");
        while(step.currentPosition() != pos::STP_3){
            step.setSpeed(speed);
            step.run();
        }
        delay(2*DELAY);
        
    }

} //!namespace testing