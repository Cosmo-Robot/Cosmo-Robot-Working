#include "levelManage.h"
// /////////////////////////////////color box detection///////////////////////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////Definitions///////////////////////////////////////////////////////////////////////////////////////////////
void level5(){

  if (pull_box_arm_retract == false) {
    arm.write(0);
    analogWrite(ENL, 0);
    analogWrite(ENR, 0);
    delay(100);
    pull_box_arm_retract = true;
  }

  if(CrossDetected()){
    crossCount++;
    if (crossCount==1){
      motor(LEFT_MOTOR, FORWARD);
      motor(RIGHT_MOTOR, FORWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);
      delay(500);

    }else{

    }
    
  }else{
    lineFollow();
  }
}
    