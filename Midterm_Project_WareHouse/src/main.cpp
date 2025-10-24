#include <Arduino.h>
#include <P1AM.h>
#include <MotorEncoder.h>

enum MachineStates{
  Home,
  Bin1,
  Bin2,
  Bin3,
  Bin4,
  Bin5,
  Bin6,
  Bin7,
  Bin8,
  Bin9
};

MachineStates curState = Home;

int modInput=1;
int modOutput=2;
int modAnalogIn=3;

int HorPos1 = 950;
int HorPos2 = 653;
int HorPos3 = 360;
int VertPos1 = 40;
int VertPos2 = 240;
int VertPos3 = 420;
int VertBin = 355;

//int mInput, int mOutput, int pCw, int pCcw, int pE, int sw
MotorEncoder HorizontalMotor(modInput, modOutput, 3, 4, 5, 1);
MotorEncoder VerticalMotor(modInput, modOutput, 5, 6, 7, 4);

//Inputs
int LBin = 2;
int LBout = 3;
int RefProbArmF = 9;
int RefProbArmR = 10;
int InputRobotArm = 11;


//Outputs
int ConveyorF = 1;
int ConveyorR = 2;
int CantMotorF = 7;
int CantMotorR = 8;
int OutToRobotArm = 13; //Listed as 9, but incorrect


void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(1000);
 
  while (!P1.init()){
    Serial.println("waiting for connection");
  }
  Serial.println("Connected");
}

//Inputs
bool SensInside(){
  return !P1.readDiscrete(modInput, LBin);
}
bool SensOutside(){
  return !P1.readDiscrete(modInput, LBout);
}
bool ArmButtonF(){
  return !P1.readDiscrete(modInput, RefProbArmF);
}
bool ArmButtonR(){
  return !P1.readDiscrete(modInput, RefProbArmR);
}
bool InRobotArm(){
  return !P1.readDiscrete(modInput, InputRobotArm);
}


//Outputs
void BeltForward(bool s){
  P1.writeDiscrete(s, modOutput, ConveyorF);
}
void BeltReverse(bool s){
  P1.writeDiscrete(s, modOutput, ConveyorR);
}
void BinArmF(bool s){
  P1.writeDiscrete(s, modOutput, CantMotorF);
}
void BinArmR(bool s){
  P1.writeDiscrete(s, modOutput, CantMotorR);
}
void OutRobotArm(bool s){
  P1.writeDiscrete(s, modOutput, OutToRobotArm);
}

void loop() {

switch (curState){
  case Home:
  BinArmR(true);
  while(ArmButtonR()){}
  BinArmR(false);
  VerticalMotor.Home(); //Puts motor at home state
  HorizontalMotor.Home(); //Puts motor at home state
  curState = Bin1;
  break;
  
  //Bin 1
  case Bin1:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos1)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos1)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos1 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    VerticalMotor.Home();
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos1)) {
    HorizontalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos1)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin2;
  }
  break;

  //Bin 2
  case Bin2:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos2)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos1)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos1 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    VerticalMotor.Home();
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos2)) {
    HorizontalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos1)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin3;
  }
  break; 

    //Bin 3
  case Bin3:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos3)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos1)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos1 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    VerticalMotor.Home();
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos3)) {
    HorizontalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos1)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin4;
  }
  break;

  //Bin 4
  case Bin4:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos1)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos2)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos2 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos1)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos2-65)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos2)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin5;
  }
  break;

  //Bin 5
  case Bin5:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos2)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos2)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos2 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos2)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos2-65)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos2)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin6;
  }
  break; 

    //Bin 6
  case Bin6:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos3)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos2)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos2 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos3)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos2-65)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos2)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin7;
  }
  break; 

    //Bin 7
  case Bin7:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos1)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos3)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos3 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos1)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos3-65)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos3)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin8;
  }
  break;

  //Bin 8
  case Bin8:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos2)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos3)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos3 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos2)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos3-65)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos3)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Bin9;
  }
  break; 

  //Bin 9
  case Bin9:
  if(!InRobotArm()){ 
    while (!HorizontalMotor.MoveTo(HorPos3)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos3)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos3 - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!VerticalMotor.MoveTo(VertBin)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    delay(500);
    if(SensInside()){
    BeltForward(true);
    };
    while(!SensOutside()){};
    BeltForward(false);
    OutRobotArm(true); //Tells arm to drop puck
    delay(1000);
    OutRobotArm(false); //Resets robot arm so it doesnt drop right way at next bin state
    BeltReverse(true);
    while(!SensInside()){};
    delay(500);
    BeltReverse(false);
    while (!VerticalMotor.MoveTo(VertBin - 35)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    HorizontalMotor.Home();
    while (!HorizontalMotor.MoveTo(HorPos3)) {
    HorizontalMotor.UpdatePulse();
    }
    while (!VerticalMotor.MoveTo(VertPos3-65)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmF(true);
    while(ArmButtonF()){}
    BinArmF(false);
    while (!VerticalMotor.MoveTo(VertPos3)) {
    VerticalMotor.UpdatePulse();
    }
    BinArmR(true);
    while(ArmButtonR()){}
    BinArmR(false);
    curState = Home;
  }
  break;
 }
}