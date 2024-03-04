#include "Drivers.hpp"

/* Table for Running this driver:

1. Input Logic Truth Table:
  IN1  |  IN2  | OUT1  | OUT2  | Function
----------------------------------------------
   L   |   L   |   Z   |   Z   | Coast
   L   |   H   |   L   |   H   | Reverse
   H   |   L   |   H   |   L   | Forward
   H   |   H   |   L   |   L   | Brake

  Z = High Impedance
  L = Low
  H = High

2. PWM Input Control
  IN1  |  IN2  |  OUT1  |  OUT2  | Function
----------------------------------------------
   H   | PWM L |   H    |   L    | Forward
   H   | PWM H |   L    |   L    | Slow Decay
 PWM L |   H   |   L    |   H    | Reverse
 PWM H |   H   |   L    |   L    | Slow Decay

  PWM = Pulse Width Modulation: 0-255

3. Sleep Mode
  SLP = LOW: Standby
  SLP = HIGH: Active

*/

MP6550::MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin, bool reversed, bool autoSleep) {
  this->IN1pin = IN1pin;
  this->IN2pin = IN2pin;
  this->SLPpin = SLPpin;
  this->autoSleep = autoSleep;
  this->reversed = reversed;
  this->speed = 0;

  pinMode(this->IN1pin, OUTPUT);
  pinMode(this->IN2pin, OUTPUT);
  digitalWrite(this->IN1pin, LOW);
  digitalWrite(this->IN2pin, LOW);

  if (this->SLPpin != 255) {
    pinMode(this->SLPpin, OUTPUT);
    digitalWrite(this->SLPpin, HIGH);
  }
  return;
}

void MP6550::run(int speed) {
  // Checks if driver in sleep mode (SLP == LOW) and if SLP is used (SLP != 255)
  if(SLPpin != 255 && sleepState() == Status::Standby) {
    this->wake();
  }
  // constraints input speed to -255 to 255, because of 8bit PWM control
  this->speed = constrain(speed, -255, 255);
  // 
  if ((this->speed > 0 && !reversed) || (this->speed < 0 && reversed)) {
    analogWrite(IN1pin, abs(this->speed));
    digitalWrite(IN2pin, LOW);
  } 
  else if ((this->speed < 0 && !reversed) || (this->speed > 0 && reversed)) {
    digitalWrite(IN1pin, LOW);
    analogWrite(IN2pin, abs(this->speed));
  }
  else {
    digitalWrite(IN1pin, LOW);
    digitalWrite(IN2pin, LOW);
  }
  return;
}

void MP6550::brake() {
  digitalWrite(IN1pin, HIGH);
  digitalWrite(IN2pin, HIGH);
  speed = 0;
  delay(500);
  digitalWrite(IN1pin, LOW);
  digitalWrite(IN2pin, LOW);
  if(autoSleep) {
    this->sleep();
  }
  return;
}

void MP6550::sleep() {
  if (SLPpin != 255) {
    if (speed != 0)
      this->brake();
    digitalWrite(SLPpin, LOW);
  }
  return;
}

void MP6550::wake() {
  if (SLPpin != 255) {
    digitalWrite(SLPpin, HIGH);
  }
  return;
}

int MP6550::getSpeed() {
   return speed;
}

Status MP6550::sleepState() {
  if (SLPpin != 255) {
    if(digitalRead(SLPpin) == HIGH) {
      return Status::Active;
    }
    else {
      return Status::Standby;
    }
  }
  return Status::Active;
}

MP6550::~MP6550() {
  this->brake();
  return;
}