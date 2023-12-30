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
      if (color_picked == 'red'){
        motor(LEFT_MOTOR, BACKWARD);
        motor(RIGHT_MOTOR, FORWARD);
        analogWrite(ENL, 255);
        analogWrite(ENR, 255);
        delay(750);
      }else if (color_picked == 'blue'){
        motor(LEFT_MOTOR, FORWARD);
        motor(RIGHT_MOTOR, BACKWARD);
        analogWrite(ENL, 255);
        analogWrite(ENR, 255);
        delay(750);
      }
      motor(LEFT_MOTOR, FORWARD);
      motor(RIGHT_MOTOR, FORWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);
      delay(1400);

    }
    
  }else{
    lineFollow();
  }
}
    