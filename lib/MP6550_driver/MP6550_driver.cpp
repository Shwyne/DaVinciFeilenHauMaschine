/* 
		Library for MP6550 DC-Motor-driver
*/

#include "MP6550_driver.hpp"

MP6550::MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin) {
    
    IN1 = IN1pin;
    IN2 = IN2pin;
    SLP = SLPpin;

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(SLP, OUTPUT); 
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(SLP, HIGH);
}

void MP6550::run(int speed){
    
    if(!digitalRead(SLP)) digitalWrite(SLP, HIGH);
    
    if(speed > 255) speed_ = 255;
    else if(speed < -255) speed_ = -255;
    else speed_ = speed;

    if(speed_ > 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        analogWrite(IN2, abs(speed_));
    }
    else if(speed_ < 0) {
        digitalWrite(IN2, LOW);
        digitalWrite(IN1, LOW);
        analogWrite(IN1, abs(speed_));
    }
}

void MP6550::brake(){
      
    if (speed_ == 0 || !digitalRead(SLP)) return;
        
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);

}

void MP6550::coast(){
    
    if (speed_ == 0 || !digitalRead(SLP)) return;
    
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
}

void MP6550::sleep(){
    brake();
	digitalWrite(SLP, LOW);
}

int MP6550::getSpeed(){
    return speed_;
}

bool MP6550::sleepState(){
	 return (digitalRead(SLP) == HIGH) ? false: true;
}

void MP6550::printData(){
	Serial.println("\nMP6550-Data:");
	Serial.println("----------------------");
	Serial.println("IN1: " + String(IN1));
	Serial.println("IN2: " + String(IN2));
	Serial.println("SLP: " + String(SLP));
	Serial.println("Speed: " + String(speed_));
	if(sleepState() == true){
		Serial.println("SleepState: Sleep");
	}
	else{
		Serial.println("SleepState: Active");
	}
	Serial.println("----------------------\n");
	Serial.println("End of Data");
	Serial.println("-----------------");
}
	
MP6550::~MP6550(){
	brake();
	delay(100);
	digitalWrite(SLP, HIGH);
}