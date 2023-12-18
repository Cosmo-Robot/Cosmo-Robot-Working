#include "lineFollow.h"

void lineFollow()
{
  readIRArray();

  motor(RIGHT_MOTOR, FORWARD);
  motor(LEFT_MOTOR, FORWARD);

  array_lit_amount = 0;
  for (int i = 0; i < IR_ARRAY_LENGTH; i++)
  {
    array_lit_amount += IR_array[i];
  }

  // The following two lines should change with respect to IR array length.

  // For 8 IR array.
  // float left_sum = -10 * IR_array[0] - 7 * IR_array[1] - 2 * IR_array[2] - 1 * IR_array[3];
  // float right_sum = 1 * IR_array[4] + 2 * IR_array[5] + 7 * IR_array[6] + 10 * IR_array[7];

  // For 12 extended IR array.
  float left_sum = -20 * IR_array[0] - 14 * IR_array[1] - 10 * IR_array[2] - 7 * IR_array[3] - 2 * IR_array[4] - 1 * IR_array[5];
  float right_sum = 1 * IR_array[6] + 2 * IR_array[7] + 7 * IR_array[8] + 10 * IR_array[9] + 14 * IR_array[10] + 20 * IR_array[11];

  int position = left_sum + right_sum;

  prev_error_history[stack_pointer] = position;
  stack_pointer++;
  if (stack_pointer == 10)
  {
    stack_pointer = 0;
  }

  derivative = position - prevError;

  int PID_constant = Kp * position + Kd * derivative;

  prevError = position;

  Drive_constant = 200;

  int offset = 30; // For correcting motor speeds
  Left_drive = Drive_constant + offset + PID_constant;
  Right_drive = Drive_constant - offset - PID_constant;

  // Limiting to 0 - 255 range
  Left_drive = min(max(Left_drive, 0), 255);
  Right_drive = min(max(Right_drive, 0), 255);

  // if ((array_lit_amount == 0 or (IR_array[0] == 0 and IR_array[1] == 0 and IR_array[2] == 1 and IR_array[3] == 0 and IR_array[4] == 0 and IR_array[5] == 0 and IR_array[6] == 0 and IR_array[7] == 0))and mode != "bend")
  if (array_lit_amount == 0 or (IR_array[0] == 0 and IR_array[1] == 0 and IR_array[2] == 0 and IR_array[3] == 0 and IR_array[4] == 1 and IR_array[5] == 0 and IR_array[6] == 0 and IR_array[7] == 0 and IR_array[8] == 0 and IR_array[9] == 0 and IR_array[10] == 0 and IR_array[11] == 0))
  {
    Left_drive = 0;
    Right_drive = 0;
    analogWrite(ENL, Left_drive);
    analogWrite(ENR, Right_drive);
    digitalWrite(Debug_led, HIGH);
    delay(50);
    digitalWrite(Debug_led, LOW);

    readIRArray();

    for (int i = 0; i < IR_ARRAY_LENGTH; i++)
    {
      array_lit_amount += IR_array[i];
    }

    if (array_lit_amount == 0 or (IR_array[0] == 0 and IR_array[1] == 0 and IR_array[2] == 0 and IR_array[3] == 0 and IR_array[4] == 1 and IR_array[5] == 0 and IR_array[6] == 0 and IR_array[7] == 0 and IR_array[8] == 0 and IR_array[9] == 0 and IR_array[10] == 0 and IR_array[11] == 0))
    {
      mode = "deviated";
      // buzzer_beep();
      // delay(20);
      // buzzer_beep();
      // delay(20);
      // buzzer_beep();
      // delay(20);
      // buzzer_beep();
      // delay(20);
      // buzzer_beep();
      // delay(20);
    }
  }

  // if (mode == "normal" and bend_done == false){
  //   //bend detection
  //   if(right_sum >= 8.5 && left_sum > -4){
  //     //breaking
  //     motor(RIGHT_MOTOR, BACKWARD);
  //     motor(LEFT_MOTOR, BACKWARD);
  //     now_time = millis();
  //     mode = "bend";

  //     bend_done = true;

  //     bend_start = true;

  //   }
  // }

  // // Parking
  // if (array_lit_amount == 12){
  //   digitalWrite(13, HIGH);
  // }
  // if (array_lit_amount >= 10){
  //   Switch = 0;
  //   myservo.write(120);
  // }

  if (mode == "bend")
  {
    if (bend_start == true)
    {
      bend_start = false;
      bend_condition = false;

      while (digitalRead(right_bend_sensor) == 1)
      {
        digitalWrite(buzzer_pin, HIGH);
        Serial.print("in loop:  ");
        Serial.println(digitalRead(right_bend_sensor));
        motor(RIGHT_MOTOR, FORWARD);
        motor(LEFT_MOTOR, FORWARD);
        Left_drive = 140;
        Right_drive = 140;
        analogWrite(ENL, Left_drive);
        analogWrite(ENR, Right_drive);
      }
      analogWrite(ENL, 0);
      analogWrite(ENR, 0);
      // delay(800);
      array_lit_amount = 0;
    }
    // analogWrite(ENL,0);
    // analogWrite(ENR,0);
    // delay(1000);

    digitalWrite(buzzer_pin, LOW);
    // buzzer_beep();

    Serial.println(array_lit_amount);

    motor(RIGHT_MOTOR, BACKWARD);
    motor(LEFT_MOTOR, FORWARD);

    Left_drive = 155;
    Right_drive = 155;
    Serial.println("in turning mode");

    // detecting if the line contacts the sensor
    if (bend_condition == false)
    {
      if (array_lit_amount >= 2)
      {

        bend_condition = true;
      }
    }
    else
    {
      // detecting if the line comes to the middle
      if (IR_array[3] == 1 or IR_array[4] == 1)
      {

        Serial.print("bend over");
        mode = "normal";

        // motor(RIGHT_MOTOR, BACKWARD);
        // motor(LEFT_MOTOR, BACKWARD);

        bend_condition = false;

        buzzer_beep();
        delay(100);
        buzzer_beep();

        analogWrite(ENL, 0);
        analogWrite(ENR, 0);
        delay(500);
      }
    }
  }

  //  // Deviation detection
  if (mode == "deviated")
  {
    for (int p = 0; p < 10; p++)
    {
      prev_history_sum += prev_error_history[p];
    }

    if (prev_history_sum > 0)
    {
      motor(RIGHT_MOTOR, BACKWARD);
      motor(LEFT_MOTOR, FORWARD);
      Left_drive = 230;
      Right_drive = 230;
      analogWrite(ENL, Left_drive);
      analogWrite(ENR, Right_drive);
      delay(300);
    }
    else if (prev_history_sum < 0)
    {
      motor(RIGHT_MOTOR, FORWARD);
      motor(LEFT_MOTOR, BACKWARD);
      Left_drive = 230;
      Right_drive = 230;
      analogWrite(ENL, Left_drive);
      analogWrite(ENR, Right_drive);
      delay(300);
    }
    else
    {
      motor(RIGHT_MOTOR, BACKWARD);
      motor(LEFT_MOTOR, BACKWARD);
      Left_drive = 200;
      Right_drive = 200;
      analogWrite(ENL, Left_drive);
      analogWrite(ENR, Right_drive);
      delay(300);
    }
    prev_history_sum = 0;
    mode = "normal";
  }

  prev_position = position;
}