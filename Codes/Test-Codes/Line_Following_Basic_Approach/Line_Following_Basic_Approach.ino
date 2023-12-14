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

int BWThresholds[ARRAY_LENGTH] = {380, 380, 380, 380, 380, 380, 380, 380};
bool IRArray[ARRAY_LENGTH] = {0, 0, 0, 0, 0, 0, 0, 0};       // This stores either black or white value of the IR sensors
// int IRArray[ARRAY_LENGTH] = {0};

int prevError = 0;
int integral = 0;

double Kp = 10;
double Ki = 0;
double Kd = 0;

double speed = 20;

// Keep the robot fully on the arena (only black or white colors),
// and calibrate to get the color details of the surface.
void calibrate() {
    short int values[8][50] = {{0}};
    long long int time = millis();
    short int i = 0;

    digitalWrite(BUZZER, HIGH);
    while (millis() - time <= 5000) {   // Calibraiting for 5 seconds
        for (short int j = 0; j < ARRAY_LENGTH; j++) {
            values[j][i] = analogRead(j);
        }
        i++;
        delay(100);
        // Serial.print(values[i - 1]);
        // Serial.print("\t");
    }
    digitalWrite(BUZZER, LOW);
    // Serial.println();
    
    
    for (short int j = 0; j < ARRAY_LENGTH; j++) {
        short int currentMin = 1023, currentMax = 0;
        for (short int k = 0; k < 50; k++) {
            if (values[j][k] < currentMin) {
                currentMin = values[j][k];
            }
            if (values[j][k] > currentMax) {
                currentMax = values[j][k];
            }
        }
        BWThresholds[j] = (currentMin + currentMax) / 2;        
    }

    // DEBUG CODE
    Serial.print("Thresholds: ");
    for (int p = 0; p < ARRAY_LENGTH; p++) {
        Serial.print(BWThresholds[p]);
        Serial.print("\t");
    }
    Serial.println();
    // DEBIG DONE.
}

void getIRData() {
    for (int i = 0; i < ARRAY_LENGTH; i++) {
        // Might need to change the sign > or <
        IRArray[i] = analogRead(i) > BWThresholds[i];   // true or false
        // IRArray[i] = analogRead(i);
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

void motorSpeed(double leftSpeed, double rightSpeed) {
    // Might need to swap ENA and ENB
    if (leftSpeed > 255) leftSpeed = 255;
    if (rightSpeed > 255) rightSpeed = 255;

    // 3.3V limit
    double limit = (255 / 5 * 3.3) - 1;
    leftSpeed = map(leftSpeed, 0, 255, 0, limit);
    rightSpeed = map(rightSpeed, 0, 255, 0, limit);

    analogWrite(ENA, leftSpeed);
    analogWrite(ENB, rightSpeed);
}

void setup() {
    Serial.begin(9600);
    
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
    if (digitalRead(CALIBRATE_BUTTON) == HIGH) {
        calibrate();
    }
    
    getIRData();
    int error = (-4) * IRArray[0] + (-2) * IRArray[1] + (-1) * IRArray[2] + (+1) * IRArray[5] + (+2) * IRArray[6] + (+4) * IRArray[7];
    int derivative = error - prevError;
    prevError = error;
    int integral = integral + error;

    double PID = Kp * error + Ki * integral + Kd * derivative;

    double leftSpeed = speed + PID;
    double rightSpeed = speed - PID;

    motorSpeed(leftSpeed, rightSpeed);

    // DEBUG
    for (int i = 0; i < ARRAY_LENGTH; i++) {
      Serial.print(IRArray[i]);
      Serial.print("\t");
    }
    Serial.println();
    delay(1000);
    // DEBUG DONE.
}