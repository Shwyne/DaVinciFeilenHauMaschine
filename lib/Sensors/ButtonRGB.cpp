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

void Button::waitForPress() {
    while(this->read() != true){
        delay(1);
    }
    while(this->read() != false){
        delay(1);
    }
    return;
}

void Button::updateLED(uint8_t mode){
    switch(mode){
        case 0: //OFF
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, LOW);
            break;
        case 1: //RED
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, LOW);
            break;
        case 2: //GREEN
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, LOW);
            break;
        case 3: //BLUE
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, HIGH);
            break;
        case 4: //YELLOW
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, LOW);
            break;
        case 5: //CYAN
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, HIGH);
            break;
        case 6: //MAGENTA
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, HIGH);
            break;
        case 7: //WHITE
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, HIGH);
            break;
        default:    //DEFAULT: OFF
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, LOW);
            break;
    }
    return;
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