#include "Sensors.hpp"

namespace Sensor {

//* Constructor for Button class (Only Butpin is required, rest is optional)
Button::Button(uint8_t Butpin, uint8_t Rpin, uint8_t Gpin, uint8_t Bpin) {
    this->Butpin = Butpin;  // Pin for the Button
    this->Rpin = Rpin;      // Pin for the Red LED
    this->Gpin = Gpin;      // Pin for the Green LED
    this->Bpin = Bpin;      // Pin for the Blue LED

    // pin setup:
    pinMode(Butpin, INPUT_PULLUP);  // Set Butpin as input with pullup
    pinMode(Rpin, OUTPUT);          // Set Rpin as output
    pinMode(Gpin, OUTPUT);          // Set Gpin as output
    pinMode(Bpin, OUTPUT);          // Set Bpin as output

    // Turn LED OFF:
    digitalWrite(Rpin, LOW);        // Set Rpin to LOW
    digitalWrite(Gpin, LOW);        // Set Gpin to LOW
    digitalWrite(Bpin, LOW);        // Set Bpin to LOW
    return;
}

//* Read the Button and returns the state
bool Button::read() {
    // If the Button is pressed, set triggered to true
    if(digitalRead(Butpin) == LOW){
        triggered = true;
    }
    // If the Button is not pressed, set triggered to false
    else{
        triggered = false;
    }
    // Return the state of the Button
    return triggered;
}

//* Check if the state of the Button has changed, returns true if changed
bool Button::changed() {
    // Save the previous state of the Button
    bool prevState = triggered;
    // If the state of the Button has changed, return true
    if(read() != prevState){
        return true;
    }
    // If the state of the Button has not changed, return false
    return false;
}

//* Wait for the Button to be pressed, blocks the programm!
void Button::waitForPress() {
    // Wait for the Button to be pressed
    while(this->read() != true){
        delay(1);
    }
    // Wait for the Button to be released
    while(this->read() != false){
        delay(1);
    }
    // Return
    return;
}

//* Update the LED of the Button (By Number)
void Button::updateLED(uint8_t mode){
//(0 = off, 1 = red, 2 = green, 3 = blue, 4 = yellow, 5 = cyan, 6 = magenta, 7 = white)
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

//* Update the LED of the Button with custom RGB values, needs PWM pins
void Button::updateLED(uint8_t red, uint8_t green, uint8_t blue){
    if(Rpin != 255) analogWrite(Rpin, red);     // Set the brightness of the Red LED, if Rpin is defined
    if(Gpin != 255) analogWrite(Gpin, green);   // Set the brightness of the Green LED, if Gpin is defined
    if(Bpin != 255) analogWrite(Bpin, blue);    // Set the brightness of the Blue LED, if Bpin is defined
    return;
}

} 