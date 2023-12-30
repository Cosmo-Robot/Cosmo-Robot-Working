#include "levelManage.h"
// /////////////////////////////////what to do when on a checkpoint///////////////////////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////Definitions///////////////////////////////////////////////////////////////////////////////////////////////
void checkpoint(){
    if (pointer == 3){
        if (boxGrabbed == false){
            motor(LEFT_MOTOR, FORWARD);
            motor(RIGHT_MOTOR, FORWARD);
            analogWrite(ENL, 230);
            analogWrite(ENR, 170);delay(500);
            arm.write(140);
            analogWrite(ENL, 0);
            analogWrite(ENR, 0);delay(1000);
            analogWrite(ENL, 240);
            analogWrite(ENR, 160);
            motor(LEFT_MOTOR, BACKWARD);
            motor(RIGHT_MOTOR, BACKWARD);delay(6000);
            arm.write(0);
            boxGrabbed = true;
        }   
      }


      if(pointer != 3){
        Serial.println("Square detected");
        analogWrite(ENL, 0);
        analogWrite(ENR, 0);

        // if (Stage_no == 257){
        //   motor(LEFT_MOTOR, FORWARD);
        //   motor(RIGHT_MOTOR, BACKWARD);
        //   analogWrite(ENL, 255);
        //   analogWrite(ENR, 255);
        //   delay(1000);
        // }

        if (pointer == 1){
          motor(LEFT_MOTOR, FORWARD);
          motor(RIGHT_MOTOR, FORWARD);
          analogWrite(ENL, 230);
          analogWrite(ENR, 170);
          delay(900);
          motor(LEFT_MOTOR, FORWARD);
          motor(RIGHT_MOTOR, BACKWARD);
          analogWrite(ENL, 255);
          analogWrite(ENR, 255);
          delay(750);
          analogWrite(ENL,0);
          analogWrite(ENR, 0);
        }

        if (pointer == 4){
          motor(LEFT_MOTOR, FORWARD);
          motor(RIGHT_MOTOR, FORWARD);
          analogWrite(ENL, 230);
          analogWrite(ENR, 170);
          delay(900);
          motor(LEFT_MOTOR, BACKWARD);
          motor(RIGHT_MOTOR, FORWARD);
          analogWrite(ENL, 255);
          analogWrite(ENR, 255);
          delay(750);
          analogWrite(ENL,0);
          analogWrite(ENR, 0);
        }

        pointer++;
        // Stage_no = Stage_no +1 ;
        arm.write(120);
        delay(1000);
        arm.write(0);
        delay(1000);
        arm.write(120);
        motor(LEFT_MOTOR, FORWARD);
        motor(RIGHT_MOTOR, FORWARD);
        analogWrite(ENL, 230);
        analogWrite(ENR, 170);
        delay(1400);

        //     //TASK = Stage(TASK + 1);
        //     // square is correct for the final competition.`````````````````````````````````````
        //     // TASK = HALT;
      }
}