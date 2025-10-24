#include <Arduino.h>
#include <P1AM.h>

enum MachineStates{
  OvenFeeder,
  SuckZone,
  TurnTable,
  Conveyor,
};

MachineStates curState = OvenFeeder;

int modInput=1;
int modOutput=2;
int modAnalogIn=3;

//Inputs:
int OFin = 6;
int OFout = 7;
int SwitchSuckOven = 8;
int SwitchSuckTurnTable = 5;
int LBoven = 9;
int TurnSaw = 4;
int TurnSuck = 1;
int TurnConvey = 2;
int LBConvey = 3;


//Outputs:
int OFmotorR = 5;
int OFmotorE = 6;
int SuckerOven = 7;
int SuckerTurnTable = 8;
int Lamp = 9;
int compressor = 10;
int sucksuck = 11;
int LowerSuck =12;
int valveDoor = 13;
int valveFeed = 14;
int TurnTabC = 1;
int TurnTabCCW = 2;
int SawMotor = 4;
int Convey = 3;


void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(1000);
 
  while (!P1.init()){
    Serial.println("waiting for connection");
  }
  Serial.println("Connected");
}

//Switches
bool OFswitchR(){
  return !P1.readDiscrete(modInput, OFin);
}
bool OFswitchE(){
  return !P1.readDiscrete(modInput, OFout);
}
bool StartLB(){
  return !P1.readDiscrete(modInput, LBoven);
}
bool SuckSWO(){
  return !P1.readDiscrete(modInput, SwitchSuckOven);
}
bool SuckSWTT(){
  return !P1.readDiscrete(modInput, SwitchSuckTurnTable);
}
bool TSawPOS(){
  return !P1.readDiscrete(modInput, TurnSaw);
}
bool TSuckPOS(){
  return !P1.readDiscrete(modInput, TurnSuck);
}
bool TConveyPOS(){
  return !P1.readDiscrete(modInput, TurnConvey);
}
bool ConveyTrigger(){
  return !P1.readDiscrete(modInput, LBConvey);
}


//Output Commands
void ToggleCompressor(bool s){
  P1.writeDiscrete(s, modOutput, compressor);
}
void ToggleValveDoor(bool s){
  P1.writeDiscrete(s, modOutput, valveDoor);
}
void OFR(bool s){
  P1.writeDiscrete(s, modOutput, OFmotorR);
}
void OFE(bool s){
  P1.writeDiscrete(s, modOutput, OFmotorE);
}
void Light(bool s){
  P1.writeDiscrete(s, modOutput, Lamp);
}
void SuckO(bool s){
  P1.writeDiscrete(s, modOutput, SuckerOven);
}
void SuckTT(bool s){
  P1.writeDiscrete(s, modOutput, SuckerTurnTable);
}
void Sucker(bool s){
  P1.writeDiscrete(s, modOutput, sucksuck);
}
void SuckerDown(bool s){
  P1.writeDiscrete(s, modOutput, LowerSuck);
}
void TC(bool s){
  P1.writeDiscrete(s, modOutput, TurnTabC);
}
void TCCW(bool s){
  P1.writeDiscrete(s, modOutput, TurnTabCCW);
}
void Saw(bool s){
  P1.writeDiscrete(s, modOutput, SawMotor);
}
void TableValve(bool s){
  P1.writeDiscrete(s, modOutput, valveFeed);
}
void Belt(bool s){
  P1.writeDiscrete(s, modOutput, Convey);
}


void loop() {

switch (curState){
case OvenFeeder:
if (StartLB()){
ToggleCompressor(true);
ToggleValveDoor(true);

delay(750);
OFE(true);
while(OFswitchE()){}
OFE(false);

OFR(true);
while(OFswitchR()){}
OFR(false);
ToggleValveDoor(false);

Light(true);
delay(3000);
Light(false);

ToggleValveDoor(true);
OFE(true);
while(OFswitchE()){}
OFE(false);
ToggleValveDoor(false);
curState = SuckZone;
}
break;

case SuckZone:

SuckO(true);
while(SuckSWO()){}
SuckO(false);

SuckerDown(true);
delay(500);
Sucker(true);
delay(500);
SuckerDown(false);
delay(500);

SuckTT(true);
while(SuckSWTT()){}
SuckTT(false);

curState = TurnTable;
break;

case TurnTable:

TCCW(true);
while(TSuckPOS()){}
TCCW(false);

SuckerDown(true);
delay(500);
Sucker(false);
delay(500);
SuckerDown(false);
delay(500);

TC(true);
while(TSawPOS()){}
TC(false);

Saw(true);
delay(5000);
Saw(false);

TC(true);
while(TConveyPOS()){}
TC(false);

TableValve(true);
delay(500);
TableValve(false);
curState = Conveyor;
break;

case Conveyor:

Belt(true);
if (ConveyTrigger()){
  delay(1000);
  Belt(false);
  ToggleCompressor(false);
  TCCW(true);
  while(TSuckPOS()){}
  TCCW(false);
  curState = OvenFeeder;
}

break;

}
}