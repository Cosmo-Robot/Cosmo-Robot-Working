#include "levelManage.h"

//////////////////////////////Managing Levels///////////////////////////////////////////
void levelManage(){
  if (squareDetected())
    {
      if (pointer == 3){
        motor(LEFT_MOTOR, FORWARD);
        motor(RIGHT_MOTOR, FORWARD);
        analogWrite(ENL, 230);
        analogWrite(ENR, 170);
        delay(500);
        arm.write(140);
        analogWrite(ENL, 0);
        analogWrite(ENR, 0);
        delay(1000);
        analogWrite(ENL, 240);
        analogWrite(ENR, 160);
        motor(LEFT_MOTOR, BACKWARD);
        motor(RIGHT_MOTOR, BACKWARD);
        delay(6000);
        analogWrite(ENL, 0);
        analogWrite(ENR, 0);
        
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

    ///////////////////////// 1] LINE FOLLOWING///////////////////////////////////////////
    if (pointer == 0)
    {
      lineFollow();
    }

    ///////////////////////// 2] WALL FOLLOWING///////////////////////////////////////////
    else if (pointer == 1)
    {
      // int ultrasonicDistanceLeft = ultrasonicDistance(SENSOR_LEFT);
      // int ultrasonicDistanceRight = ultrasonicDistance(SENSOR_RIGHT);

      lineFollow();

      if (Switch == 0)
      {
        //   // 2nd wall
        //   if (ultrasonicDistanceLeft < 40 && ultrasonicDistanceRight > 40)
        //   {
        //     motor(LEFT_MOTOR, FORWARD);
        //     motor(RIGHT_MOTOR, FORWARD);
        //     analogWrite(ENL, 255);
        //     analogWrite(ENR, 10);
        //     delay(800);
        //     while (true)
        //     {
        //       motor(RIGHT_MOTOR, FORWARD);
        //       analogWrite(ENL, 230);
        //       analogWrite(ENR, 170);
        //       array_lit_amount = 0;
        //       readIRArray();
        //       for (int i = 0; i < IR_ARRAY_LENGTH; i++)
        //       {
        //         array_lit_amount += IR_array[i];
        //       }
        //       if (array_lit_amount >= 3)
        //       {
        //         analogWrite(ENL, 0);
        //         analogWrite(ENR, 0);
        //         // delay(500);
        //         motor(LEFT_MOTOR, FORWARD);
        //         analogWrite(ENL, 255);
        //         analogWrite(ENR, 0);
        //         delay(700);
        //         break;
        //         // Switch = 0;
        //         // analogWrite(ENL, 0);
        //         // analogWrite(ENR, 0);
        //         // delay(1000) ;
        //       }
        //     }
        //   }
        //   // 1st wall
        //   else if (ultrasonicDistanceLeft > 30 && ultrasonicDistanceRight < 30)
        //   {
        //     motor(RIGHT_MOTOR, FORWARD);
        //     analogWrite(ENR, 255);
        //     analogWrite(ENL, 0);
        //     delay(500);
        //     while (true)
        //     {
        //       motor(LEFT_MOTOR, FORWARD);
        //       analogWrite(ENL, 230);
        //       analogWrite(ENR, 170);
        //       array_lit_amount = 0;
        //       readIRArray();
        //       for (int i = 0; i < IR_ARRAY_LENGTH; i++)
        //       {
        //         array_lit_amount += IR_array[i];
        //       }
        //       if (array_lit_amount >= 3)
        //       {
        //         analogWrite(ENL, 0);
        //         analogWrite(ENR, 0);
        //         // delay(500);
        //         motor(RIGHT_MOTOR, FORWARD);
        //         analogWrite(ENL, 0);
        //         analogWrite(ENR, 255);
        //         delay(500);
        //         break;
        //         // Switch = 0;
        //         // analogWrite(ENL, 0);
        //         // analogWrite(ENR, 0);
        //         // delay(1000) ;
        //       }
        //     }
        //   }

        //  1st wall
        int count = 0;

        if (count == 0)
        {
          count = count + 1;
          motor(RIGHT_MOTOR, FORWARD);
          motor(LEFT_MOTOR, FORWARD);
          analogWrite(ENL, 255);
          analogWrite(ENR, 140);
          delay(5000);
          while (true)
          {
            motor(LEFT_MOTOR, FORWARD);
            analogWrite(ENL, 255);
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
              delay(500);
              motor(LEFT_MOTOR, FORWARD);
              analogWrite(ENL, 255);
              analogWrite(ENR, 0);
              delay(500);
              analogWrite(ENL, 0);
              analogWrite(ENR, 0);
              arm.write(120);
              delay(1000);
              arm.write(0);
              delay(1000);
              arm.write(120);
              break;

              // Switch = 0;
              // analogWrite(ENL, 0);
              // analogWrite(ENR, 0);
              // delay(1000) ;
            }
          }
        }
      }
    }

    ///////////////////////// 3] RAMP ///////////////////////////////////////////
    else if (pointer == 2)
    {
      lineFollow();
    }
    ///////////////////////// 4] PULL the Box///////////////////////////////////////////
    else if (pointer == 3)
    {
      if(pull_box_arm_retract == false){
        arm.write(0);
        analogWrite(ENL, 0);
        analogWrite(ENR, 0);
        delay(100);
        pull_box_arm_retract = true;
      }
      lineFollow();
      if (pull_bend == false){
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
    ///////////////////////// 5] Color FOLLOWING///////////////////////////////////////////
    else if (pointer == 4)
    {

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
      }else{
            lineFollow();
      }
    }
    ///////////////////////// 6] Sound Game ///////////////////////////////////////////
    else if (pointer == 5) {

      Drive_constant = 50;
      Serial.println(analogRead(13));
      if (analogRead(A13)<sound_threshold){
        delay(50);
        if (analogRead(A13)<sound_threshold){
          delay(50);
          if (analogRead(A13)<sound_threshold){
            int now_time = millis();
            while(millis()-now_time <= 500){
              lineFollow();
            }
            // analogWrite(ENL,0);
            // analogWrite(ENR,0);
            // delay(1000);
            Serial.println("triggered the sound sensor");
          }
        }
      }
      analogWrite(ENL,0);
      analogWrite(ENR,0);
      while (true){
        delay(300);
        if (analogRead(A13) < sound_threshold){break;}
      }
      {
        /* code */
      }
      
    //   if (analogRead(SOUND_SENSOR) >= SOUND_THRESHOLD) {
    //     sound_now_time = millis();
    //   }

    //   if (millis() - sound_now_time < SOUND_WAIT_TIME) {
    //     // digitalWrite(SOUND_LED, HIGH);
    //     Switch = 0;
    //   }
    //   else {
    //     // digitalWrite(SOUND_LED, LOW);
    //     Switch = 1;
    //     lineFollow();
    //   }
    // // {
    //   // Serial.println(analogRead(A13));
    //   // // Serial.println("SOUND");
    //   // if (analogRead(A13) >= 800)
    //   // {
    //   //   lineFollow();
    //   // }
    //   // else
    //   // {
    //   //   analogWrite(ENL, 0);
    //   //   analogWrite(ENR, 0);
    //   //   Serial.println("Sound Detected");
    //   //   delay(300);
    //   // }
    // // }
    }
    ///////////////////////// 7] GUARD AVOIDING///////////////////////////////////////////
    else if (pointer == 6)
    {

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
    else
    {
      // For safery, the robot will stop at any invalid TASK.
      Switch = 0;
    }
  }

  /////////////////////Mode Selection////////////////////////////////////////
  void modeSelect(){

  delay(1000); // Wait for the sensors to be ready.
  int POT_value = 0;
  while (digitalRead(calibration) == 0)
  {
    // Serial.println(digitalRead(12));
    if (digitalRead(12) == 1)
    {
      delay(600);
      POT_value++;
      while (true)
      {
        if (digitalRead(12) == 0)
        {
          break;
        }
      }
    }
    if (POT_value == 8)
    {
      POT_value = 0;
    }
    // Serial.println(POT_value);
    if (POT_value == 0)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "LINE");        // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else if (POT_value == 1)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "WALL");        // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else if (POT_value == 2)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "RAMP");        // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else if (POT_value == 3)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "PULL");        // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else if (POT_value == 4)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "COLOR");       // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else if (POT_value == 5)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "SOUND");       // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else if (POT_value == 6)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "GUARD");       // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else if (POT_value == 7)
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "HALT");        // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
    else
    {
      u8g2.clearBuffer();                 // clear the internal memory
      u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
      u8g2.drawStr(0, 10, "NOT DEF");     // write something to the internal memory
      u8g2.sendBuffer();                  // transfer internal memory to the display
    }
  }

  pointer = POT_value;

  // pointer = 5;

    u8g2.clearBuffer();                 // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.drawStr(0, 10, "ENTERED THE MODE");     // write something to the internal memory
    u8g2.sendBuffer();                  // transfer internal memory to the display

  delay(1000);

}


