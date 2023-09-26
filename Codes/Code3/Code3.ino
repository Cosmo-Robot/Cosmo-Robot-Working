// THIS IS FIRST AND SIMPLEST WORKING CODE

#include <Arduino.h>

#define Motor_Right_Forward 41
#define Motor_Right_Backward 43
#define Motor_Left_Forward 45
#define Motor_Left_Backward 47

#define IR_1 6  // Leftmost
#define IR_2 7
#define IR_3 8
#define IR_4 9
#define IR_5 10
#define IR_6 11
#define IR_7 12
#define IR_8 13  //Righmost

// bool Switch = 0; //0 for left ; 1 for right;
int x = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(Motor_Right_Forward, OUTPUT);
  pinMode(Motor_Right_Backward, OUTPUT);
  pinMode(Motor_Left_Forward, OUTPUT);
  pinMode(Motor_Left_Backward, OUTPUT);

  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  pinMode(IR_4, INPUT);
  pinMode(IR_5, INPUT);
  pinMode(IR_6, INPUT);
  pinMode(IR_7, INPUT);
  pinMode(IR_8, INPUT);


  // digitalWrite(Motor_Right_Forward,1);

}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if (digitalRead(ir_LEFT) == 1){
    Switch = 0;
  } 

  if (digitalRead(ir_RIGHT) == 1){
    Switch = 1;
  }

  if (Switch == 1){
    digitalWrite(Motor_Left_Forward,1);
    digitalWrite(Motor_Right_Forward,0);
  }else{
    digitalWrite(Motor_Left_Forward,0);
    digitalWrite(Motor_Right_Forward,1);
  }

  */

  if (digitalRead(IR_1) == 1 || digitalRead(IR_2) || digitalRead(IR_3) == 1){
    x = 1;
  } 
  else if (digitalRead(IR_5) == 1 || digitalRead(IR_6) || digitalRead(IR_7) == 1){
    x = 0;
  }
  else {
    x = 2;
  }

  if (x == 0){
    digitalWrite(Motor_Left_Forward,1);
    digitalWrite(Motor_Right_Forward,0);
  }else if (x == 1){
    digitalWrite(Motor_Left_Forward,0);
    digitalWrite(Motor_Right_Forward,1);
  }
  else {
    digitalWrite(Motor_Left_Forward,1);
    digitalWrite(Motor_Right_Forward,1);
  }

  }

