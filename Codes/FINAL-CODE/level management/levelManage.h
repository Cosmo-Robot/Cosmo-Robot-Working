// lineFollow.h

#include <Arduino.h>
#include <U8g2lib.h>
#include <Servo.h>

#include <Wire.h>
#include "Adafruit_TCS34725.h"

extern Adafruit_TCS34725 tcs;


// External servo object to control a servo
extern Servo arm;

// Header guard to prevent multiple inclusions
#ifndef LEVEL_MANAE_H
#define LEVEL_MANAE_H

// Function prototypes
void motor(int motor, int direction);
bool squareDetected();
bool TDetected();
void lineFollow();
bool CrossDetected();
int ultrasonicDistance(int sensor);
void readIRArray();
void levelManage();
void modeSelect();
void checkpoint();
void level1();
void level2();
void level3();
void level4();
void level5();
void level6();
void level7();

// Constants and variables
extern U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2;

extern bool Switch;
extern bool pull_bend;
extern bool pull_box_arm_retract;
extern bool boxGrabbed;

extern int guard_left;
extern int guard_right;
extern int guard_left_prev;
extern int guard_right_prev;
extern int count;
extern int sound_threshold;
extern int Drive_constant;
extern int pointer;

#define MAX_ULTRASONIC_DISTANCE 30 // in centimeters

// Motor and Direction Definitions
#define RIGHT_MOTOR 1
#define LEFT_MOTOR 2
#define FORWARD 1
#define BACKWARD 2

// Ultrasonic sensors
#define SENSOR_LEFT 1
#define SENSOR_RIGHT 2

#define SENSOR_LEFT_TRIG_PIN 32
#define SENSOR_LEFT_ECHO_PIN 34

#define SENSOR_RIGHT_TRIG_PIN 36
#define SENSOR_RIGHT_ECHO_PIN 38

// Pin for buzzer
#define buzzer_pin 8

//sharpIR pin
#define front_distane_sensor A15 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)


// Line color definition
extern const String LINE_COLOR;

// IR_array and related variables
#define IR_ARRAY_LENGTH 12
extern int IR_array[IR_ARRAY_LENGTH];
extern int Ir_thresholds[];
extern int temp;
extern int array_lit_amount;
extern int prev_error_history[];
extern int stack_pointer;
extern int Left_drive;
extern int Right_drive;

extern String color_picked;

extern int crossCount;

extern bool colorBoxGrabbed;

// Pin for right bend sensor
#define right_bend_sensor 9

// Motor enable pin definitions
#define ENR 3
#define ENL 2

// Calibration pushbutton input pin
#define calibration 7

#endif
