// lineFollow.h

#include <Arduino.h>

// Header guard to prevent multiple inclusions
#ifndef DETECTIONS_H
#define DETECTIONS_H

void readIRArray();
void motor(int motor, int direction);

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

// pin for right bend sensor
#define right_bend_sensor 9

// define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2



bool squareDetected();
bool TDetected();
bool CrossDetected();

#endif
