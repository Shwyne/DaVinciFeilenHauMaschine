#include "AllFunctions.hpp"

namespace exportData{
void MagnetTimings(DC_Motor_Driver::MP6550 mdc, Sensor::HallSwitch hall, uint8_t turns, uint8_t speed){
  constexpr uint8_t nMagnets = 6;     //Number of magnets on the DC-Motor
  uint32_t aTT_Timer = 0;             //Timer for the averageTriggerTime
  bool hallTriggered = false;         //Flag to check if the Sensor::Hall-sensor is triggered
  bool magnetPassed = false;          //Flag to check if a magnet has passed the Sensor::Hall-sensor
  
  mdc.run(speed);
  while(hall.read() != true){
    delay(1);
  }
  uint32_t lastTime = millis();    //Storing the time for the last time a Sensor::Hall-sensor was triggered to calculate the time between two magnets
  Serial.println("Magnet;Delta_t_m;Delta_t_on");    //Printing the header for Excel stream
  for(int m = 1; m<=turns; m++){
    Serial.print("Turn: ;");
    Serial.println(m);
    for(int n = 1; n<=nMagnets; n++){
      magnetPassed = false;
      hallTriggered = false;
      while(magnetPassed == false){
        if(hall.read() == true && hallTriggered == false){
          Serial.print(n);
          Serial.print(";");
          if(n == 0 && m == 0){
            Serial.print("n/a");
          }
          else{
            Serial.print(millis() - lastTime);
          }
          lastTime = millis();
          aTT_Timer = millis();
          hallTriggered = true;
        }
        else if(hall.read() == false && hallTriggered == true){
          Serial.print(";");
          Serial.println(millis() - aTT_Timer);
          magnetPassed = true;
        }
      }
    }
  }
  mdc.brake();
}

void WeightCountMagnets(DC_Motor_Driver::MP6550 mdc, Sensor::HallSwitch hall, Sensor::Endstops es, uint8_t speed){
    if(HW::reversed == true){
      speed = constrain(-speed, -HW::MAXSPEED, 0);
    }
    else{
      speed = constrain(speed, 0, HW::MAXSPEED);
    }
    if(es.read() != Weight::TOP){
      //serv::hammerstop();
      mdc.run(HW::RS_SPEED);
      while(es.read() != Weight::TOP){
        delay(1);
      }
      //serv::hammergo();
    }
    mdc.brake();
    delay(1000);
    mdc.run(speed);
    uint32_t lastTime = millis();
      uint32_t timeSum = 0;
      uint8_t MagnetCount = 0;
      bool hallTriggered = false;
      while(es.read() != Weight::BOTTOM){
        if(hall.read() == true && hallTriggered == false){
          MagnetCount++;
          hallTriggered = true;
          lastTime = millis();
        }
        else if(hall.read() == false && hallTriggered == true){
          hallTriggered = false;
          timeSum += millis() - lastTime;
        }  
      }
      Serial.print(MagnetCount);
      Serial.print(";");
      if(MagnetCount <= 0){
        Serial.println("n/a");
      }
      else{
        Serial.println(timeSum/MagnetCount);
      }
      mdc.brake();
      delay(500);
      //serv::hammerstop();
      mdc.run(HW::RS_SPEED);
      while(es.read() != Weight::TOP){
        delay(1);
      }
      mdc.brake();
      //serv::hammergo();
    
}

void ServoPositions(ServoExp servo, uint8_t angle1, uint8_t angle2, uint16_t iteration, uint16_t delayTime){
  if(servo.attached() == false){
    servo.attach();
  }
  Serial.println("Iteration;Position;Angle(read)");
  for(uint16_t n = 1; n<=iteration; n++){
    servo.write(angle1);
    delay(delayTime/2);
    Serial.print(n);
    Serial.print(";POS1;");
    Serial.println(servo.read());
    delay(delayTime/2);
    servo.write(angle2);
    delay(delayTime/2);
    Serial.print(n);
    Serial.print(";POS2;");
    Serial.println(servo.read());
    delay(delayTime/2);
  }
}

void SliderTiming(DC_Motor_Driver::MP6550 mdc, Sensor::Endstops es, uint8_t speed, uint8_t iterations){
  if(es.read() != Slider::RIGHT){
    mdc.run(-speed);
    while(es.read() != Slider::RIGHT){
      delay(1);
    }
  }
  mdc.brake();
  delay(100);
  Serial.println("Iteration;Time(ms)");
  for(uint8_t n = 1; n<=iterations; n++){
    uint32_t startTime = millis();
    if(n%2 == 0){
      mdc.run(-speed);
      while(es.read() != Slider::RIGHT){
      delay(1);
      }
    }
    else if(n%2 == 1){
      mdc.run(speed);
      while(es.read() != Slider::LEFT){
      delay(1);
      }
    }
    uint32_t stopwatch = millis() - startTime;
    mdc.brake();
    Serial.print(n);
    Serial.print(";");
    Serial.println(stopwatch);
    delay(100);
  }
}

} //namespace export