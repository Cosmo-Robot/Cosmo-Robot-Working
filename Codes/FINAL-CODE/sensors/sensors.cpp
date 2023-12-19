#include "sensors.h"

///////////////////////////Calibarating IR Thresholds///////////////////////////////////////////

void calibrate()
{

  motor(RIGHT_MOTOR, BACKWARD);
  motor(LEFT_MOTOR, FORWARD);

  // Stop motors when calibrating
  analogWrite(ENL, 255);
  analogWrite(ENR, 255);

  // make sensor_max_values array of length 8 equal to sensor calibration array
  int sensor_max_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  // sensor min values array
  int sensor_min_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  int now_time_for_calibration = millis();
  Serial.print("now time for calibration: ");
  Serial.print(now_time_for_calibration);
  Serial.print("  millis: ");
  Serial.println(millis());

  while (millis() - now_time_for_calibration < 8000)
  {
    // Serial.println("Caliberating IR");
    digitalWrite(buzzer_pin, 1);

    bool allZeros = true;
    for (int i = 0; i < 8; i++)
    {
      if (sensor_max_values[i] != 0)
      {
        allZeros = false;
        break; // No need to continue checking if we find a non-zero value
      }
    }

    if (allZeros)
    {
      for (int i = 0; i < 8; i++)
      {
        sensor_max_values[i] = analogRead(i);
        sensor_min_values[i] = analogRead(i);
      }
    }

    for (int i = 0; i < 8; i++)
    {
      if (analogRead(i) > sensor_max_values[i])
      {
        sensor_max_values[i] = analogRead(i);
      }
      if (analogRead(i) < sensor_min_values[i])
      {
        sensor_min_values[i] = analogRead(i);
      }
    }
  }
  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(sensor_max_values[i]);
    Serial.print(", ");
  }
  Serial.print("};    ");
  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(sensor_min_values[i]);
    Serial.print(", ");
  }
  Serial.print("};    ");

  for (int i = 0; i < 8; i++)
  {
    Ir_thresholds[i] = (sensor_max_values[i] + sensor_min_values[i]) / 2;
  }

  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(Ir_thresholds[i]);
    Serial.print(", ");
  }
  Serial.println("};    ");

  // save the array to eeprom
  for (int i = 0; i < 8; i++)
  {
    // EEPROM.write(i, byte(Ir_thresholds[i]/4));
    EEPROM.put(i, byte(Ir_thresholds[i]));
  }

  delay(1000);

  digitalWrite(buzzer_pin, 0);

  analogWrite(ENL, 0);
  analogWrite(ENR, 0);

  delay(4000);
}

///////////////////////////Ultrasonic sensor reading///////////////////////////////////////////

int ultrasonicDistance(int sensor)
{
  int distance = 0;
  if (sensor == SENSOR_LEFT)
  {
    digitalWrite(SENSOR_LEFT_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(SENSOR_LEFT_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SENSOR_LEFT_TRIG_PIN, LOW);
    distance = pulseIn(SENSOR_LEFT_ECHO_PIN, HIGH) / 2 * 0.034;
  }
  else if (sensor == SENSOR_RIGHT)
  {
    digitalWrite(SENSOR_RIGHT_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(SENSOR_RIGHT_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(SENSOR_RIGHT_TRIG_PIN, LOW);
    distance = pulseIn(SENSOR_RIGHT_ECHO_PIN, HIGH) / 2 * 0.034;
  }
  return distance;
}


///////////////////////////Updating the IR array elements///////////////////////////////////////////
void readIRArray()
{
  for (int i = 0; i < IR_ARRAY_LENGTH - 4; i++)
  {
    // Serial.print(analogRead(i));
    // Serial.print(" ");
    if (LINE_COLOR == "WHITE")
    {
      temp = analogRead(i) <= Ir_thresholds[i];
    }
    else if (LINE_COLOR == "BLACK")
    {
      temp = analogRead(i) > Ir_thresholds[i];
    }
    // IR_array[IR_ARRAY_LENGTH - 1 - i] = temp;

    // IR_array[i] = temp; // This is correct if pins are connected in order.
    IR_array[i + 2] = temp;

    // This part is needed because the pins are connected not in order.
    // if (i < 8) {
    //     IR_array[i + 2] = temp;
    // }
    // else {
    //     if (i == 8) IR_array[0] = temp;
    //     else if (i == 9) IR_array[1] = temp;
    //     else if (i == 10) IR_array[10] = temp;
    //     else if (i == 11) IR_array[11] = temp;
    // }
  }

  IR_array[0] = 1 - digitalRead(A11);
  IR_array[1] = 1 - digitalRead(A10);
  IR_array[10] = 1 - digitalRead(A9);
  IR_array[11] = 1 - digitalRead(A8);

  // for (int i = 0; i < IR_ARRAY_LENGTH; i++)
  // {
  //   Serial.print(IR_array[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
}


///////////////////////////Sound Calibration///////////////////////////////////////////
void soundCalibrate()
{
  int now_time = millis();
  int max_sound = 0;
  int min_sound = 1024;
  u8g2.clearBuffer();                 // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0, 10, "SOUND CALIBERATING");        // write something to the internal memory
  u8g2.sendBuffer();
  while (millis()-now_time <= 5000){
    int analog_sound_read = analogRead(A13);
    Serial.println(analog_sound_read);
    max_sound = max(analog_sound_read, max_sound);
    min_sound = min(analog_sound_read,min_sound);
  }
  sound_threshold = (max_sound + min_sound)/2;
  Serial.println(sound_threshold);
}
