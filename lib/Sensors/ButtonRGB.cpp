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
        if(Rpin != 255) digitalWrite(Rpin, LOW);
        if(Gpin != 255) digitalWrite(Gpin, LOW);
        if(Bpin != 255) digitalWrite(Bpin, LOW);
        break;
    case 1: //RED
        if(Rpin != 255) digitalWrite(Rpin, HIGH);
        if(Gpin != 255) digitalWrite(Gpin, LOW);
        if(Bpin != 255) digitalWrite(Bpin, LOW);
        break;
    case 2: //GREEN
        if(Rpin != 255) digitalWrite(Rpin, LOW);
        if(Gpin != 255) digitalWrite(Gpin, HIGH);
        if(Bpin != 255) digitalWrite(Bpin, LOW);
        break;
    case 3: //BLUE
        if(Rpin != 255) digitalWrite(Rpin, LOW);
        if(Gpin != 255) digitalWrite(Gpin, LOW);
        if(Bpin != 255) digitalWrite(Bpin, HIGH);
        break;
    case 4: //YELLOW
        if(Rpin != 255 && Gpin != 255) {
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, HIGH);
        } else {
            updateLED(0); // turn off the LED if either pin is not defined
        }
        if(Bpin != 255) digitalWrite(Bpin, LOW);
        break;
    case 5: //CYAN
        if(Gpin != 255 && Bpin != 255) {
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, HIGH);
        } else {
            updateLED(0); // turn off the LED if either pin is not defined
        }
        if(Rpin != 255) digitalWrite(Rpin, LOW);
        break;
    case 6: //MAGENTA
        if(Rpin != 255 && Bpin != 255) {
            digitalWrite(Rpin, HIGH);
            digitalWrite(Bpin, HIGH);
        } else {
            updateLED(0); // turn off the LED if either pin is not defined
        }
        if(Gpin != 255) digitalWrite(Gpin, LOW);
        break;
    case 7: //WHITE
        if(Rpin != 255 && Gpin != 255 && Bpin != 255) {
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, HIGH);
        } else {
            updateLED(0); // turn off the LED if any pin is not defined
        }
        break;
    default:    //DEFAULT: OFF
        updateLED(0);
        break;
    }
}

void Button::updateLED(uint8_t red, uint8_t green, uint8_t blue){
    if(Rpin != 255) analogWrite(Rpin, red);
    if(Gpin != 255) analogWrite(Gpin, green);
    if(Bpin != 255) analogWrite(Bpin, blue);
    return;
}

} 