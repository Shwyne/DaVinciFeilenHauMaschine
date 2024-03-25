#include "AllFunctions.hpp"

// TODO: Updating functions to library changes

namespace testComp { //*Testfunctions for components

void TestMP6550(MP6550 DC, uint8_t speed){
  //Testing the Motor driver. The Motor will run forward, then brake, then run reverse, then brake.
  //Running forward and changing the direction will happen after pressing the Go-Button.
  //The LED will change color after each step (forward = cyan, reverse = yellow, brake = red)

  Serial.println("Testing Motor-Driver:");
  Serial.println("Press the button to start the Motor-Driver-Test.");
  Go.updateLED(LED::GREEN);
  Go.waitForPress();

  Serial.println("Moving forward.");
  Go.updateLED(LED::CYAN);
  DC.run(speed);

  Go.waitForPress();
  Serial.println("Braking.");
  Go.updateLED(LED::RED);
  DC.brake();
  delay(1000);

  Serial.println("Moving reverse.");
  Go.updateLED(LED::YELLOW);
  DC.run(-speed);

  Go.waitForPress();
  Serial.println("Braking.");
  Go.updateLED(LED::RED);
  DC.brake();
  delay(1000);

  Serial.println("Test finished.");
  Serial.println("--------------------");
  return;
}

void TestServo(ServoExp srv, uint8_t pos1, uint8_t pos2){
  //Testing the Servo. The Servo will move to a specific position, then wait for the button to move to the next position.
  //The LED will change color after each step (Position 1 = yellow, Position 2 = cyan, Wait for Button = green)
  
  Serial.println("Testing Servo:");
  Serial.println("Press the button to start the Servo-Test.");
  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  
  Go.updateLED(LED::YELLOW);
  Serial.print("Moving to Position 1: ");
  Serial.print(pos1);
  Serial.println("°");
  srv.runToPos(pos1);
  delay(1000);

  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  
  Go.updateLED(LED::CYAN);
  Serial.print("Moving to Position 2: ");
  Serial.print(pos2);
  Serial.println("°");
  srv.runToPos(pos2);
  delay(1000);

  Serial.println("Test finished.");
  return;
}

void TestStepper(AccelStepper stp){
  //Testing the Stepper. The Stepper will home, then move to a specific position, then move to another position.
  //The Stepper will go to the next step after pressing the Go-Button.
  //The LED will change color after each step (Home = yellow, Position 1 = cyan, Position 2 = magenta)

  Serial.println("Testing Stepper:");
  Serial.println("Press the button to start the Stepper-Test.");

  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);

  Serial.println("Homing Stepper.");
  step::home();
  delay(1000);

  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  Go.updateLED(LED::CYAN);

  Serial.println("Moving Stepper to Position 1.");
  step::move(STP::POS);
  delay(1000);

  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  Go.updateLED(LED::MAGENTA);

  Serial.println("Moving Stepper to Position 2.");
  step::move(STP::POS);
  delay(1000);

  return;
}

void TestEndstops(Sensor::Endstops es){
  //Testing the Endstops. The Endstops will be checked for their status and the LED will change color accordingly.
  //The LED will change color after each step (Untriggered = white, Endstop 1 = green, Endstop 2 = cyan, Both Endstops = red)
  //Pressing the Go-Button will end the test.

  Serial.println("Testing Endstops:");
  uint8_t status = 0;
  uint8_t lastStatus = 0;

  while(true){
    status = es.read();
    if(status != lastStatus){
      switch(status){
        case 0:
          Serial.println("Endstop is not triggered.");
          Go.updateLED(LED::WHITE);
          break;
        case 1:
          Serial.println("Endstop 1 is triggered.");
          Go.updateLED(LED::GREEN);
          break;
        case 2:
          Serial.println("Endstop 2 is triggered.");
          Go.updateLED(LED::CYAN);
          break;
        case 3:
          Serial.println("Both Endstops are triggered.");
          Go.updateLED(LED::RED);
          break;
      }
      lastStatus = status;
    }

    if(Go.read() == true){
      break;
    }
  }

  return;
}

void TestHall(Sensor::HallSwitch hall){
  //Testing the Hall-Sensor. The Hall-Sensor will be checked for its status and the LED will change color accordingly.
  //The LED will change color after each step (Untriggered = white, Triggered = magenta)
  //Pressing the Go-Button will end the test.

  Serial.println("Testing Hall-Sensor:");
  bool status = false;
  bool lastStatus = false;

  while(true){
    status = hall.read();
    if(status != lastStatus){
      if(status == true){
        Serial.println("Hall-Sensor is triggered.");
        Go.updateLED(LED::MAGENTA);
      }
      else{
        Serial.println("Hall-Sensor is not triggered.");
        Go.updateLED(LED::WHITE);
      }
      lastStatus = status;
    }

    if(Go.read() == true){
      break;
    }
  }

  return;
}

void TestButton(){
  //Testing the Button. The Button will be checked for its status and the LED will change color accordingly.
  //The LED will change color after each step (Button not pressed = white, Button pressed = random color)
  //Holding the Go-Button for 5 seconds will end the test.
  Serial.println("Testing Button:");
  randomSeed(analogRead(A5));
  bool status = false;
  bool lastStatus = false;
  unsigned long buttonPressTime = 0;
  while(true){
    status = Go.read();
    if(status != lastStatus){
      if(status == true){
        Serial.println("Button is pressed.");
        Go.updateLED(random(1, 8));
        //Start the timer if the button is pressed
        buttonPressTime = millis();
      }
      else{
        Serial.println("Button is not pressed.");
        Go.updateLED(LED::WHITE);
      }
      lastStatus = status;
    }
    //End the test if the button is pressed for 5 seconds
    if(status == true && millis() - buttonPressTime >= 5000){
      break;
    }
  }
  return;
}

} // namespace testComp


//TODO: Continue here

namespace testFunc {

void IdentifyES(){
  Serial.println("Identifying Endstops.");

  uint8_t es_1 = 0;
  uint8_t es_2 = 0;
  while(1){
  if(es_1 != WGes.read()){
    es_1 = WGes.read();
    switch(es_1){
      case 0:
        Serial.println("Weight: Untriggered");
        break;
      case 2:
        Serial.println("Weight: Bottom");
        break;
      case 1:
        Serial.println("Weight: Top");
        break;
    }
    delay(100);
  }
  if(es_2 != SLes.read()){
    es_2 = SLes.read();
    switch(es_2){
      case 0:
        Serial.println("Slider: Untriggered");
        break;
      case 1:
        Serial.println("Slider: Left");
        break;
      case 2:
        Serial.println("Slider: Right");
        break;
    }
    delay(100);
  }
  }
}

void WeightAndHammer(){

  if(WGes.read() != Weight::TOP){
    serv::hammerstop();
    HWdc.run(-HW::RS_SPEED);
    while(WGes.read() != Weight::TOP){
      delay(1);
    }
    HWdc.brake();
    serv::hammergo();
    delay(1000);
  }
  HWdc.run(HW::SPEED);
  while(WGes.read() != Weight::BOTTOM){
    delay(1);
  }
  HWdc.brake();
  delay(1000);
  serv::hammerstop();
  HWdc.run(-HW::RS_SPEED);
  while(WGes.read() != Weight::TOP){
    delay(1);
  }
  HWdc.brake();
  serv::hammergo();
  return;

}

void Slider(){
  Go.updateLED(LED::GREEN);
  Serial.println("Press Button to Start Slider-Test");
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);
  if(SLes.read() != Slider::RIGHT){
    SLdc.run(-SL::RS_SPEED);
    while(SLes.read() != Slider::RIGHT){
      if(Go.read() == true){
        SLdc.brake();
        return;
      }
      delay(1);
    }
    Go.updateLED(LED::GREEN);
    SLdc.brake();
    delay(1000);
    Go.updateLED(LED::YELLOW);
  }
  SLdc.run(SL::RS_SPEED);
  while(SLes.read() != Slider::LEFT){
    if(Go.read() == true){
        SLdc.brake();
        return;
      }
    delay(1);
  }
  Go.updateLED(LED::GREEN);
  SLdc.brake();
  delay(1000);
  Go.updateLED(LED::YELLOW);
  SLdc.run(-SL::RS_SPEED);
  while(SLes.read() != Slider::RIGHT){
    if(Go.read() == true){
        SLdc.brake();
        return;
      }
    delay(1);
  }
  Go.updateLED(LED::GREEN);
  SLdc.brake();
  return;
}

void Sign(uint16_t delayTime){
 step::home();
  delay(delayTime);
 step::move(STP::POS);
  delay(delayTime);
 step::move(STP::POS);
  delay(2*delayTime);
 step::home();
  delay(delayTime);
 step::move(-STP::POS);
  delay(delayTime);
 step::move(-STP::POS);
  delay(2*delayTime);
  return;
}


void Machine(){
  constexpr uint16_t delayTime = 500;
  //Testing the whole machine for functionality after repairs or replacements
  Serial.println("Testing Machine");
  Serial.println("Press Button to start");
  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  //1. Test Endstops Weight (Press Button then wait for Endstop Top, then Endstop Bottom)
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  Serial.println("Endstop Weight Top");
  while(WGes.read() != Weight::TOP){
    delay(1);
  }
  Go.updateLED(LED::GREEN);
  delay(1000);
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  Serial.println("Endstop Weight Bottom");
  while(WGes.read() != Weight::BOTTOM){
    delay(1);
  }
  Go.updateLED(LED::GREEN);
  delay(1000);
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  //2. Test Endstops Slider (Press Button then wait for Endstop Left, then Endstop Right, Press Button again to skip this process)
  Slider();
  delay(delayTime);
  Go.updateLED(LED::GREEN);
  //3. Test Hall-Sensor Hammerwheel (Press Button, then spin the wheel manually. IF the light changes to purple, the hall-sensor is sensing a magnet)
  Serial.println("Hall-Sensor Hammerwheel");
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  while(Go.read() != true){
    if(HWha.read() == true){
      Go.updateLED(LED::MAGENTA);
    }
    else{
      Go.updateLED(LED::YELLOW);
    }
    delay(1);
  }
  Go.updateLED(LED::GREEN);
  //4. Test Hall-Sensor Sign (Press Button, then move the sign manually. IF the light changes to purple, the hall-sensor is sensing a magnet)
  Serial.println("Hall-Sensor Sign");
  digitalWrite(pin::STP_SLP, LOW);
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  while(Go.read() != true){
    if(SGha.read() == true){
      Go.updateLED(LED::MAGENTA);
    }
    else{
      Go.updateLED(LED::YELLOW);
    }
    delay(1);
  }
  Go.updateLED(LED::GREEN);
  //5. Test Servo Hammerstop (Press Button, then the hammerstop moves to the off Position, press again to move in the on Position)
  Serial.println("Servo Hammerstop");
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  HSsv.runToPos(HS::OFF);
  delay(1000);
  Go.updateLED(LED::GREEN);
  while(Go.read() != true){
    delay(1);
  }
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  HSsv.runToPos(HS::ON);
  delay(1000);
  Go.updateLED(LED::GREEN);
  //6. Test Servo Coupling (Press Button, then the sign moves to the off Position, press again to move in the on Position)
  Serial.println("Servo Coupling");
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  COsv.runToPos(SERVO::OFF);
  delay(1000);
  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  Go.updateLED(LED::YELLOW);
  delay(delayTime);
  COsv.runToPos(SERVO::ON);
  delay(1000);
  Go.updateLED(LED::GREEN);





}

} // namespace testFunc

namespace measure {

  void SliderTiming(){
    if(SLes.read() != Slider::RIGHT){
    SLdc.run(-255);
    while(SLes.read() != Slider::RIGHT){
      delay(1);
    }
    SLdc.brake();
    delay(1000);
  }

  Go.updateLED(LED::GREEN);
  while(Go.read() != true){
    delay(1);
  }
  Go.updateLED(LED::CYAN);
  ctime = millis();
  SLdc.run(SL::SPEED);
  while(SLes.read() != Slider::LEFT){
    delay(1);
  }
  long SLIDER = millis()-ctime;
  SLdc.brake();
  Serial.print("Time: ");
  Serial.print(SLIDER);
  Serial.print(" ms |");
  Serial.print(SLIDER/1000.0);
  Serial.print(" s | ");
  }

void MagnetsHR(MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns, uint8_t speed) {
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
          if(n == 0 && m == 0){
            Serial.println("n/a");
          }
          else{
            Serial.print(millis() - aTT_Timer);
            Serial.println(" ms.");
          }
          magnetPassed = true;
        }
      }
    }
  }
  mdc.brake();
}

/*void TestWeight(MP6550 HWdc, Sensor::Endstops WGes, Sensor::HallSwitch HWhall, bool infinite){
  bool top = 0;
  bool bottom = 0;
  int n = 0;
  bool Hall_Triggered = false;
  if(WGes.read() == Weight::TOP){
    Serial.println("Weight is TOP");
    HWdc.run(HW::SPEED);
  }
  else{
    Serial.println("Weight is NOT top");
    HWdc.run(-HW::RS_SPEED);
  }
  while(infinite == true){
    if(HWhall.read() && (Hall_Triggered == false)){
      //Serial.println("I've found a Magnet! :)");
      Hall_Triggered = true;
      n++;
    }
    else if (!HWhall.read() && Hall_Triggered == true) Hall_Triggered = false;
    if(WGes.read() == Weight::TOP && top == 0){
      HWdc.brake();
      delay(500);
      top = 1;
      bottom = 0;
      Serial.println("Top triggered!");
      Serial.print("n: ");
      Serial.println(n);
      HWdc.run(HW::SPEED);
      n = 0;
    }
    else if(WGes.read() == Weight::BOTTOM && bottom == 0){
      HWdc.brake();
      delay(500);
      top = 0;
      bottom = 1;
      Serial.println("Bottom triggered!");
      Serial.print("n: ");
      Serial.println(n);
      HWdc.run(-HW::RS_SPEED);
      n = 0;
    }
  }
}*/
} // namespace measure
