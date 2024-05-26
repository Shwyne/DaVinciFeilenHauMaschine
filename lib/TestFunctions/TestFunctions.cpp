#include "TestFunctions.hpp"

void printEndstops(uint8_t status){
  switch(status){
    case 0:
      Serial.println("Endstops: NOT triggered. {Status = 0}");
      break;
    case 1:
      Serial.println("Endstops: 1 Triggered. {Status = 1}");
      break;
    case 2:
      Serial.println("Endstops: 2 Triggered. {Status = 2}");
      break;
    case 3:
      Serial.println("Endstops: 1+2 Triggered. {Status = 3}");
      break;
  }
  return;
}

namespace testComp{

void DC(MP6550 DC, uint8_t speed){
  
  //*Settins:
  uint16_t delayTime = 3000;  //Delay-Time between the steps
  uint16_t blockTime = 1000;  //Block-Time till a button press is accepted (ensures minimum run time)
  
  //*Starting:
  if(DEBUG>0) Serial.println("Testing DC-Motor (MP6550 Driver):");
  if(useButtonLED){
    Go.updateLED(LED::GREEN);
  }
  if(useButton){
    if(DEBUG>0) Serial.println("Press Button to start");
    Go.waitForPress();
  }
  else{
    if(DEBUG>0){
      Serial.print("Starting in ");
      Serial.print(delayTime/1000);
      Serial.println("seconds.");
    }
    delay(delayTime);
  }
  
  //*DC Forward:
  if(DEBUG>0) Serial.println("Moving forward.");
  if(useButtonLED){
    Go.updateLED(LED::CYAN);
  }
  DC.run(speed);
  if(useButton){
    delay(blockTime);
    Go.waitForPress();
  }
  else{
    delay(delayTime);
  }
  
  //*DC Braking:
  if(DEBUG>0) Serial.println("Braking.");
  if(useButtonLED){
    Go.updateLED(LED::RED);
  }
  DC.brake();
  delay(delayTime);
  
  //*DC Reverse:
  if(DEBUG>0) Serial.println("Moving reverse.");
  if(useButtonLED){
    Go.updateLED(LED::YELLOW);
  }
  DC.run(-speed);
  if(useButton){
    delay(blockTime);
    Go.waitForPress();
  }
  else{
    delay(delayTime);
  }
  
  //*DC Braking:
  if(DEBUG>0) Serial.println("Braking.");
  if(useButtonLED){
    Go.updateLED(LED::RED);
  }
  DC.brake();
  delay(delayTime);
  
  //*Test finished:
  if(DEBUG>0) Serial.println("Test finished.");
  if(useButtonLED){
    Go.updateLED(LED::WHITE);
  }
  return;
}
void Servo(ServoExp srv, uint8_t pos1, uint8_t pos2){
  
  //*Settings:
  uint32_t delayTime = 3000;  //Delay-Time between the steps
  uint32_t blockTime = 1000;  //Blocks the user input for the servo to reach its target
  
  //*Starting:
  if(DEBUG>0) Serial.println("Testing Servo:");
  if(useButtonLED){
    Go.updateLED(LED::GREEN);
  }
  if(useButton){
    if(DEBUG>0) Serial.println("Press Button to start");
    Go.waitForPress();
  }
  else{
    if(DEBUG>0){
      Serial.print("Starting in ");
      Serial.print(delayTime/1000);
      Serial.println("seconds.");
    }
    delay(delayTime);
  }
  
  //*Servo Position 1:
  if(DEBUG>0){
    Serial.print("Moving to Position 1: ");
    Serial.print(pos1);
    Serial.println("°");
  }
  if(useButtonLED){
    Go.updateLED(LED::YELLOW);
  }
  srv.runToPos(pos1);
  if(useButton){
    delay(blockTime);
    Go.waitForPress();
  }
  else{
    delay(3000);
  }

  //*Servo Position 2:
  if(DEBUG>0){
    Serial.print("Moving to Position 2: ");
    Serial.print(pos2);
    Serial.println("°");
  }
  if(useButtonLED){
    Go.updateLED(LED::CYAN);
  }
  srv.runToPos(pos2);
  if(useButton){
    delay(blockTime);
    Go.waitForPress();
  }
  else{
    delay(3000);
  }

  //*Test finished:
  if(DEBUG>0) Serial.println("Test finished.");
  if(useButtonLED){
    Go.updateLED(LED::WHITE);
  }
  return;
}
void Stepper(AccelStepper stp){
  using namespace STP;  //Namespace for the Stepper-Configurations
  //*Settings:
  uint32_t delayTime = 3000;  //Delay-Time between the steps (Not the stepper steps, but the steps of the test)
  stp.setMaxSpeed(MAX_SPEED);  //Setting the maximum speed of the Stepper
  stp.setAcceleration(ACCEL);  //Setting the acceleration of the Stepper

  //*Starting:
  if(DEBUG>0) Serial.println("Testing Stepper:");
  if(useButtonLED){
    Go.updateLED(LED::GREEN);
  }
  if(useButton){
    if(DEBUG>0) Serial.println("Press Button to start");
    Go.waitForPress();
  }
  else{
    if(DEBUG>0){
      Serial.print("Starting in ");
      Serial.print(delayTime/1000);
      Serial.println("seconds.");
    }
    delay(delayTime);
  }

  //*Moving forward:
  if(DEBUG>0) Serial.println("Stepper forwards");
  if(useButtonLED){
    Go.updateLED(LED::CYAN);
  }
  stp.moveTo(FULL_REV);
  stp.runToPosition();
  if(useButton){
    delay(100);
    Go.waitForPress();
  }
  else{
    delay(delayTime);
  }

  //*Moving backwards:
  if(DEBUG>0) Serial.println("Stepper backwards");
  if(useButtonLED){
    Go.updateLED(LED::MAGENTA);
  }
  stp.moveTo(0);
  stp.runToPosition();
  if(useButton){
    delay(100);
    Go.waitForPress();
  }
  else{
    delay(delayTime);
  }

  //*Test finished:
  if(DEBUG>0) Serial.println("Test finished.");
  if(useButtonLED){
    Go.updateLED(LED::WHITE);
  }
}
void Endstops(Sensor::Endstops es){
  uint8_t StatusBuffer = 0;
  if(DEBUG>0){
    Serial.println("Testing Endstops:");
    if(useButton) Serial.println("Press Button to end the test.");
    Serial.println("--------------------");
    printEndstops(es.read());
  } 
  while(1){
    if(StatusBuffer != es.read()){
      StatusBuffer = es.read();
      if(useButtonLED){
        switch(StatusBuffer){
          case 0:
            Go.updateLED(LED::WHITE);
            break;
          case 1:
            Go.updateLED(LED::CYAN);
            break;
          case 2:
            Go.updateLED(LED::YELLOW);
            break;
          case 3:
            Go.updateLED(LED::RED);
            break;
        }
      }
      if(DEBUG>0){
        printEndstops(StatusBuffer);
      }
    }
    if(useButton){
      if(Go.read() == true){
        break;
      }
    }
  }
}
void Hall(Sensor::HallSwitch hall){
  bool StatusBuffer = false;
  if(DEBUG>0){
    Serial.println("Testing Hall-Sensor:");
    if(useButton) Serial.println("Press Button to end the test.");
    Serial.println("--------------------");
    if(hall.read() == true){
      Serial.println("Hall-Sensor is triggered.");
      if(useButtonLED){
        Go.updateLED(LED::MAGENTA);
      }
    }
    else{
      Serial.println("Hall-Sensor is not triggered.");
      if(useButtonLED){
        Go.updateLED(LED::WHITE);
      }
    }
  }
  while(1){
    if(StatusBuffer != hall.read()){
      StatusBuffer = hall.read();
      if(useButtonLED){
        if(StatusBuffer == true){
          Go.updateLED(LED::MAGENTA);
        }
        else{
          Go.updateLED(LED::WHITE);
        }
      }
      if(DEBUG>0){
        if(StatusBuffer == true){
          Serial.println("Hall-Sensor is triggered.");
        }
        else{
          Serial.println("Hall-Sensor is not triggered.");
        }
      }
    }
    if(useButton){
      if(Go.read() == true){
        break;
      }
    }
  }
}
void Button(){
  bool StatusBuffer = false;
  if(DEBUG>0){
    Serial.println("Testing Button:");
    if(useButton) Serial.println("Press Button for 5 seconds to end the test.");
    Serial.println("--------------------");
  }
  while(1){
    if(StatusBuffer != Go.read()){
      StatusBuffer = Go.read();
      if(useButtonLED){
        if(StatusBuffer == true){
          Go.updateLED(LED::GREEN);
        }
        else{
          Go.updateLED(LED::WHITE);
        }
      }
      if(DEBUG>0){
        if(StatusBuffer == true){
          Serial.println("Button is pressed.");
        }
        else{
          Serial.println("Button is not pressed.");
        }
      }
    }
    if(useButton){
      if(Go.read() == true){
        uint32_t pressTime = millis();
        while(Go.read() == true){
          if(millis() - pressTime >= 5000){
            break;
          }
        }
        if(millis() - pressTime >= 5000){
          break;
        }
      }
    }
  }
}
void ButtonLED(){
  //*Settings:
  uint32_t delayTime = 3000;
  //*Starting:
  if(DEBUG>1){
    Serial.println("Testing RGB-LED:");
    Serial.println("--------------------");
  }
  //*Shifting through the colors:
  for(uint8_t n = 0; n < 8; n++){
    Go.updateLED(n);
    delay(delayTime);
  }
  //*Test finished:
  Go.updateLED(LED::WHITE);
  if(DEBUG>1){
    Serial.println("Test finished.");
  }
  return;
}

} // namespace testComp


namespace testFunc {

void findEndstops(){
  //*Settings:
  uint16_t bufferDelay = 100; //Delay-Time after processing a signal change
  uint16_t pressTimeExit = 5000; //Time to press the button to exit the test
  if(DEBUG>0) Serial.println("Identifying Endstops.");
  uint8_t bufferES1 = 0;
  uint8_t bufferES2 = 0;
  while(1){
    if(bufferES1 != WGes.read()){
      bufferES1 = WGes.read();
      if(DEBUG>0){
        switch(bufferES1){
          case 0:
            Serial.println("Weight: Untriggered");
            break;
          case 2:
            Serial.println("Weight: Bottom");
            break;
          case 1:
            Serial.println("Weight: Top");
            break;
          case 3:
            Serial.println("Weight: Top + Bottom");
            break;
        }
      }
      if(useButtonLED){
        switch(bufferES1){
          case 0:
            Go.updateLED(LED::WHITE);
            break;
          case 1:
            Go.updateLED(LED::CYAN);
            break;
          case 2:
            Go.updateLED(LED::YELLOW);
            break;
          case 3:
            Go.updateLED(LED::RED);
            break;
        }
      }
      delay(bufferDelay);
    }
    if(bufferES2 != SLes.read()){
      bufferES2 = SLes.read();
      if(DEBUG>0){
        switch(bufferES2){
          case 0:
            Serial.println("Slider: Untriggered");
            break;
          case 2:
            Serial.println("Slider: Right");
            break;
          case 1:
            Serial.println("Slider: Left");
            break;
          case 3:
            Serial.println("Slider: Left + Right");
            break;
        }
      }
      if(useButtonLED){
        switch(bufferES2){
          case 0:
            Go.updateLED(LED::WHITE);
            break;
          case 1:
            Go.updateLED(LED::CYAN);
            break;
          case 2:
            Go.updateLED(LED::YELLOW);
            break;
          case 3:
            Go.updateLED(LED::RED);
            break;
        }
      }
      delay(bufferDelay);
    }
    //Exit, if Button is pressed for pressTimeExit ms
    if(useButton){
      if(Go.read() == true){
        uint32_t pressTime = millis();
        while(Go.read() == true){
          if(millis() - pressTime >= pressTimeExit){
            return;
          }
        }
      }
    }
  }
}

void WeightHammer(){
  //*Settings:
  uint16_t delayTime = 3000;
  uint16_t blockTime = 1000;
  //*Starting:
  if(DEBUG>0) Serial.println("Hammer and Weight Test:");
  if(useButtonLED){
    Go.updateLED(LED::GREEN);
  }
  if(useButton){
    if(DEBUG>0) Serial.println("Press Button to start");
    Go.waitForPress();
  }
  else{
    if(DEBUG>0){
      Serial.print("Starting in ");
      Serial.print(delayTime/1000);
      Serial.println("seconds.");
    }
    delay(delayTime);
  }
  //*If the weight is not at the top, move it to the top
  if(WGes.read() != Weight::TOP){
    if(useButtonLED){
      Go.updateLED(LED::YELLOW);
    }
    if(DEBUG>0) Serial.println("Weight NOT Top");
    if(DEBUG>0) Serial.println("Moving Weight to Top");
    HWdc.run(-HW::RS_SPEED);
    while(WGes.read() != Weight::TOP){
      delay(1);
    }
    HWdc.brake();
    if(DEBUG>0) Serial.println("Weight reached Top");
    if(useButtonLED){
      Go.updateLED(LED::GREEN);
    }
    delay(1000);
  }
  //*Moving Weight down, hammer is working
  if(useButton){
    delay(blockTime);
    Go.waitForPress();
  }
  else{
    delay(delayTime);
  }
  if(DEBUG>0) Serial.println("Weight runs down, Hammerwheel is moving");
  if(useButtonLED){
    Go.updateLED(LED::CYAN);
  }
  HWdc.run(HW::SPEED);
  while(WGes.read() != Weight::BOTTOM){
    delay(1);
  }
  HWdc.brake();
  //*Weight reached bottom, hammer stops
  if(DEBUG>0) Serial.println("Weight reached Bottom");
  delay(1000);
  if(useButtonLED){
    Go.updateLED(LED::MAGENTA);
  }
  if(useButton){
    delay(blockTime);
    Go.waitForPress();
  }
  else{
    delay(delayTime);
  }

  //*Moving Weight up, hammer is blocked in position
  if(DEBUG>0) Serial.println("Moving Weight to Top");
  if(useButtonLED){
    Go.updateLED(LED::YELLOW);
  }
  HWdc.run(-HW::RS_SPEED);
  while(WGes.read() != Weight::TOP){
    delay(1);
  }
  HWdc.brake();
  if(DEBUG>0) Serial.println("Weight reached Top");
  //*Test finished
  if(DEBUG>0) Serial.println("Test finished.");
  return;

}

void Slider(){
  //*Settings:
  uint16_t delayTime = 3000;
  uint16_t blockTime = 1000;
  //*Starting:
  if(DEBUG>0) Serial.println("Slider Test:");
  if(useButtonLED){
    Go.updateLED(LED::GREEN);
  }
  if(useButton){
    if(DEBUG>0) Serial.println("Press Button to start");
    Go.waitForPress();
  }
  else{
    if(DEBUG>0){
      Serial.print("Starting in ");
      Serial.print(delayTime/1000);
      Serial.println("seconds.");
    }
    delay(delayTime);
  }
  //*If the slider is not at the left, move it to the left
  if(SLes.read() != Slider::LEFT){
    if(useButtonLED){
      Go.updateLED(LED::YELLOW);
    }
    if(DEBUG>0) Serial.println("Slider NOT Left");
    if(DEBUG>0) Serial.println("Moving Slider to Left");
    SLdc.run(-SL::RS_SPEED);
    while(SLes.read() != Slider::LEFT){
      delay(1);
    }
    SLdc.brake();
    if(DEBUG>0) Serial.println("Slider reached Left");
    if(useButtonLED){
      Go.updateLED(LED::GREEN);
    }
    delay(1000);
  }
  //*Moving Slider right
  if(useButton){
    delay(blockTime);
    Go.waitForPress();
  }
  else{
    delay(delayTime);
  }
  if(DEBUG>0) Serial.println("Slider runs right");
  if(useButtonLED){
    Go.updateLED(LED::CYAN);
  }
  SLdc.run(SL::SPEED);
  while(SLes.read() != Slider::RIGHT){
    delay(1);
  }
  SLdc.brake();
  //*Slider reached right
  if(DEBUG>0) Serial.println("Slider reached Right");
  delay(1000);
  if(DEBUG>0) Serial.println("Test finished.");
  return;
}

void Coupling(){
  //*Settings:
  uint16_t delayTime = 3000;
  uint16_t blockTime = 1000;
  uint16_t pressTimeExit = 5000;
  //*Starting:
  if(DEBUG>0) Serial.println("Coupling Test:");
  if(useButtonLED){
    Go.updateLED(LED::GREEN);
  }
  if(useButton){
    if(DEBUG>0) Serial.println("Press Button to start");
    Go.waitForPress();
  }
  else{
    if(DEBUG>0){
      Serial.print("Starting in ");
      Serial.print(delayTime/1000);
      Serial.println("seconds.");
    }
    delay(delayTime);
  }
  uint32_t stopWatch = millis();
  bool Engaged = false;
  while(1){
    if(millis() - stopWatch >= delayTime){
      if(useButton == false || Go.read() == true){
        if(Engaged){
          if(DEBUG>0) Serial.println("Disengaging Coupling");
          if(useButtonLED){
            Go.updateLED(LED::MAGENTA);
          }
          COsv.write(COUP::DIS);
        }
        else{
          if(DEBUG>0) Serial.println("Engaging Coupling");
          if(useButtonLED){
            Go.updateLED(LED::YELLOW);
          }
          COsv.write(COUP::EN);
        }
        Engaged = !Engaged;
        delay(blockTime);
        stopWatch = millis();
      }
    }
    //If Button is pressed for pressTimeExit ms, exit the test
    if(useButton){
      if(Go.read() == true){
        uint32_t pressTime = millis();
        while(Go.read() == true){
          if(millis() - pressTime >= pressTimeExit){
            if(DEBUG>0) Serial.println("Test finished.");
            return;
          }
        }
      }
    }
  }
}

} // namespace testFunc
