#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>
#include <U8g2lib.h>

#include "lineFollow.h"
#include "detections.h"
#include "sensors.h"
#include "levelManage.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

/////////////////////////////////Definitions///////////////////////////////////////////////////////////////////////////////////////////////
// ========== LINE_COLOR DEFINITION ==========
const String LINE_COLOR = "WHITE";
int array_lit_amount = 0;
// Define the size of the array and the EEPROM address to start writing data
const int arraySize = 8;     // Adjust this based on your array size
const int eepromAddress = 0; // EEPROM address where you want to start writing

#define Debug_led 13

bool wallVar = true;
// PID parameters for Line Following
double Kp = 8;
double Ki = 0;
double Kd = 7;

// PID parameters for Ultrasonic Wall Following
double UKp = 35;
double UKi = 0;
double Ukd = 20;

int Drive_constant = 200;

// Initialize PID variables
double prevError = 0;
double integral = 0;
double derivative = 0;

// recorrect
int Ir_thresholds[] = {300, 300, 100, 100, 100, 105, 400, 400};

int prev_error_history[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int stack_pointer = 0;

int IR_history_count = 0;
int IR_history = 0;
int IR_first_grabbed = 0;

int guard_left = 0;
int guard_right = 0;
int guard_left_prev = 2;
int guard_right_prev = 2;

int count = 0;

bool pull_box_arm_retract = false;
bool pull_bend = false;

// define trig pin 6 and echo pin 7
#define trigPin 6
#define echoPin 7

// pushbutton input for calibration
#define calibration 7

// pin for buzzer
#define buzzer_pin 8

bool bend_done = false;

// pin for right bend sensor
#define right_bend_sensor 9

bool Switch = 1;

bool bend_start = false;

bool bend_condition = false;

int now_time = 0;

String mode = "normal";
String wall_side = "";

Servo myservo; // create servo object to control a servo

enum Stage // This is for the final competition.
{
  LINE = 1,
  WALL,
  RAMP,
  PULL,
  COLOR,
  SOUND,
  GUARD,
  HALT
};
// If initial square is also counted add something first and assign it to TASK.
Stage TASK = LINE;  // 1 - Line Following, 2 - Wall Following, etc.
int Stage_no = 256; // 1 - Line Following, 2 - Wall Following, etc.

String STAGES[] = {"LINE", "WALL", "RAMP", "PULL", "COLOR", "SOUND", "GUARD", "HALT"};
int pointer = 0;
// int TASK = 1; // 1 - Line Following, 2 - Wall Following, etc.

// IR_array 8 space empty array
#define IR_ARRAY_LENGTH 12
int IR_array[IR_ARRAY_LENGTH];

int Threshold = 100;

int prev_position = 0;
int prev_history_sum = 0;

// define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2

#define Motor_Right_Forward 28
#define Motor_Right_Backward 24
#define Motor_Left_Forward 22
#define Motor_Left_Backward 26

// Motor and Direction Definitions
#define RIGHT_MOTOR 1
#define LEFT_MOTOR 2
#define FORWARD 1
#define BACKWARD 2

// Sound sensor
#define SOUND_SENSOR A10
#define SOUND_THRESHOLD 300
#define SOUND_WAIT_TIME 500 // milliseconds
#define SOUND_LED 13
unsigned long int sound_now_time;

// bool Switch = 0; //0 for left ; 1 for right;
int x = 0;

// Ultrasonic sensors
#define SENSOR_LEFT 1
#define SENSOR_RIGHT 2

#define SENSOR_LEFT_TRIG_PIN 32
#define SENSOR_LEFT_ECHO_PIN 34

#define SENSOR_RIGHT_TRIG_PIN 36
#define SENSOR_RIGHT_ECHO_PIN 38

float ultrasonic_prev_error = 0;

#define MAX_ULTRASONIC_DISTANCE 30 // in centimeters

// Task selection switch
#define TASK_SWITCH 44
#define SWITCH_MSB 45
#define SWITCH_MID_SB 46
#define SWITCH_LSB 47

int sound_threshold = 500;

/////////////////////////////////Function Declarations/////////////////////////////////////////////////////////////////////////////////////
void motor(int motor, int direction);
void buzzer_beep();
Stage getStageFromSwitches();
void wallFollow();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////Setup//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{

  

    Serial.begin(115200);
     u8g2.begin();

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
  
 

  pinMode(buzzer_pin, OUTPUT);

  // pin 8 is buzzer, tur it on and off for 2 quick beeps
  buzzer_beep();
  delay(100);
  buzzer_beep();

  // pinmode trig and echo
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // attaches the servo on pin 11 to the servo object
  myservo.attach(11);

  for (int i = 0; i < IR_ARRAY_LENGTH; i++)
  {
    byte temp = 0;
    // Ir_thresholds[i] = EEPROM.read(i)*4;
    EEPROM.get(i, temp);
    Ir_thresholds[i] = temp;
  }

  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < IR_ARRAY_LENGTH; i++)
  {
    Serial.print(Ir_thresholds[i]);
    Serial.print(", ");
  }
  Serial.println("};    ");

  pinMode(Motor_Right_Forward, OUTPUT);
  pinMode(Motor_Right_Backward, OUTPUT);
  pinMode(Motor_Left_Forward, OUTPUT);
  pinMode(Motor_Left_Backward, OUTPUT);

  pinMode(right_bend_sensor, INPUT);

  motor(RIGHT_MOTOR, FORWARD);
  motor(LEFT_MOTOR, FORWARD);

  pinMode(calibration, INPUT);

  sound_now_time = millis();

  pinMode(SENSOR_LEFT_ECHO_PIN, INPUT);
  pinMode(SENSOR_LEFT_TRIG_PIN, OUTPUT);
  pinMode(SENSOR_RIGHT_ECHO_PIN, INPUT);
  pinMode(SENSOR_RIGHT_TRIG_PIN, OUTPUT);

  pinMode(TASK_SWITCH, INPUT);
  pinMode(SWITCH_MSB, INPUT);
  pinMode(SWITCH_MID_SB, INPUT);
  pinMode(SWITCH_LSB, INPUT);

  pinMode(12, INPUT);

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

int temp = 0;
int Left_drive = 0;
int Right_drive = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////Loop//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  // while(1) {
  //     analogWrite(ENL, 230);
  //     analogWrite(ENR, 170);
  //     motor(LEFT_MOTOR, FORWARD);
  //     motor(RIGHT_MOTOR, FORWARD);
  // }

  if (digitalRead(calibration) == 1)
  {
    Serial.println("Calibrating...");
    calibrate();
  }

  // Automatically increment task after detecting a white square.
  levelManage();

  if (Switch == 1)
  {
    analogWrite(ENL, Left_drive);
    analogWrite(ENR, Right_drive);
  }

  ///////////////////// 8]Tasks completed////////////////////////////////////////
  else
  {
    analogWrite(ENL, 0);
    analogWrite(ENR, 0);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////FUNCTIONS//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////For debugging ///////////////////////////////////////////

void buzzer_beep()
{
  digitalWrite(buzzer_pin, 1);
  delay(100);
  digitalWrite(buzzer_pin, 0);
}

///////////////////////////Motor direction choosing///////////////////////////////////////////

void motor(int motor, int direction)
{
  if (motor == RIGHT_MOTOR)
  {
    if (direction == FORWARD)
    {
      digitalWrite(Motor_Right_Forward, HIGH);
      digitalWrite(Motor_Right_Backward, LOW);
    }
    else if (direction == BACKWARD)
    {
      digitalWrite(Motor_Right_Forward, LOW);
      digitalWrite(Motor_Right_Backward, HIGH);
    }
  }
  else if (motor == LEFT_MOTOR)
  {
    if (direction == 1)
    {
      digitalWrite(Motor_Left_Forward, HIGH);
      digitalWrite(Motor_Left_Backward, LOW);
    }
    else if (direction == BACKWARD)
    {
      digitalWrite(Motor_Left_Forward, LOW);
      digitalWrite(Motor_Left_Backward, HIGH);
    }
  }
}

//////////////////////////////For getting the stage from switches///////////////////////////////////////////

Stage getStageFromSwitches()
{
  bool msb = digitalRead(SWITCH_MSB);
  bool midsb = digitalRead(SWITCH_MID_SB);
  bool lsb = digitalRead(SWITCH_LSB);

  int level = msb * 4 + midsb * 2 + lsb * 1;

  switch (level)
  {
  case 1:
    return LINE;
  case 2:
    return WALL;
  case 3:
    return RAMP;
  case 4:
    return PULL;
  case 5:
    return COLOR;
  case 6:
    return SOUND;
  case 7:
    return GUARD;
  default:
    return HALT;
  }
}

