#include <Arduino.h>

// Define sensor pins
#define IR_SENSOR_1 2
#define IR_SENSOR_2 3
#define IR_SENSOR_3 4
#define IR_SENSOR_4 5
#define IR_SENSOR_5 6
#define IR_SENSOR_6 7
#define IR_SENSOR_7 8
#define IR_SENSOR_8 9

// Define motor pins
#define LEFT_MOTOR_ENA 10
#define LEFT_MOTOR_IN1 11
#define LEFT_MOTOR_IN2 12
#define RIGHT_MOTOR_ENB 5
#define RIGHT_MOTOR_IN3 6
#define RIGHT_MOTOR_IN4 7

// PID variables
double Kp = 1.0; // Proportional gain
double Ki = 0.0; // Integral gain
double Kd = 0.0; // Derivative gain
double lastError = 0;
double integral = 0;

void setup() {
  pinMode(LEFT_MOTOR_ENA, OUTPUT);
  pinMode(LEFT_MOTOR_IN1, OUTPUT);
  pinMode(LEFT_MOTOR_IN2, OUTPUT);
  pinMode(RIGHT_MOTOR_ENB, OUTPUT);
  pinMode(RIGHT_MOTOR_IN3, OUTPUT);
  pinMode(RIGHT_MOTOR_IN4, OUTPUT);

  pinMode(IR_SENSOR_1, INPUT);
  pinMode(IR_SENSOR_2, INPUT);
  pinMode(IR_SENSOR_3, INPUT);
  pinMode(IR_SENSOR_4, INPUT);
  pinMode(IR_SENSOR_5, INPUT);
  pinMode(IR_SENSOR_6, INPUT);
  pinMode(IR_SENSOR_7, INPUT);
  pinMode(IR_SENSOR_8, INPUT);
}

void loop() {
  // Read sensor values
  int sensor1 = digitalRead(IR_SENSOR_1);
  int sensor2 = digitalRead(IR_SENSOR_2);
  int sensor3 = digitalRead(IR_SENSOR_3);
  int sensor4 = digitalRead(IR_SENSOR_4);
  int sensor5 = digitalRead(IR_SENSOR_5);
  int sensor6 = digitalRead(IR_SENSOR_6);
  int sensor7 = digitalRead(IR_SENSOR_7);
  int sensor8 = digitalRead(IR_SENSOR_8);

  // Calculate error based on sensor readings
  int error = (sensor1 * 1 + sensor2 * 2 + sensor3 * 3 + sensor4 * 4 - sensor5 * 4 - sensor6 * 3 - sensor7 * 2 - sensor8 * 1);

  // Calculate PID control output
  double pidOutput = (Kp * error) + (Ki * integral) + (Kd * (error - lastError));

  // Update the motor speeds
  int leftSpeed = constrain(200 + pidOutput, 0, 255);
  int rightSpeed = constrain(200 - pidOutput, 0, 255);

  analogWrite(LEFT_MOTOR_ENA, leftSpeed);
  analogWrite(RIGHT_MOTOR_ENB, rightSpeed);

  lastError = error;
  integral += error;
}
