//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Creator:  Team Cosmo
//members: 1)Yasiru Basnayake
//         2)Kavindu Jayathissa
//         3)Lasith Haputhanthri
//         4)Linuka Ratnayake
//         5)Hasitha Gallella
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>
#include <U8g2lib.h>

// Header guard to prevent multiple inclusions
#include "lineFollow.h"
#include "detections.h"
#include "sensors.h"
#include "levelManage.h"

/////////////////////////////////Object Definitions///////////////////////////////////////////////////////////////////////////////////////////////

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

Servo arm; // create servo object to control a servo

/////////////////////////////////Definitions///////////////////////////////////////////////////////////////////////////////////////////////

#define Debug_led 13

#define trigPin 6 // define trig pin 6 and echo pin 7
#define echoPin 7

#define calibration 7 // pushbutton input for calibration

#define buzzer_pin 8 // pin for buzzer

#define right_bend_sensor 9 // pin for right bend sensor

#define IR_ARRAY_LENGTH 12 // IR_array 8 space empty array

#define ENR 3 // define right motor enable pin is 3 and left 2
#define ENL 2

#define Motor_Right_Forward 28
#define Motor_Right_Backward 24
#define Motor_Left_Forward 22
#define Motor_Left_Backward 26

#define RIGHT_MOTOR 1 // Motor and Direction Definitions
#define LEFT_MOTOR 2
#define FORWARD 1
#define BACKWARD 2

#define SOUND_SENSOR A10 // Sound sensor
#define SOUND_THRESHOLD 300
#define SOUND_WAIT_TIME 500 // milliseconds
#define SOUND_LED 13

#define SENSOR_LEFT 1 // Ultrasonic sensors
#define SENSOR_RIGHT 2

#define SENSOR_LEFT_TRIG_PIN 32
#define SENSOR_LEFT_ECHO_PIN 34

#define SENSOR_RIGHT_TRIG_PIN 36
#define SENSOR_RIGHT_ECHO_PIN 38

#define MAX_ULTRASONIC_DISTANCE 30 // in centimeters

#define TASK_SWITCH 44 // Task selection switch
#define SWITCH_MSB 45
#define SWITCH_MID_SB 46
#define SWITCH_LSB 47

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

const String LINE_COLOR = "WHITE";
int array_lit_amount = 0;
// Define the size of the array and the EEPROM address to start writing data
const int arraySize = 8;     // Adjust this based on your array size
const int eepromAddress = 0; // EEPROM address where you want to start writing

bool bend_done = false;

bool Switch = 1;

bool bend_start = false;

bool bend_condition = false;

int now_time = 0;

String mode = "normal";
String wall_side = "";

String STAGES[] = {"LINE", "WALL", "RAMP", "PULL", "COLOR", "SOUND", "GUARD", "HALT"};
int pointer = 0;

int IR_array[IR_ARRAY_LENGTH];

int Threshold = 100;

int prev_position = 0;
int prev_history_sum = 0;

int x = 0; // bool Switch = 0; //0 for left ; 1 for right;

float ultrasonic_prev_error = 0;

int sound_threshold = 500;

int temp = 0;

int Left_drive = 0;
int Right_drive = 0;

/////////////////////////////////Function Declarations/////////////////////////////////////////////////////////////////////////////////////

void motor(int , int);
void buzzer_beep();
void wallFollow();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////Setup//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  // put your setup code here, to run once:

  //begin serial communication
  Serial.begin(115200);
  u8g2.begin();

  soundCalibrate(); // Calibrate the sound sensor

  // pinmode trig and echo
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer_pin, OUTPUT);

  pinMode(Motor_Right_Forward, OUTPUT);
  pinMode(Motor_Right_Backward, OUTPUT);
  pinMode(Motor_Left_Forward, OUTPUT);
  pinMode(Motor_Left_Backward, OUTPUT);
  
  pinMode(right_bend_sensor, INPUT);

  pinMode(calibration, INPUT);

  pinMode(SENSOR_LEFT_ECHO_PIN, INPUT);
  pinMode(SENSOR_LEFT_TRIG_PIN, OUTPUT);
  pinMode(SENSOR_RIGHT_ECHO_PIN, INPUT);
  pinMode(SENSOR_RIGHT_TRIG_PIN, OUTPUT);

  pinMode(TASK_SWITCH, INPUT);
  pinMode(SWITCH_MSB, INPUT);
  pinMode(SWITCH_MID_SB, INPUT);
  pinMode(SWITCH_LSB, INPUT);

  pinMode(12, INPUT);

  //reading the IR thresholds from EEPROM
  for (int i = 0; i < IR_ARRAY_LENGTH; i++)
  {
    byte temp = 0;
    // Ir_thresholds[i] = EEPROM.read(i)*4;
    EEPROM.get(i, temp);
    Ir_thresholds[i] = temp;
  }

  //for motor initialization
  motor(RIGHT_MOTOR, FORWARD);
  motor(LEFT_MOTOR, FORWARD);

  modeSelect(); // Select the mode of operation
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////Loop//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{

  // Automatically increment task after detecting a white square.
  levelManage();

  if (Switch != 1)
  {
    Left_drive = 0;
    Right_drive = 0;
  }

  //signalling the speed to the motors
  analogWrite(ENL, Left_drive);
  analogWrite(ENR, Right_drive);

  // put your main code here, to run repeatedly:
  if (digitalRead(calibration) == 1){calibrate();}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////FUNCTIONS//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////For debugging ///////////////////////////////////////////

void buzzer_beep()
{
  digitalWrite(buzzer_pin, 1);
  delay(100);
  digitalWrite(buzzer_pin, 0);
}


