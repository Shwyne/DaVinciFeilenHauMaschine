#include "Testfunctions.hpp"

// TODO: Updating functions to library changes

namespace testComp { //*Testfunctions for components

void TestDC(DC_Motor_Driver::MP6550 mdc) {
  Serial.println("\nTEST: DC-Motor");
  Serial.println("----------------------");
  mdc.printData();
  delay(1000);
  Serial.println("Running motor from 0 to 255");
  for (int i = 0; i <= 255; i++) {
    mdc.run(i);
    delay(10);
  }
  delay(1000);
  Serial.println("Running motor from 255 to 0");
  for (int i = 255; i >= 0; i--) {
    mdc.run(i);
    delay(10);
  }
  delay(1000);
  Serial.println("Running motor to full speed then coasting");
  mdc.run(255);
  delay(1000);
  mdc.coast();
  delay(1000);
  Serial.println("Running motor to full speed then braking");
  mdc.run(255);
  delay(1000);
  mdc.brake();
  delay(1000);
  Serial.println("Disabling motor");
  mdc.disable();
  delay(1000);
  Serial.println("TEST: Done.");
  Serial.println("----------------------\n");
  return;
}

void TestServo(ServoExp srv, uint8_t pos1, uint8_t pos2) {
  if (srv.attached() == false) {
    srv.attach();
  }
  Serial.println("Test: Servo");
  Serial.println("----------------------");
  srv.printData();
  delay(1000);
  Serial.println("Servo to pos1");
  srv.write(pos1);
  delay(5000);
  Serial.println("Servo to pos2");
  srv.write(pos2);
  delay(5000);
  Serial.println("Test: Done.");
  Serial.println("----------------------\n");
  return;
}

void TestServo(ServoExp srv, uint8_t Butpin, uint8_t pos1, uint8_t pos2) {
  if (srv.attached() == false) {
    srv.attach();
  }
  Serial.println("Test: Servo");
  Serial.println("----------------------");
  srv.printData();
  delay(1000);
  Serial.println("Servo to pos1. Press Button to continue.");
  unsigned long ctime = millis();
  while (true) {
    if (millis() - ctime >= 1000) {
      ctime = millis();
      Serial.print(".");
    }
    if (digitalRead(Butpin) == LOW) {
      srv.write(pos1);
      delay(500);
      break;
    }
  }
  Serial.println("Servo to pos2. Press Button to continue.");
  ctime = millis();
  while (true) {
    if (millis() - ctime >= 1000) {
      ctime = millis();
      Serial.print(".");
    }
    if (digitalRead(Butpin) == LOW) {
      srv.write(pos2);
      delay(500);
      break;
    }
  }
  Serial.println("Test: Done.");
  Serial.println("----------------------\n");
  return;
}
void TestStepper(DRV8825 stp, Sensor::HallSwitch hall, bool hallInUse) {
  Serial.println("Test: Stepper");
  Serial.println("----------------------");
  delay(1000);
  if (hallInUse) {
    Serial.println("Stepper to Home.");
    while (hall.read() != true) {
      stp.move(1);
    }
  } else {
    Serial.println("No Sensor::Hall Sensor in use. Pos1 = current position.");
  }
  delay(1000);
  Serial.println("Stepper to Pos 2.");
  stp.move(STP_POS);
  delay(1000);
  Serial.println("Stepper to Pos 3.");
  stp.move(STP_POS);
  delay(1000);
  Serial.println("Test: Done.");
  Serial.println("----------------------\n");
  return;
}
} // namespace testComp

namespace testFunc {
void TestHammer(DC_Motor_Driver::MP6550 HRdc, ServoExp HSsv, Sensor::Endstops WGes, Sensor::HallSwitch hallHr) {
  if (HSsv.attached() == false) {
    HSsv.attach();
  }
  Serial.println("Testing Hammerfunction");

  Serial.println("Disengaging Hammerstop");
  HSsv.write(HS::OFF);
  delay(1000);

  Serial.println("Starting Motor.");
  HRdc.run(HR::SPEED);
  delay(1000);

  Serial.println("Waiting for weight to reach bottom.");
  while(WGes.read() != Weight::BOTTOM) {
    delay(10);
  }

  Serial.println("Weight reached bottom, motor is braking.");
  HRdc.brake();
  delay(DELAY);

  Serial.println("Engaging Hammerstop.");
  serv::hammerstop();

  Serial.println("Rewinding the Weight.");
  HRdc.run(HR::RS_SPEED);
  while(WGes.read() != Weight::BOTTOM) {
    delay(10);
  }

  Serial.println("Weight reached the top, motor is braking.");
  HRdc.brake();
  serv::hammergo();
}

void TestSchlitten(DC_Motor_Driver::MP6550 moSl, ServoExp KUsv, Sensor::Endstops esSl) {
  if (KUsv.attached() == false) {
    KUsv.attach();
  }
  Serial.println("Testing Schlitten");

  int speed = 255;

  if (esSl.read() != 0) {
    if (esSl.read() == 1) {
      Serial.println("Schlitten ist links");
      delay(DELAY);
      moSl.run(speed);
    } else if (esSl.read() == 2) {
      Serial.println("Schlitten ist rechts");
      delay(DELAY);
      moSl.run(-speed);
    }
  } else {
    Serial.println("Kein Endschalter erreicht");
    delay(DELAY);
    moSl.run(speed);
    while (esSl.read() == 0) {
      delay(10);
    }
    moSl.brake();
    delay(DELAY);
  }
}

void TestSchild(DRV8825 stp, bool hallInUse) {

  Serial.println("Testing Schild\n-----------------");

  if (hallInUse) {
    Serial.println("Stepper to Home.");
    step::pos1();
    delay(DELAY);
  }

  Serial.println("Stepper to Pos 2.");
  step::pos2();
  delay(DELAY);

  Serial.println("Stepper to Pos 3.");
  step::pos3();
  delay(2 * DELAY);
}

} // namespace testFunc

namespace measure {

void MagnetsHR(DC_Motor_Driver::MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns, uint8_t speed) {
  constexpr uint8_t nMagnets = 6;     //Number of magnets on the DC-Motor
  uint32_t aTT_Timer = 0;             //Timer for the averageTriggerTime
  bool hallTriggered = false;         //Flag to check if the Sensor::Hall-sensor is triggered
  bool magnetPassed = false;          //Flag to check if a magnet has passed the Sensor::Hall-sensor
  
  mdc.run(speed);
  while(hall.read() != true){
    delay(1);
  }
  uint32_t lastTime = millis();    //Storing the time for the last time a Sensor::Hall-sensor was triggered to calculate the time between two magnets
  for(int m = 0; m<turns; m++){
    Serial.println("----------");
    Serial.print("Turn ");
    Serial.print(m+1);
    Serial.println(":");
    Serial.println("----------");
    for(int n = 0; n<nMagnets; n++){
      magnetPassed = false;
      hallTriggered = false;
      while(magnetPassed == false){
        if(hall.read() == true && hallTriggered == false){
          Serial.print("Magnet ");
          Serial.print(n+1);
          Serial.print(" Delta_t_m: ");
          if(n == 0 && m == 0){
            Serial.print("n/a | ");
          }
          else{
            Serial.print(millis() - lastTime);
            Serial.print(" ms | ");
          }
          lastTime = millis();
          aTT_Timer = millis();
          hallTriggered = true;
        }
        else if(hall.read() != true && hallTriggered == true){
          Serial.print("Delta_t_on: ");
          Serial.print(millis() - aTT_Timer);
          Serial.println(" ms.");
          magnetPassed = true;
        }
      }
    }
  }
  mdc.brake();
}


} // namespace measure