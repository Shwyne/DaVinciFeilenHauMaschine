#include "AllFunctions.hpp"

// TODO: Updating functions to library changes

namespace testComp { //*Testfunctions for components

void TestMP6550(MP6550 DC, uint8_t ButtonPin){
  bool ButtonPresent = false;
  if(ButtonPin != 255){
    Serial.println("Press and hold the button to run the motor.");
    Serial.println(" Releasing and pressing again will change the direction.");
    ButtonPresent = true;
    pinMode(ButtonPin, INPUT_PULLUP);
  }
  bool reverse = false;
  if(ButtonPresent){
    while(true){
      if(digitalRead(ButtonPin) == LOW){
        if(reverse){
          Serial.println("Forward");
          DC.run(255);
          reverse = false;
        }
        else{
          Serial.println("Reverse");
          DC.run(-255);
          reverse = true;
        }
        while(digitalRead(ButtonPin) == LOW){
          delay(1);
        }
        DC.brake();
      }
    }
  }
  else{
    Serial.println("Running Motor forward.");
    DC.run(255);
    delay(5000);
    Serial.println("Braking Motor.");
    DC.brake();
    delay(1000);
    Serial.println("Running Motor reverse.");
    DC.run(-255);
    delay(5000);
    Serial.println("Braking Motor.");
    DC.brake();
  }
  return;
}

void TestServo(ServoExp srv, uint8_t ButtonPin, uint8_t pos1, uint8_t pos2){
  bool ButtonPresent = false;
  if(ButtonPin != 255){
    Serial.println("Press the button to move the servo to next position.");
    ButtonPresent = true;
    pinMode(ButtonPin, INPUT_PULLUP);
  }
  bool reverse = false;
  if(ButtonPresent){
    while(true){
      if(digitalRead(ButtonPin) == LOW){
        if(reverse){
          Serial.println("Position 1");
          srv.write(pos1);
          reverse = false;
        }
        else{
          Serial.println("Position 2");
          srv.write(pos2);
          reverse = true;
        }
        while(digitalRead(ButtonPin) == LOW){
          delay(1);
        }
      }
    }
  }
  else{
    Serial.println("Moving to Position 1.");
    srv.write(pos1);
    delay(500);
    Serial.println("Moving to Position 2.");
    srv.write(pos2);
    delay(500);
    Serial.println("Moving to Position 1.");
    srv.write(pos1);
    delay(500);
    Serial.println("Moving to Position 2.");
    srv.write(pos2);
    delay(500);
  }
  return;
}

void TestStepper(AccelStepper stp, uint8_t ButtonPin){
  /*bool ButtonPresent = false;
  if(ButtonPin != 255){
    Serial.println("Press the button to move the stepper 200 Steps.");
    Serial.println("The Direction will change after each press.");
    ButtonPresent = true;
    pinMode(ButtonPin, INPUT_PULLUP);
  }
  bool reverse = false;
  if(ButtonPresent){
    while(true){
      if(digitalRead(ButtonPin) == LOW){
        if(reverse){
          Serial.println("REV: 200 Steps");
          stp.run(-200);
          reverse = false;
        }
        else{
          Serial.println("FWD: 200 Steps");
          stp.run(200);
          reverse = true;
        }
        while(digitalRead(ButtonPin) == LOW){
          delay(1);
        }
      }
    }
  }
  else{
    Serial.println("Moving 200 Steps forward.");
    stp.run(200);
    delay(500);
    Serial.println("Moving 200 Steps reverse.");
    stp.run(-200);
    delay(500);
    Serial.println("Moving 200 Steps forward.");
    stp.run(200);
    delay(500);
    Serial.println("Moving 200 Steps reverse.");
    stp.run(-200);
  }*/
  return;
}

void TestEndstops(Sensor::Endstops es, Sensor::Button button, bool UseLED){

  Serial.println("Testing Endstops:");
  while(true){
    if(es.read() == 0){
      Serial.println("Endstop is not triggered.");
      if(UseLED){
        button.updateLED(LED::RED);
      }
    }
    else if(es.read() == 1){
      Serial.println("Endstop 1 is triggered.");
      if(UseLED){
        button.updateLED(LED::GREEN);
      }
    }
    else if(es.read() == 2){
      Serial.println("Endstop 2 is triggered.");
      if(UseLED){
        button.updateLED(LED::BLUE);
      }
    }
    else if(es.read() == 3){
      Serial.println("Both Endstops are triggered.");
      if(UseLED){
        button.updateLED(LED::YELLOW);
      }
    }
  }
  return;
}

void TestHall(Sensor::HallSwitch hall, Sensor::Button button, bool UseLED){
  Serial.println("Testing Hall-Sensor:");
  while(true){
    if(hall.read() == false){
      Serial.println("Hall-Sensor is not triggered.");
      if(UseLED){
        button.updateLED(LED::RED);
      }
    }
    else if(hall.read() == true){
      Serial.println("Hall-Sensor is triggered.");
      if(UseLED){
        button.updateLED(LED::GREEN);
      }
    }
  }
  return;
}

void TestButton(Sensor::Button button, bool UseLED){
  Serial.println("Testing Button:");
  randomSeed(analogRead(A5));
  while(true){
    if(button.read() == false){
      Serial.println("Button is not pressed.");
      if(UseLED){
        button.updateLED(LED::OFF);
      }
    }
    else if(button.read() == true){
      Serial.println("Button is pressed.");
      if(UseLED){
        button.updateLED(random(1,8));
      }
    }
  }
  return;
}
} // namespace testComp

namespace testFunc {

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

void SliderTiming(){
  Go.updateLED(LED::GREEN);
  Go.waitForPress();
  if(SLes.read() != Slider::RIGHT){
    Go.updateLED(LED::YELLOW);
    SLdc.run(-255);
    while(SLes.read() != Slider::RIGHT){
      delay(1);
    }
    SLdc.brake();
    delay(1000);
  }
  Go.updateLED(LED::CYAN);
  Serial.println(SL::SPEED);
  SLdc.run(SL::SPEED);
  while(SLes.read() != Slider::LEFT){
    delay(1);
  SLdc.brake();
} 
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
