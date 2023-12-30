#include "levelManage.h"

//////////////////////////////Managing Levels///////////////////////////////////////////
void levelManage(){
  if (squareDetected())
    {
      checkpoint();
    }

    ///////////////////////// 1] LINE FOLLOWING///////////////////////////////////////////
    if (pointer == 0)
    {
      level1();
    }
    ///////////////////////// 2] WALL FOLLOWING///////////////////////////////////////////
    else if (pointer == 1)
    {
      level2();
    }
    ///////////////////////// 3] RAMP ///////////////////////////////////////////
    else if (pointer == 2)
    {
      level3();
    }
    ///////////////////////// 4] PULL the Box///////////////////////////////////////////
    else if (pointer == 3)
    {
      level4();
    }
    ///////////////////////// 5] Color FOLLOWING///////////////////////////////////////////
    else if (pointer == 4)
    {
      level5(); 
    }
    ///////////////////////// 6] Sound Game ///////////////////////////////////////////
    else if (pointer == 5) 
    {
      level6();
    }
    ///////////////////////// 7] GUARD AVOIDING///////////////////////////////////////////
    else if (pointer == 6)
    {
      level7();
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


