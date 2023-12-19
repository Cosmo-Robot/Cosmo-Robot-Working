#include "levelManage.h"

// /////////////////////////////////Definitions///////////////////////////////////////////////////////////////////////////////////////////////
void level2(){
    int ultrasonicDistanceLeft = ultrasonicDistance(SENSOR_LEFT);
    int ultrasonicDistanceRight = ultrasonicDistance(SENSOR_RIGHT);

      lineFollow();

      if (Switch != 0)
      {
          // wall from left
          if (ultrasonicDistanceLeft < 40 && ultrasonicDistanceRight > 40)
          {
            motor(LEFT_MOTOR, FORWARD);
            motor(RIGHT_MOTOR, FORWARD);
            analogWrite(ENL, 255);
            analogWrite(ENR, 10);
            delay(800);
            while (true)
            {
              motor(RIGHT_MOTOR, FORWARD);
              analogWrite(ENL, 230);
              analogWrite(ENR, 170);
              array_lit_amount = 0;
              readIRArray();
              for (int i = 0; i < IR_ARRAY_LENGTH; i++)
              {
                array_lit_amount += IR_array[i];
              }
              if (array_lit_amount >= 3)
              {
                analogWrite(ENL, 0);
                analogWrite(ENR, 0);
                // delay(500);
                motor(LEFT_MOTOR, FORWARD);
                analogWrite(ENL, 255);
                analogWrite(ENR, 0);
                delay(700);
                break;
                // Switch = 0;
                // analogWrite(ENL, 0);
                // analogWrite(ENR, 0);
                // delay(1000) ;
              }
            }
          }
          
        //wall from right
        else if (ultrasonicDistanceLeft > 30 && ultrasonicDistanceRight < 30)
        {
            motor(RIGHT_MOTOR, FORWARD);
            analogWrite(ENR, 255);
            analogWrite(ENL, 0);
            delay(500);
            while (true)
            {
                motor(LEFT_MOTOR, FORWARD);
                analogWrite(ENL, 230);
                analogWrite(ENR, 170);
                array_lit_amount = 0;
                readIRArray();
                for (int i = 0; i < IR_ARRAY_LENGTH; i++)
                {
                array_lit_amount += IR_array[i];
                }
                if (array_lit_amount >= 3)
                {
                analogWrite(ENL, 0);
                analogWrite(ENR, 0);
                // delay(500);
                motor(RIGHT_MOTOR, FORWARD);
                analogWrite(ENL, 0);
                analogWrite(ENR, 255);
                delay(500);
                break;
                // Switch = 0;
                // analogWrite(ENL, 0);
                // analogWrite(ENR, 0);
                // delay(1000) ;
                }
            }
        }

        // //  1st wall
        // int count = 0;

        // if (count == 0)
        // {
        //   count = count + 1;
        //   motor(RIGHT_MOTOR, FORWARD);
        //   motor(LEFT_MOTOR, FORWARD);
        //   analogWrite(ENL, 255);
        //   analogWrite(ENR, 140);
        //   delay(5000);
        //   while (true)
        //   {
        //     motor(LEFT_MOTOR, FORWARD);
        //     analogWrite(ENL, 255);
        //     analogWrite(ENR, 170);
        //     array_lit_amount = 0;
        //     readIRArray();
        //     for (int i = 0; i < IR_ARRAY_LENGTH; i++)
        //     {
        //       array_lit_amount += IR_array[i];
        //     }
        //     if (array_lit_amount >= 3)
        //     {
        //       analogWrite(ENL, 0);
        //       analogWrite(ENR, 0);
        //       delay(500);
        //       motor(LEFT_MOTOR, FORWARD);
        //       analogWrite(ENL, 255);
        //       analogWrite(ENR, 0);
        //       delay(500);
        //       analogWrite(ENL, 0);
        //       analogWrite(ENR, 0);
        //       arm.write(120);
        //       delay(1000);
        //       arm.write(0);
        //       delay(1000);
        //       arm.write(120);
        //       break;

        //       // Switch = 0;
        //       // analogWrite(ENL, 0);
        //       // analogWrite(ENR, 0);
        //       // delay(1000) ;
        //     }
        //   }
        // }
      }
}