#include "levelManage.h"

// /////////////////////////////////Definitions///////////////////////////////////////////////////////////////////////////////////////////////
void level7(){
    Serial.print("GUARD :    ");

      if (count < 3)
      {

        analogWrite(ENL, 0);
        analogWrite(ENR, 0);
        int ultrasonicDistanceLeft = ultrasonicDistance(SENSOR_LEFT);
        int ultrasonicDistanceRight = ultrasonicDistance(SENSOR_RIGHT);

        Serial.print(ultrasonicDistanceLeft);
        Serial.print("     ");
        Serial.print(ultrasonicDistanceRight);
        Serial.print("     ");
        Serial.print(guard_left);
        // Serial.print("     ");
        // Serial.print(guard_left_prev);
        Serial.print("     ");
        Serial.print(guard_right);
        // Serial.print("     ");
        // Serial.print(guard_right_prev);
        Serial.print("     ");
        Serial.println(count);

        if (ultrasonicDistanceLeft < 20)
        {
          guard_left = 1;
        }
        else
        {
          guard_left = 0;
        }

        if (ultrasonicDistanceRight < 20)
        {
          guard_right = 1;
        }
        else
        {
          guard_right = 0;
        }

        delay(300);

        if (count == 0 and guard_left == 1 and guard_right == 1)
        {
          count = 1;
          u8g2.clearBuffer();                 // clear the internal memory
          u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
          u8g2.drawStr(0, 10, "STEP 1");      // write something to the internal memory
          u8g2.sendBuffer();                  // transfer internal memory to the display
        }
        if (count == 1 and guard_left == 1 and guard_right == 0)
        {
          count = 2;
          u8g2.clearBuffer();                 // clear the internal memory
          u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
          u8g2.drawStr(0, 10, "STEP 2");      // write something to the internal memory
          u8g2.sendBuffer();                  // transfer internal memory to the display
        }
        if (count == 2 and guard_left == 0 and guard_right == 0)
        {
          count = 3;
          u8g2.clearBuffer();                 // clear the internal memory
          u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
          u8g2.drawStr(0, 10, "STEP 3");      // write something to the internal memory
          u8g2.sendBuffer();                  // transfer internal memory to the display
        }
      }
      else
      {
        u8g2.clearBuffer();                 // clear the internal memory
        u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
        u8g2.drawStr(0, 10, "Satisfied");   // write something to the internal memory
        u8g2.sendBuffer();                  // transfer internal memory to the display
        Serial.print("Satisfied");
        // motor(LEFT_MOTOR,FORWARD);
        // motor(RIGHT_MOTOR, FORWARD);
        // analogWrite(ENL,230);
        // analogWrite(ENR, 170);
        // delay(1000);
        lineFollow();
        if(TDetected()){
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
            delay(1000);
            pull_bend = true;
        }
      }
}