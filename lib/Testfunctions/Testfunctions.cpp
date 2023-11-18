#include "Testfunctions.hpp"

#define DELAY 2000  //Default delay for testing purposes

namespace testing{  //!Namespace for testing functions

    void TestDC(motor mdc){
        Serial.println("Testing DC-Motor");
        Serial.println("Testing run");
        mdc.run(255);
        delay(DELAY);
        mdc.run(-255);
        delay(DELAY);
        Serial.println("Testing brake");
        mdc.brake(255);
        delay(DELAY);
        mdc.run(255);
        delay(DELAY);
        mdc.brake(0);
        delay(DELAY);
        mdc.run(-255);
        delay(DELAY);
        mdc.brake(127);
        Serial.println("Testing sleep");
        mdc.sleep();
        delay(2*DELAY);
    }

    void ServoData(CustomServo srv){
        if(srv.attached() == false){
            srv.attach();
        }
        Serial.print("Current Position: ");
        Serial.println(srv.read());
        Serial.print("Target Position: ");
        Serial.println(srv.getPosTarg());
        Serial.print("Min Position: ");
        Serial.println(srv.getMin());
        Serial.print("Max Position: ");
        Serial.println(srv.getMax());
        Serial.print("Pos1: ");
        Serial.println(srv.getPos1());
        Serial.print("Pos2: ");
        Serial.println(srv.getPos2());
    }

    void TestServo(CustomServo srv){
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

    void TestServo(CustomServo srv, Endstop es){
        if(srv.attached() == false){
            srv.attach();
        }
        Serial.println("Testing Servo");
        Serial.println("Servo to pos1 - Waiting for Signal");
        while(es.getState() != 1){
            delay(10);
        }
        srv.run(pos::HS_1);
        delay(2*DELAY);
        Serial.println("Servo to pos2 - Waiting for Signal");
        while(es.getState() != 1){
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
        
        Serial.println("Motor running");
        moHr.run(255);
        delay(DELAY);
        
        Serial.println("Motor waiting for stop signal");
        esGe.waitUntil(1);
        
        Serial.println("Motor breaking");
        moHr.brake(255);
        delay(DELAY);
        
        Serial.println("Servo to pos2");
        srvHs.write(pos::HS_2);
        delay(3*DELAY);
        
        Serial.println("Servo to pos1");
        srvHs.write(pos::HS_1);
        delay(DELAY);
        
        Serial.println("Motor running in other direction");
        moHr.run(-255);
        delay(DELAY);
        
        Serial.println("Motor braking");
        moHr.brake(255);
    }

    void TestSchlitten(motor moSl, CustomServo srvKu, Endstop esSl){
        if(srvKu.attached() == false){
            srvKu.attach();
        }
        Serial.println("Testing Schlitten");
        
        int speed = 255;
        
        if(esSl.getState() != 0){
            if(esSl.getState() == 1){
                Serial.println("Schlitten ist links");
                delay(DELAY);
                moSl.run(speed);
            }
            else if(esSl.getState() == 2){
                Serial.println("Schlitten ist rechts");
                delay(DELAY);
                moSl.run(-speed);
            }
        }
        else{
            Serial.println("Kein Endschalter erreicht");
            delay(DELAY);
            moSl.run(speed);
            while(esSl.getState() == 0){
                delay(10);
            }
            moSl.brake(speed);
            delay(DELAY);
        }

    }

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