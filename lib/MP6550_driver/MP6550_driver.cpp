/* 
		Library for MP6550 DC-Motordriver
*/

#include "MP6550_driver.hpp"

motor::motor(int IN1pin, int IN2pin, int SLPpin){
	
 	IN1 = IN1pin;
	IN2 = IN2pin;
	SLP = SLPpin;
	
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
	pinMode(SLP, OUTPUT);
	
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	digitalWrite(SLP, LOW);
	speed_ = 0;
	breakForce_ = 0;
}

void motor::run(int speed){
	if(speed > 255) speed = 255;
	else if(speed < -255) speed = -255;

	if(digitalRead(SLP) == LOW) digitalWrite(SLP, HIGH);
	speed_ = speed;
	if (speed_ > 0) {
		
		digitalWrite(IN1, HIGH);
		analogWrite(IN2, 255-speed_);
	}
	else if (speed_ < 0) {
		digitalWrite(IN2, HIGH);
		analogWrite(IN1, 255+speed_);
	}
}

void motor::brake(int breakForce){
	if(breakForce > 255) breakForce = 255;
	else if(breakForce < -255) breakForce = -255;

	if(digitalRead(SLP) == HIGH){ 
		breakForce_ = breakForce;
		if(breakForce_ == 0){
			digitalWrite(IN1, LOW);
			digitalWrite(IN2, LOW);
		}
		else if(breakForce_ == 1){
			digitalWrite(IN1, HIGH);
			digitalWrite(IN2, HIGH);
		}
		else if (breakForce_ > 1) {
			digitalWrite(IN1, HIGH);
			analogWrite(IN2, breakForce_);
		}
		else if (breakForce_ < 0) {
			digitalWrite(IN2, HIGH);
			analogWrite(IN1, breakForce_);
		}
		delay(100);
	}
}

int motor::getSpeed(){
	return speed_;
}

int motor::getBrakeForce(){
	return breakForce_;
}

bool motor::sleepState(){
	if(digitalRead(SLP) == HIGH){
		return false;
	}
	else{
		return true;
	}
}

void motor::sleep(){
	brake(0);
	digitalWrite(SLP,LOW);
}

void motor::printData(){
	Serial.println("\nMotor-Data:");
	Serial.println("----------------------");
	Serial.println("IN1: " + String(IN1));
	Serial.println("IN2: " + String(IN2));
	Serial.println("SLP: " + String(SLP));
	Serial.println("Speed: " + String(speed_));
	Serial.println("BrakeForce: " + String(breakForce_));
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
	
motor::~motor(){
	brake(0);
	delay(500);
	digitalWrite(SLP, HIGH);
}

motorVisen::motorVisen(int IN1pin, int IN2pin, int SLPpin, int VISpin) : motor(IN1pin, IN2pin, SLPpin), VIS(VISpin) {
    // constructor implementation
	pinMode(VIS, INPUT);

	CuRatio_ = 0.5;
	Visen_ = 0;
}

void motorVisen::setCurrentRatio(int ratio){
	CuRatio_ = ratio;
}

double motorVisen::getCurrent(){	//! Calculations are still wrong!
	Visen_ = analogRead(VIS);
	Visen_ = (Visen_/1023)*5;
	Visen_ = Visen_/CuRatio_;
	return Visen_;
}

motorVisen::~motorVisen(){
	brake(1);
	delay(500);
	sleep();
}

