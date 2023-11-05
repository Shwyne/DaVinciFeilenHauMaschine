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
	dir_ = 0;
	br_speed_ = 255;
}

void motor::forwards(){
	if(digitalRead(SLP) == 0)wakeUp();
	digitalWrite(IN1, HIGH);
	analogWrite(IN2, speed_);
	dir_	=  1;
}

void motor::forwards(int speed){
	setSpeed(speed);
	forwards();
}

void motor::backwards(){
	if(digitalRead(SLP) == 0)wakeUp();
	digitalWrite(IN2, HIGH);
	analogWrite(IN1, speed_);
	dir_ = -1;
}

void motor::backwards(int speed){
	setSpeed(speed);
	backwards();
}

void motor::brake(){
	if(dir_ == 1){
		digitalWrite(IN1, HIGH);
		analogWrite(IN2, br_speed_);
	}
	else if(dir_ == -1){
		analogWrite(IN1, br_speed_);
		digitalWrite(IN2, HIGH);
  }
	else{
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, HIGH);
	}
	delay(100);
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	dir_ = 0;
}

void motor::brake(int br_speed){
	setBrakeSpeed(br_speed);
	brake();
}

void motor::setBrakeSpeed(int br_speed){
	if(br_speed>255)br_speed = 255;
	else if(br_speed<0)br_speed = 0;
	
	br_speed_ = br_speed;
}

void motor::toggleDir(){
	if(digitalRead(SLP) ==	0){
		if(dir_ == 1) backwards();
		else if(dir_ == -1) forwards();
		else brake();
 }
	else dir_ =	-dir_;
}

void motor::goDirection(int dir){
	if(dir>1)dir = 1;
	else if(dir<-1)dir = -1;
	
	dir_ = dir;
	if(dir_ == 1)forwards();
	else if(dir_ == -1)backwards();
	else brake();
}
		
void motor::setSpeed(int speed){
	if(speed>255)speed =	255;
	else if(speed<0)speed = 0;
	speed_ =	255- speed;	//One Pin High and One Pin Low is highest speed - thus 255 - speed -> speed input = 255 -> speed_ = 0 -> max speed
}

int motor::getSpeed(){
	return 255-speed_;
}

int motor::getBrakeSpeed(){
	return br_speed_;
}

int motor::getStbyStat(){
	return digitalRead(SLP);
}

void motor::sleep(){
	brake();
	digitalWrite(SLP,LOW);
}

void motor::sleep(int secs){
	brake();
	digitalWrite(SLP, LOW);
	delay(secs*1000);
	wakeUp();
}

void motor::wakeUp(){
	digitalWrite(SLP, HIGH);
}

void motor::run(int secs){
	if(dir_ == 1)forwards();
	else if(dir_ == -1)backwards();
	else brake();
	delay(secs*1000);
	brake();
}
	
motor::~motor(){
	brake();
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
	brake();
	delay(500);
	digitalWrite(SLP, HIGH);
}

