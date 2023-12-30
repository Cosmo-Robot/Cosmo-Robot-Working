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
        pull_box_arm_retract = false;
      }   
    }else if (pointer == 4 and colorBoxGrabbed == false){
      motor(LEFT_MOTOR, FORWARD);
      motor(RIGHT_MOTOR, FORWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);delay(500);
      arm.write(140);
      analogWrite(ENL, 0);
      analogWrite(ENR, 0);delay(1000);
      colorBoxGrabbed = true;
      motor(LEFT_MOTOR, BACKWARD);
      motor(RIGHT_MOTOR, FORWARD);
      analogWrite(ENL, 255);
      analogWrite(ENR, 255);
      delay(1500);
      motor(LEFT_MOTOR, FORWARD);
      motor(RIGHT_MOTOR, FORWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);
      delay(1400);

      //array of 10 intergers to record color data
      int redColorData[10];
      int blueColorData[10];  
      uint16_t r, g, b, c, colorTemp, lux;
      while (true){
        //read color data
        tcs.getRawData(&r, &g, &b, &c);
      
        redColorData[count] = r;
        blueColorData[count] = b;
        //increment count
        count++;
        //if count is 10
        if (count == 10){
          //break out of while loop
          break;
        }
        delay(100);
      }

      //calculate average of red color data
      int redColorDataAverage = 0;
      for (int i = 0; i < 10; i++){
        redColorDataAverage += redColorData[i];
      }
      redColorDataAverage /= 10;

      //calculate average of blue color data
      int blueColorDataAverage = 0;
      for (int i = 0; i < 10; i++){
        blueColorDataAverage += blueColorData[i];
      }
      blueColorDataAverage /= 10;

      //if red color data is greater than blue color data
      if (redColorDataAverage > blueColorDataAverage){
        //set color picked to red
        color_picked = "red";
      }
      //if blue color data is greater than red color data
      else if (blueColorDataAverage > redColorDataAverage){
        //set color picked to blue
        color_picked = "blue";
      }

    }else{
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