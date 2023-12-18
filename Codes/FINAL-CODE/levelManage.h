// lineFollow.h

#include <Arduino.h>
#include <U8g2lib.h>
#include <Servo.h>

extern Servo myservo; // create servo object to control a servo

// Header guard to prevent multiple inclusions
#ifndef LEVEL_MANAE_H
#define LEVEL_MANAE_H

void motor(int motor, int direction);
bool squareDetected();
bool TDetected();
void lineFollow();
int ultrasonicDistance(int sensor);
void readIRArray();

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

extern U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2;

extern bool Switch;
extern bool pull_bend;

extern bool pull_box_arm_retract;

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

// pin for buzzer
#define buzzer_pin 8

// ========== LINE_COLOR DEFINITION ==========
extern const String LINE_COLOR;

// IR_array 8 space empty array
#define IR_ARRAY_LENGTH 12
extern int IR_array[IR_ARRAY_LENGTH];

extern int Ir_thresholds[];

extern int temp;

extern int array_lit_amount;

extern int prev_error_history[];
extern int stack_pointer;

extern int Left_drive;
extern int Right_drive;

// pin for right bend sensor
#define right_bend_sensor 9

// define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2


void levelManage();
#endif
