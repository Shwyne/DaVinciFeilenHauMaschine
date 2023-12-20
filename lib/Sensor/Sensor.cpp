#include "Sensor.hpp"

namespace Sensor{

    HallSwitch::HallSwitch(uint8_t pin, bool triggeredIf) {
        pin_ = pin;
        pinMode(pin_, INPUT);
        triggered_ = triggeredIf;
    }

    void HallSwitch::setTriggeredIf(bool triggeredIf) {
        triggered_ = triggeredIf;
    }

    bool HallSwitch::read() {
        return (digitalRead(pin_) == triggered_);
    }

    void HallSwitch::printData(){
        Serial.println("\nHall-Sensor-Data:");
        Serial.println("----------------------");
        Serial.print("Pin: ");
        Serial.println(pin_);
        Serial.print("Is triggered if  ");
        Serial.println((triggered_ == HIGH) ? "HIGH" : "LOW");
        Serial.println((read() == triggered_) ? "Triggered" : "Untriggered");
        Serial.println("-----------------\n");
    }

    Endstops::Endstops(uint8_t pin1, uint8_t pin2, bool triggeredIf) {
        pin1_ = pin1;
        pin2_ = pin2;
        triggered_ = triggeredIf;
        if(triggered_ == LOW){
            pinMode(pin1_, INPUT_PULLUP);
            pinMode(pin2_, INPUT_PULLUP);
        }
        else{
            pinMode(pin1_, INPUT);
            pinMode(pin2_, INPUT);
        }
    }

    int8_t Endstops::read() {
        
        if(digitalRead(pin1_) == triggered_) {
            return 1;
        } else if(digitalRead(pin2_) == triggered_) {
            return 2;
        } else {
            return 0;
        }
    }


    void Endstops::printData(){
        Serial.println("\nEndstop-Data:");
        Serial.println("----------------------");
        Serial.println("Pin 1: ");
        Serial.println(pin1_);
        Serial.println("Pin 2: ");
        Serial.println(pin2_);
        Serial.println("Mode: ");
        Serial.println(triggered_);
        Serial.print("State: ");
        Serial.println(read() == 0 ? "Untriggered" : read() == 1 ? "Triggered Pin 1" : "Triggered Pin 2");
        Serial.println("----------------------\n");
        Serial.println("End of Data");
        Serial.println("-----------------");
    }

    Button::Button(uint8_t pin, uint8_t LED, bool triggeredIf) {
        if(LED!=255){
            pinMode(LED, OUTPUT);
            digitalWrite(LED, LOW);
        }
        pin_ = pin;
        triggered_ = triggeredIf;
        if(triggered_ == 0){
            pinMode(pin_, INPUT_PULLUP);
        }
        else{
            pinMode(pin_, INPUT);
        }
    }

    bool Button::read() {
        if(digitalRead(pin_) == triggered_) {
            return true;
        } else {
            return false;
        }
    }

    void Button::printData(){
        Serial.println("\nButton-Data:");
        Serial.println("----------------------");
        Serial.print("Pin: ");
        Serial.println(pin_);
        Serial.print("Mode: ");
        Serial.println(triggered_);
        Serial.println(read() == triggered_ ? "Triggered" : "Untriggered");
        Serial.println("----------------------\n");
        Serial.println("End of Data");
        Serial.println("-----------------");
    }

    void Button::toggleLED(){
        digitalWrite(pin_, !digitalRead(pin_));
    }
}