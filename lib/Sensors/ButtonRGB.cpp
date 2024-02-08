#include "Sensors.hpp"

namespace Sensor {

Button::Button(uint8_t Butpin, uint8_t Rpin, uint8_t Gpin, uint8_t Bpin) {
    this->Butpin = Butpin;
    this->Rpin = Rpin;
    this->Gpin = Gpin;
    this->Bpin = Bpin;

    pinMode(Butpin, INPUT_PULLUP);
    pinMode(Rpin, OUTPUT);
    pinMode(Gpin, OUTPUT);
    pinMode(Bpin, OUTPUT);

    digitalWrite(Rpin, LOW);
    digitalWrite(Gpin, LOW);
    digitalWrite(Bpin, LOW);
    return;
}

bool Button::read() {
    if(digitalRead(Butpin) == LOW){
        triggered = true;
    }
    else{
        triggered = false;
    }
    return triggered;
}

bool Button::changed() {
    bool prevState = triggered;
    if(read() != prevState){
        return true;
    }
    return false;
}

void Button::updateLED(uint8_t mode){
    switch(mode){
        case 0: //off
            red = 0;
            green = 0;
            blue = 0;
            break;
        case 1: //red
            red = 255;
            green = 0;
            blue = 0;
            break;
        case 2: //green
            red = 0;
            green = 255;
            blue = 0;
            break;
        case 3: //blue
            red = 0;
            green = 0;
            blue = 255;
            break;
        case 4: //yellow
            red = 255;
            green = 255;
            blue = 0;
            break;
        case 5: //cyan
            red = 0;
            green = 255;
            blue = 255;
            break;
        case 6: //magenta
            red = 255;
            green = 0;
            blue = 255;
            break;
        case 7: //white
            red = 255;
            green = 255;
            blue = 255;
            break;
        default:
            red = 0;
            green = 0;
            blue = 0;
            break;
    digitalWrite(Rpin, red==255);
    digitalWrite(Gpin, green==255);
    digitalWrite(Bpin, blue==255);
    return;
  }
}

void Button::updateLED(uint8_t red, uint8_t green, uint8_t blue){
    this->red = red;
    this->green = green;
    this->blue = blue;
    analogWrite(Rpin, this->red);
    analogWrite(Gpin, this->green);
    analogWrite(Bpin, this->blue);
    return;
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

} 