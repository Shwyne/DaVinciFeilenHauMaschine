#include <Arduino.h>      //If using Arduino IDE -> comment this line out.
#include "MP6550_driver.hpp"
#include "Servo_Expanded.hpp"
#include "Sensors.hpp"

//* Port-Definitions:

//DC-Motor Hammerrad:
#define HR_IN1 12
#define HR_IN2 13
#define HR_SLP 11
#define HR_VIS A0

//DC-Motor Schlitten:
#define SL_IN1 4
#define SL_IN2 3
#define SL_SLP 2
#define SL_VIS A1

//Servo-Motoren:
#define HS_SRV 6    //Hammerstop
#define KU_SRV 7    //Kupplung

//Stepper:
#define STP_DIR 18
#define STP_STP 17
#define STP_SLP 16
#define STP_RST 15
#define STP_FLT 14

//Sensorik:
#define GE_ES_O 39  //Endschalter Gewicht oben
#define GE_ES_U 41  //Endschalter Gewicht unten
#define SL_ES_L 43  //Endschalter Schlitten links
#define SL_ES_R 45  //Endschalter Schlitten rechts

#define HR_HALL 49  //Hall-Sensor Hammerrad
#define SH_HALL 51  //Hall-Sensor Schild

#define GO_BUT 53   //Taster "Go"

//* Constants:

#define DELAY 1000  //Default delay for testing purposes
#define SERIAL_BPS 9600 //Serial Baudrate

#define HS_MIN 0    //Hammerstop min Angle
#define HS_MAX 90  //Hammerstop max Angle

//* objects:
motor moHr(HR_IN1,HR_IN2,HR_SLP);
//motor moSL(SL_IN1,SL_IN2,SL_SLP);
//stepper stp(STP_DIR,STP_STP,STP_SLP,STP_RST,STP_FLT);
ServoExpanded srvHs(HS_SRV,0,160);
//Servo srvKu(KU_SRV,MIN_ANGLE,MAX_ANGLE);
Hall hallHr(HR_HALL);
//Hall hallSh(SH_HALL);
Endstop esGe(GE_ES_O,GE_ES_U, 0);   //State 1 = Oben, State 2 = Unten
//endstop esSl(SL_ES_L,SL_ES_R);    //State 1 = Links, State 2 = Rechts

//* functions:

void HammerReset();
void HammerStop();
void HammerGo();

void SchlittenReset();
void SchlittenStop();
void SchlittenGo();

void Schild1();
void Schild2();
void Schild3();

void setup() {

  pinMode(GO_BUT, INPUT_PULLUP);
  Serial.begin(SERIAL_BPS);
  HammerReset();

}

void loop() {
  
  if(digitalRead(GO_BUT) == 0){
    HammerGo();
    delay(DELAY);
    HammerReset();
  }

}

void HammerReset(){
  srvHs.write(HS_MIN);
  if(esGe.getState() != 1){
    while(hallHr.getState() != 1){
      moHr.backwards(255);
    }
    moHr.brake(255);
    srvHs.write(HS_MAX);
    delay(500);
    while(esGe.getState() != 1){
      moHr.backwards(255);
    }
    moHr.brake(255);
  }
}

void HammerGo(){
  srvHs.write(HS_MIN);
  while(esGe.getState() != 2){
    moHr.forwards(255);
  }
  moHr.brake(255);
}
