#include <Arduino.h>
#include <P1AM.h>
#include <MotorEncoder.h>

enum MachineStates{
  Home,
  BinRead,
  ArmMove,
  Placement,
  WaitWareHouse
};

MachineStates curState = Home;

int modInput=1;
int modOutput=2;
int modAnalogIn=3;

int HorPosW = 165;
int VertPosW = 400;
int TurnPosW = 220;
int HorPosR = 190;
int VertPosR = 400;
int TurnPosR = 183;
int HorPosB = 265;
int VertPosB = 400;
int TurnPosB = 148;

// Outputs
int compressor = 7;
int valve = 8;
int WarehouseSignal = 9;

// Inputs
int LBw = 11;
int LBr = 12;
int LBb= 13;
int WarehouseReady = 14;

//BinRead readings character
char BinPin = ' ';

//int mInput, int mOutput, int pCw, int pCcw, int pE, int sw
MotorEncoder HorizontalMotor(modInput, modOutput, 4, 3, 7, 2);
MotorEncoder VerticalMotor(modInput, modOutput, 2, 1, 5, 1);
MotorEncoder BaseMotor(modInput, modOutput, 6, 5, 9, 3);
 
void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(1000);
 
  while (!P1.init()){
    Serial.println("waiting for connection");
  }
  Serial.println("Connected");
}
 
bool WarehouseDone(){
  return !P1.readDiscrete(modInput, WarehouseReady);
}

void ToggleCompressor(bool s){
  P1.writeDiscrete(s, modOutput, compressor);
}
void ToggleValve(bool s){
  P1.writeDiscrete(s, modOutput, valve);
}
void CallWarehouse(bool s){
  P1.writeDiscrete(s, modOutput, WarehouseSignal);
}
 
 
void loop() {

bool binW = P1.readDiscrete(modInput, LBw);
bool binR = P1.readDiscrete(modInput, LBr);
bool binB = P1.readDiscrete(modInput, LBb);

 switch (curState){
  case Home:
  VerticalMotor.Home(); //Puts motor at home state
  HorizontalMotor.Home(); //Puts motor at home state
  BaseMotor.Home(); //Puts motor at home state
  curState = BinRead;
  break;
  
  case BinRead:

   //Used to check which bin(s) has a puck

   if(binW){
    BinPin = 'w';
    curState = ArmMove;
    }
    else if(binR){
    BinPin = 'r';
    curState = ArmMove;
    }
    else if (binB){
    BinPin = 'b';
    curState = ArmMove;
    }
    else{
    BinPin = 'e'; //Bins are empty loops back
    curState = BinRead;
    }
    Serial.println(BinPin);
    
    
   break;

   case ArmMove:
   if(BinPin == 'w'){
    while (!BaseMotor.MoveTo(TurnPosW)) {
    BaseMotor.UpdatePulse();
    }
    while (!HorizontalMotor.MoveTo(HorPosW)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPosW)) {
    VerticalMotor.UpdatePulse();
    }
   }

   if(BinPin == 'r'){
    while (!BaseMotor.MoveTo(TurnPosR)) {
    BaseMotor.UpdatePulse();
    }
    while (!HorizontalMotor.MoveTo(HorPosR)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPosR)) {
    VerticalMotor.UpdatePulse();
    }
   }

   if(BinPin == 'b'){
    while (!BaseMotor.MoveTo(TurnPosB)) {
    BaseMotor.UpdatePulse();
    }
    while (!HorizontalMotor.MoveTo(HorPosB)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPosB)) {
    VerticalMotor.UpdatePulse();
    }
   }
   ToggleCompressor(true);
   delay(500);
   ToggleValve(true);
   curState = Placement;
   break;

   case Placement:
   VerticalMotor.Home();
   HorizontalMotor.Home(); 
   while (!BaseMotor.MoveTo(670)) {
    BaseMotor.UpdatePulse();
    }
   while (!HorizontalMotor.MoveTo(80)) {
    HorizontalMotor.UpdatePulse();
    }
   CallWarehouse(true);
   curState = WaitWareHouse;
   break;

   case WaitWareHouse:
   if(!WarehouseDone()){
   while (!VerticalMotor.MoveTo(70)) {
   VerticalMotor.UpdatePulse();
    }
   
   ToggleValve(false);
   ToggleCompressor(false);
   VerticalMotor.Home();
   CallWarehouse(false);
   curState = BinRead;
   }
   break;
 }
  
}