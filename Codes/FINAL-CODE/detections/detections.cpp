#include "detections.h"

//////////////////////////////For detecting a white square///////////////////////////////////////////

bool squareDetected()
{

  if (array_lit_amount >= 11)
  {
    motor(LEFT_MOTOR, FORWARD);
    motor(RIGHT_MOTOR, FORWARD);
    analogWrite(ENL, 230);
    analogWrite(ENR, 170);
    delay(500);
    analogWrite(ENL, 0);
    analogWrite(ENR, 0);

    readIRArray();
    array_lit_amount = 0;
    for (int i = 0; i < IR_ARRAY_LENGTH; i++)
    {
      array_lit_amount += IR_array[i];
    }
    if (array_lit_amount >= 11)
    {
      motor(LEFT_MOTOR, BACKWARD);
      motor(RIGHT_MOTOR, BACKWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);
      delay(500);
      Serial.print("Square detected when: ");
      Serial.println(array_lit_amount);
      return true;
    }
    else
    {
      motor(LEFT_MOTOR, BACKWARD);
      motor(RIGHT_MOTOR, BACKWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);
      delay(500);
      return false;
    }
  }
  else
  {
    return false;
  }

  // { // Suspect this as a detection of white all long the array.
  //   // This can either be a T junction or a white square.

  //   // Go some distance forward and check again.
  //   motor(LEFT_MOTOR, FORWARD);
  //   analogWrite(ENL, Drive_constant);
  //   motor(RIGHT_MOTOR, FORWARD);
  //   analogWrite(ENR, Drive_constant);

  //   delay(750); // Change accordingly.

  //   analogWrite(ENL, 0);
  //   analogWrite(ENR, 0);

  //   readIRArray();
  //   array_lit_amount = 0;
  //   for (int i = 0; i < IR_ARRAY_LENGTH; i++)
  //   {
  //     array_lit_amount += IR_array[i];
  //   }

  //   if (array_lit_amount == IR_ARRAY_LENGTH)
  //   { // 2 here is a safety factor.
  //     // Should turn and align with a normal line before returning.
  //     return true;
  //   }
  //   else
  //   {
  //     // Coming back a little
  //     motor(LEFT_MOTOR, BACKWARD);
  //     analogWrite(ENL, Drive_constant);
  //     motor(RIGHT_MOTOR, BACKWARD);
  //     analogWrite(ENR, Drive_constant);

  //     delay(600); // Change accordingly.

  //     analogWrite(ENL, 0);
  //     analogWrite(ENR, 0);
  //     return false;
  //   }
  // }
  // else
  // {
  //   return false;
  // }
}


bool TDetected()
{

  if (array_lit_amount >= 11)
  {
    motor(LEFT_MOTOR, FORWARD);
    motor(RIGHT_MOTOR, FORWARD);
    analogWrite(ENL, 230);
    analogWrite(ENR, 170);
    delay(500);
    analogWrite(ENL, 0);
    analogWrite(ENR, 0);

    readIRArray();
    array_lit_amount = 0;
    for (int i = 0; i < IR_ARRAY_LENGTH; i++)
    {
      array_lit_amount += IR_array[i];
    }
    if (array_lit_amount >= 11)
    {
      motor(LEFT_MOTOR, BACKWARD);
      motor(RIGHT_MOTOR, BACKWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);
      delay(500);
      Serial.print("Square detected when: ");
      Serial.println(array_lit_amount);
      return false;
    }
    else
    {
      motor(LEFT_MOTOR, BACKWARD);
      motor(RIGHT_MOTOR, BACKWARD);
      analogWrite(ENL, 230);
      analogWrite(ENR, 170);
      delay(500);
      return true;
    }
  }
  else
  {
    return false;
  }

  // { // Suspect this as a detection of white all long the array.
  //   // This can either be a T junction or a white square.

  //   // Go some distance forward and check again.
  //   motor(LEFT_MOTOR, FORWARD);
  //   analogWrite(ENL, Drive_constant);
  //   motor(RIGHT_MOTOR, FORWARD);
  //   analogWrite(ENR, Drive_constant);

  //   delay(750); // Change accordingly.

  //   analogWrite(ENL, 0);
  //   analogWrite(ENR, 0);

  //   readIRArray();
  //   array_lit_amount = 0;
  //   for (int i = 0; i < IR_ARRAY_LENGTH; i++)
  //   {
  //     array_lit_amount += IR_array[i];
  //   }

  //   if (array_lit_amount == IR_ARRAY_LENGTH)
  //   { // 2 here is a safety factor.
  //     // Should turn and align with a normal line before returning.
  //     return true;
  //   }
  //   else
  //   {
  //     // Coming back a little
  //     motor(LEFT_MOTOR, BACKWARD);
  //     analogWrite(ENL, Drive_constant);
  //     motor(RIGHT_MOTOR, BACKWARD);
  //     analogWrite(ENR, Drive_constant);

  //     delay(600); // Change accordingly.

  //     analogWrite(ENL, 0);
  //     analogWrite(ENR, 0);
  //     return false;
  //   }
  // }
  // else
  // {
  //   return false;
  // }
}
