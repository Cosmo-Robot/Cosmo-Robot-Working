// lineFollow.h

#include <Arduino.h>

// Header guard to prevent multiple inclusions
#ifndef LINE_FOLLOW_H
#define LINE_FOLLOW_H

void readIRArray();
void motor(int motor, int direction);
void buzzer_beep();

// PID parameters for Line Following
extern double Kp;
extern double Ki;
extern double Kd;

extern int Drive_constant;

// Initialize PID variables
extern double prevError;
extern double integral;
extern double derivative;

// Motor and Direction Definitions
#define RIGHT_MOTOR 1
#define LEFT_MOTOR 2
#define FORWARD 1
#define BACKWARD 2

// IR_array 8 space empty array
#define IR_ARRAY_LENGTH 12
extern int IR_array[IR_ARRAY_LENGTH];

extern int array_lit_amount;

extern int prev_error_history[];
extern int stack_pointer;

extern int Left_drive;
extern int Right_drive;

extern String mode;

extern bool bend_start;

extern bool bend_condition;

// pin for right bend sensor
#define right_bend_sensor 9

#define buzzer_pin 8

extern int prev_position;
extern int prev_history_sum;

// define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2

#define Debug_led 13


// Declare your constants or variables here if needed
// Declare your function prototypes
void lineFollow();


#endif
