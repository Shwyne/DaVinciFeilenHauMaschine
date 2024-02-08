#include "Drivers.hpp"

MP6550::MP6550(uint8_t IN1pin, uint8_t IN2pin, uint8_t SLPpin) {

  IN1 = IN1pin;
  IN2 = IN2pin;
  SLP = SLPpin;
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  if (SLP != 255) {
    pinMode(SLP, OUTPUT);
    this->wake();
  }
  return;
}

void MP6550::run(int speed) {
  // Checks if driver in sleep mode (SLP == LOW) and if SLP is used (SLP != 255)
  if (sleepState() == Status::Standby) {
    this->wake();
  }
  // constraints input speed to -255 to 255, because of 8bit PWM control
  speed_ = constrain(speed, -255, 255);

  if (speed_ > 0) {
    analogWrite(IN1, abs(speed_));
    digitalWrite(IN2, LOW);
  } else if (speed_ < 0) {
    digitalWrite(IN1, LOW);
    analogWrite(IN2, abs(speed_));
  }
  return;
}

void MP6550::brake() {
  brake(255); // Call the other version of the brake function with full force
  return;
}

void MP6550::brake(int brakeForce) {
  // Checks if driver in sleep mode (SLP == LOW) and if SLP is used (SLP != 255)
  if (sleepState() == Status::Standby) {
    this->wake();
  }
  // constraints input brakeForce to 0 to 255, because of 8bit PWM control
  brakeForce = constrain(brakeForce, 0, 255);
  // deciding which direction to brake
  if (speed_ > 0) {
    digitalWrite(IN1, HIGH);
    analogWrite(IN2, 255 - brakeForce);
  } else if (speed_ < 0) {
    digitalWrite(IN2, HIGH);
    analogWrite(IN1, 255 - brakeForce);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  // TODO: Testing with hardware
  delay(50);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  speed_ = 0;
  delay(50);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  return;
}

void MP6550::coast() {

  if ((speed_ == 0) || (sleepState() == Status::Standby))
    return;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(50);
  return;
}

void MP6550::sleep() {
  if (SLP != 255) {
    if (speed_ != 0)
      brake();
    digitalWrite(SLP, LOW);
  }
  return;
}

void MP6550::wake() {
  if (SLP != 255) {
    digitalWrite(SLP, HIGH);
  }
  return;
}

int MP6550::getSpeed() { return speed_; }

Status MP6550::sleepState() {
  if (SLP != 255) {
    return (digitalRead(SLP)) ? Status::Active : Status::Standby;
  }
  return Status::Active;
}

void MP6550::printData() {
  Serial.println("\nMP6550-Data:");
  Serial.println("----------------------");
  Serial.print(" IN1: ");
  Serial.println(IN1);
  Serial.print(" IN2: ");
  Serial.println(IN2);
  if (SLP != 255) {
    Serial.print(" SLP: ");
    Serial.println(SLP);
  }
  Serial.print(" Speed: ");
  Serial.println(speed_);
  if (sleepState() == Status::Standby) {
    Serial.println(" Status: Standby");
  } else {
    Serial.println(" Status: Active");
  }
  Serial.println("----------------------\n");
  return;
}

MP6550::~MP6550() {
  brake();
  if (SLP != 255) {
    delay(500);
    digitalWrite(SLP, HIGH);
  }
  return;
}