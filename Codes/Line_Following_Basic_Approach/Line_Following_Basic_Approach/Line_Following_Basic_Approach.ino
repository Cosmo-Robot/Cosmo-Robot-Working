#include <Arduino.h>

#define IN1 41
#define IN2 43
#define IN3 45
#define IN4 47

#define ENA 3
#define ENB 2

#define BUZZER 8

#define CALIBRATE_BUTTON 7

#define Motor_Right_Forward 41
#define Motor_Right_Backward 43
#define Motor_Left_Forward 45
#define Motor_Left_Backward 47

#define ARRAY_LENGTH 8  // Should be changed accordingly

int BWThreshold = 0;
bool IRArray[ARRAY_LENGTH] = {0};       // This stores either black or white value of the IR sensors

int prevError = 0;
int integral = 0;

double Kp = 7;
double Ki = 0;
double Kd = 10;

double speed = 10;

// Keep the robot fully on the arena (only black or white colors),
// and calibrate to get the color details of the surface.
void calibrate() {
    short int values[225] = {0};
    long long int time = millis();
    short int i = 0;

    digitalWrite(BUZZER, HIGH);
    while (millis() - time <= 5000) {   // Calibraiting for 5 seconds
        values[i++] = analogRead(A0);
        values[i++] = analogRead(A1);
        values[i++] = analogRead(A2);
        values[i++] = analogRead(A3);
        values[i++] = analogRead(A4);
        values[i++] = analogRead(A5);
        values[i++] = analogRead(A6);
        values[i++] = analogRead(A7);
        delay(200);
        // Serial.print(values[i - 1]);
        // Serial.print("\t");
    }
    digitalWrite(BUZZER, LOW);
    // Serial.println();
    
    short int current_min = 1023, current_max = 0;
    for (int j = 0; j < 200; j++) {
        if (values[j] < current_min) {
            current_min = values[j];
        }
        if (values[j] > current_max) {
            current_max = values[j];
        }
    }

    Serial.print("Min value: ");
    Serial.println(current_min);
    Serial.print("Max value: ");
    Serial.println(current_max);
    BWThreshold = (current_min + current_max) / 2;
    Serial.print("Current Threshold: ");
    Serial.println(BWThreshold);
}

void getIRData() {
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        // Might need to change the sign > or <
        IRArray[i] = analogRead(i) > BWThreshold;   // true or false
    }
}

// Motor direction control functions
void motorDirection(int motor, int direction){
  // Motor 1 - Right
  //    1 - Forward
  //    2 - Backward
  // Motor 2 - Left
  //    1 - Forward
  //    2 - Backward

  if (motor == 1){
    if (direction == 1){
      digitalWrite(Motor_Right_Forward,1);
      digitalWrite(Motor_Right_Backward,0);
    }else{
      digitalWrite(Motor_Right_Forward,0);
      digitalWrite(Motor_Right_Backward,1);
    }
  }else{
    if (direction == 1){
      digitalWrite(Motor_Left_Forward,1);
      digitalWrite(Motor_Left_Backward,0);
    }else{
      digitalWrite(Motor_Left_Forward,0);
      digitalWrite(Motor_Left_Backward,1);
    }
  }
}

void motorSpeed(double leftSpeed, double rigthSpeed) {
    // Might need to swap ENA and ENB
    analogWrite(ENA, leftSpeed);
    analogWrite(ENB, rigthSpeed);
}

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    // This part is redundant as no need to define A0 - A7 as INPUT.
    pinMode(A0, INPUT);     // Leftmost
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);     // Rightmost

    pinMode(BUZZER, OUTPUT);

    Serial.begin(9600);

    // Initially both motors rotate forward at the same speed.
    motorDirection(1, 1);
    motorDirection(2, 1);
    motorSpeed(speed, speed);
}

void loop() {
    if (digitalRead(CALIBRATE_BUTTON)) {
        calibrate();
    }
    
    getIRData();
    int error = (-3) * IRArray[0] + (-2) * IRArray[1] + (-1) * IRArray[2] + (+1) * IRArray[5] + (+2) * IRArray[6] + (+3) * IRArray[7];
    int derivative = error - prevError;
    prevError = error;
    int integral = integral + error;

    double PID = Kp * error + Ki * integral + Kd * derivative;

    double leftSpeed = speed + PID;
    double rightSpeed = speed - PID;

    motorSpeed(leftSpeed, rightSpeed);

}