#include "levelManage.h"

// /////////////////////////////////Definitions///////////////////////////////////////////////////////////////////////////////////////////////
void level6(){
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