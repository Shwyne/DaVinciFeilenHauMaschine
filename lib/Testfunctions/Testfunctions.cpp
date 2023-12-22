#include "Testfunctions.hpp"

// TODO: Updating functions to library changes

namespace testComp { //*Testfunctions for components

void TestDC(MP6550 mdc) {
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
void TestStepper(DRV8825 stp, bool hallInUse) {
  Serial.println("Test: Stepper");
  Serial.println("----------------------");
  delay(1000);
  if (hallInUse) {
    Serial.println("Stepper to Home.");
    while (SGha.read() != HALL_TRIGGER) {
      SGst.move(1);
    }
  } else {
    Serial.println("No Hall Sensor in use. Pos1 = current position.")
  }
  delay(1000);
  Serial.println("Stepper to Pos 2.");
  SGst.move(STP_POS);
  delay(1000);
  Serial.println("Stepper to Pos 3.");
  SGst.move(STP_POS);
  delay(1000);
  Serial.println("Test: Done.");
  Serial.println("----------------------\n");
  return;
}
} // namespace testComp

namespace testFunc {
void TestHammer(MP6550 HRdc, ServoExp HSsv, Endstop WGes, Hall hallHr) {
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
  WGes.waitUntil(Weight::BOTTOM);

  Serial.println("Weight reached bottom, motor is braking.");
  HRdc.brake();
  delay(DELAY);

  Serial.println("Engaging Hammerstop.");
  serv::decouple();

  Serial.println("Rewinding the Weight.");
  HRdc.run(HR::RS_SPEED);
  WGes.waitUntil(Weight::TOP);

  Serial.println("Weight reached the top, motor is braking.");
  HRdc.brake();
  serv::couple();
}

void TestSchlitten(MP6550 moSl, ServoExp KUsv, Endstop esSl) {
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
    step::home();
    delay(DELAY);
  }

  Serial.println("Stepper to Pos 2.");
  step::nextPos();
  delay(DELAY);

  Serial.println("Stepper to Pos 3.");
  step::nextPos();
  delay(2 * DELAY);
}

} // namespace testFunc