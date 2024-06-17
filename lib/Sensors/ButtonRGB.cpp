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
    isPressed = (digitalRead(Butpin) == LOW);
    // Return the state of the Button
    return isPressed;
}

//* Check if the state of the Button has changed, returns true if changed
bool Button::changed() {
    // Save the previous state of the Button
    bool prevState = isPressed;
    // If the state of the Button has changed, return true
    return (read() != prevState);
}

//* Wait for the Button to be pressed and released (Blocking)
void Button::waitForPress() { 
    // Wait for the Button to be pressed
    while(this->read() != true) delay(1);
    // Wait for the Button to be released
    while(this->read() != false) delay(1);
    // Return
    return;
}

//* Update the LED of the Button (By Number)
void Button::updateLED(uint8_t mode) {
    //Mode: 0 = OFF, 1 = RED, 2 = GREEN, 3 = BLUE, 4 = YELLOW, 5 = CYAN, 6 = MAGENTA, 7 = WHITE
    switch (mode) {
        case 0: // OFF
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, LOW);
            break;
        case 1: // RED
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, LOW);
            break;
        case 2: // GREEN
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, LOW);
            break;
        case 3: // BLUE
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, HIGH);
            break;
        case 4: // YELLOW
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, LOW);
            break;
        case 5: // CYAN
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, HIGH);
            break;
        case 6: // MAGENTA
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, HIGH);
            break;
        case 7: // WHITE
            digitalWrite(Rpin, HIGH);
            digitalWrite(Gpin, HIGH);
            digitalWrite(Bpin, HIGH);
            break;
        default: // OFF
            digitalWrite(Rpin, LOW);
            digitalWrite(Gpin, LOW);
            digitalWrite(Bpin, LOW);
            break;
    }
}

//* Update the LED of the Button with custom RGB values, needs PWM pins
void Button::updateLED(uint8_t red, uint8_t green, uint8_t blue){
    // 255^3-1 = 16.581.375 colors
    if(Rpin != 255) analogWrite(Rpin, red);     // Set the brightness of the Red LED, if Rpin is defined
    if(Gpin != 255) analogWrite(Gpin, green);   // Set the brightness of the Green LED, if Gpin is defined
    if(Bpin != 255) analogWrite(Bpin, blue);    // Set the brightness of the Blue LED, if Bpin is defined
    return;
}

} 