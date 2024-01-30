#include "Sensor.hpp"

namespace Sensor {

HallSwitch::HallSwitch(uint8_t pin, bool triggeredIf) {
  pin_ = pin;
  pinMode(pin_, INPUT);
  triggered_ = triggeredIf;
}

void HallSwitch::setTriggeredIf(bool triggeredIf) { triggered_ = triggeredIf; }

bool HallSwitch::read() { return (digitalRead(pin_) == triggered_); }

void HallSwitch::printData() {
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
  if (triggered_ == LOW) {
    pinMode(pin1_, INPUT_PULLUP);
    pinMode(pin2_, INPUT_PULLUP);
  } else {
    pinMode(pin1_, INPUT);
    pinMode(pin2_, INPUT);
  }
}

int8_t Endstops::read() {

  if (digitalRead(pin1_) == triggered_) {
    return 1;
  } else if (digitalRead(pin2_) == triggered_) {
    return 2;
  } else {
    return 0;
  }
}

void Endstops::printData() {
  Serial.println("\nEndstop-Data:");
  Serial.println("----------------------");
  Serial.println("Pin 1: ");
  Serial.println(pin1_);
  Serial.println("Pin 2: ");
  Serial.println(pin2_);
  Serial.println("Mode: ");
  Serial.println(triggered_);
  Serial.print("State: ");
  Serial.println(read() == 0   ? "Untriggered"
                 : read() == 1 ? "Triggered Pin 1"
                               : "Triggered Pin 2");
  Serial.println("----------------------\n");
  Serial.println("End of Data");
  Serial.println("-----------------");
}

Button::Button(uint8_t Butpin, uint8_t Rpin, uint8_t Gpin, uint8_t Bpin) {
  if(Butpin != 255){
    this->Butpin = Butpin;
    pinMode(Butpin, INPUT_PULLUP);
  }
  if(Rpin != 255){
    this->Rpin = Rpin;
    pinMode(Rpin, OUTPUT);
    digitalWrite(Rpin, LOW);
  }
  if(Gpin != 255){
    this->Gpin = Gpin;
    pinMode(Gpin, OUTPUT);
    digitalWrite(Gpin, LOW);
  }
  if(Bpin != 255){
    this->Bpin = Bpin;
    pinMode(Bpin, OUTPUT);
    digitalWrite(Bpin, LOW);
  }
  return;
}

bool Button::read() {
  return !digitalRead(Butpin);
}

void Button::printData() {
  Serial.println("\nButton-Data:");
  Serial.println("----------------------");
  Serial.print("But-Pin: ");
  Serial.println(Butpin);
  Serial.print("R-Pin: ");
  Serial.println(Rpin);
  Serial.print("G-Pin: ");
  Serial.println(Gpin);
  Serial.print("B-Pin: ");
  Serial.println(Bpin);
  Serial.println(read()? "Triggered" : "Untriggered");
  Serial.println("----------------------\n");
  Serial.println("End of Data");
  Serial.println("-----------------");
  return;
}

void Button::updateLED(uint8_t mode){
  switch(mode){
    case 0: //off
        digitalWrite(Rpin, LOW);
        digitalWrite(Gpin, LOW);
        digitalWrite(Bpin, LOW);
        break;
      case 1: //red
        digitalWrite(Rpin, HIGH);
        digitalWrite(Gpin, LOW);
        digitalWrite(Bpin, LOW);
        break;
      case 2: //green
        digitalWrite(Rpin, LOW);
        digitalWrite(Gpin, HIGH);
        digitalWrite(Bpin, LOW);
        break;
      case 3: //blue
        digitalWrite(Rpin, LOW);
        digitalWrite(Gpin, LOW);
        digitalWrite(Bpin, HIGH);
        break;
      case 4: //yellow
        digitalWrite(Rpin, HIGH);
        digitalWrite(Gpin, HIGH);
        digitalWrite(Bpin, LOW);
        break;
      case 5: //cyan
        digitalWrite(Rpin, LOW);
        digitalWrite(Gpin, HIGH);
        digitalWrite(Bpin, HIGH);
        break;
      case 6: //magenta
        digitalWrite(Rpin, HIGH);
        digitalWrite(Gpin, LOW);
        digitalWrite(Bpin, HIGH);
        break;
      case 7: //white
        digitalWrite(Rpin, HIGH);
        digitalWrite(Gpin, HIGH);
        digitalWrite(Bpin, HIGH);
        break;
      default:
        digitalWrite(Rpin, LOW);
        digitalWrite(Gpin, LOW);
        digitalWrite(Bpin, LOW);
        break;
    
  }
}

void Button::updateLED(uint8_t red, uint8_t green, uint8_t blue){
  analogWrite(Rpin, red);
  analogWrite(Gpin, green);
  analogWrite(Bpin, blue);
  return;
}

} // namespace Sensor