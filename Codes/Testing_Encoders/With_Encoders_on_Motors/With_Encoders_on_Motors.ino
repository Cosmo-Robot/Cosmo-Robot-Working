#include <Arduino.h>
#include <EEPROM.h>

// Define the size of the array and the EEPROM address to start writing data
const int arraySize = 8; // Adjust this based on your array size
const int eepromAddress = 0; // EEPROM address where you want to start writing


// Define PID parameters
double Kp = 0;
double Ki = 0;
double Kd = 0;

// Initialize PID variables
double prevError = 0;
double integral = 0;
double derivative = 0;

//recorrect
int Ir_thresholds[] = {300, 300, 100, 100, 100, 105, 400, 400};

int prev_error_history[] = {0,0,0,0,0,0,0,0,0,0};
int stack_pointer = 0;

int IR_history_count = 0;
int IR_history = 0;
int IR_first_grabbed = 0;

// Interrupt Pins for motors
#define LeftMotorInterrupt 18
#define RightMotorInterrupt 19

volatile unsigned long int stepsL = 0;
volatile unsigned long int stepsR = 0;

#define NUMBER_OF_HOLES 20

void incrementLeft () {
  stepsL++;
}

void incrementRight () {
  stepsR++;
}


//define trig pin 6 and echo pin 7
#define trigPin 6
#define echoPin 7

//pushbutton input for caliberation
#define caliberation 7

//pin for buzzer
#define buzzer_pin 8

bool bend_done = false;

//pin for right bend sensor
#define right_bend_sensor 9

bool swich = 1; 

bool bend_start = false;

bool bend_condition = false;

int now_time = 0;

String mode = "normal";

//IR_array 8 space empty array
int IR_array[8];

int Threshold = 100;

int prev_position = 0;

//define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2

#define Motor_Right_Forward 22
#define Motor_Right_Backward 24
#define Motor_Left_Forward 26
#define Motor_Left_Backward 28

// defines variables
long duration;
int distance;

// bool Switch = 0; //0 for left ; 1 for right;
int x = 0;

void motor(int motor, int direction);
void caliberate();
void buzzer_beep();

void setup() {
  attachInterrupt(digitalPinToInterrupt(LeftMotorInterrupt), incrementLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RightMotorInterrupt), incrementRight, RISING);

  pinMode(buzzer_pin, OUTPUT);

  //pin 8 is buzzer, tur it on and off for 2 quick beeps
  buzzer_beep();
  delay(100);
  buzzer_beep();
  
  //pinmode trig and echo
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Serial begin
  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    Ir_thresholds[i] = EEPROM.read(i)*4;
  }

  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++){
    Serial.print(Ir_thresholds[i]);
    Serial.print(", ");
  }
  Serial.println("};    ");


  pinMode(Motor_Right_Forward, OUTPUT);
  pinMode(Motor_Right_Backward, OUTPUT);
  pinMode(Motor_Left_Forward, OUTPUT);
  pinMode(Motor_Left_Backward, OUTPUT);

  pinMode(right_bend_sensor, INPUT);


  // digitalWrite(Motor_Right_Forward,1);

  motor(1,1);
  motor(2,1);

  pinMode(caliberation, INPUT);
  
}

int temp = 0;

void loop() {
  int distanceRequired = 80;
  
  while (1) {
    analogWrite(ENR, 255);
    motor(1, 1);
    analogWrite(ENL, 255);
    motor(2, 2);
    while (stepsR < distanceRequired) {
      analogWrite(ENR, 255);      
    }
    analogWrite(ENR, 255);
    motor(1, 2);
    delay(1000);
    stepsR = 0;
    
    analogWrite(ENR, 255);
    motor(1, 2);
    analogWrite(ENL, 255);
    motor(2, 1);
    while (stepsL < distanceRequired) {
      analogWrite(ENL, 255);      
    }
    analogWrite(ENL, 255);
    motor(2, 2);
    delay(1000);
    stepsL = 0;
  }

 if (digitalRead(caliberation) == 1){
    caliberate();
  }


  //serial print from A0 to A7
   for (int i = 0; i < 8; i++){
    // Serial.print(analogRead(i));
    // Serial.print(" ");
    if (analogRead(i) > Ir_thresholds[i]){
      temp = 1;}
    else{
      temp = 0;}
     IR_array[7-i] = temp;

   }

    motor(1,1);
    motor(2,1);



//  for (int i = 0; i < 8; i++){
//    Serial.print(IR_array[i]);
//    Serial.print(" ");
//   }
//   Serial.println(" ");

  int array_lit_amount = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3] + IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
  float left_sum = -3.7*IR_array[0] -2.9*IR_array[1] -2*IR_array[2] -1*IR_array[3];
  float right_sum = 1*IR_array[4] +2*IR_array[5] +2.9*IR_array[6] +3.7*IR_array[7];
  int position = left_sum + right_sum;

  prev_error_history[stack_pointer] = position;
  stack_pointer++;
  if (stack_pointer == 10){
    stack_pointer =0;
  }

   derivative = position - prevError;

  int PID_constant = position*Kp + Kd * derivative;

  prevError = position;

  //Serial.println(position);
  // lcd.setCursor(2,2);
  // lcd.print("The IR weight: ");
  // lcd.print(position);

  int Drive_constant = 150;

  int Left_drive = Drive_constant + PID_constant;
  int Right_drive = Drive_constant - PID_constant;

  Serial.println(mode);

  Left_drive = min(max(Left_drive, 0), 255);
  Right_drive = min(max(Right_drive, 0), 255);


  //end detection
  if (array_lit_amount == 8 and bend_done == true){
    swich = 0;
  }

   if (array_lit_amount == 0 and mode != "bend"){
    mode = "deviated";
    buzzer_beep();
    delay(20);
    buzzer_beep();
    delay(20);
    buzzer_beep();
    delay(20);
    buzzer_beep();
    delay(20);
    buzzer_beep();
    delay(20);
  }

  if (mode == "normal" and bend_done == false){
    //bend detection
    if(right_sum >= 8.5 && left_sum > -4){
      //breaking
      motor(1,0);
      motor(2,0);
      now_time = millis();
      mode = "bend";

      bend_done = true;

      bend_start = true;

    }
  }

  if (mode == "bend"){
    if(bend_start == true){
      bend_start = false;
      bend_condition = false;

      while (digitalRead(right_bend_sensor) == 0){
        digitalWrite(buzzer_pin,HIGH);
        Serial.print("in loop:  ");
        Serial.println(digitalRead(right_bend_sensor));
        motor(1,1);
        motor(2,1);
        Left_drive = 140;
        Right_drive = 140;
        analogWrite(ENL,Left_drive);
        analogWrite(ENR,Right_drive);
      }
      analogWrite(ENL,0);
      analogWrite(ENR,0);
      // delay(800);
      array_lit_amount = 0;
    }
    // analogWrite(ENL,0);
    // analogWrite(ENR,0);
    // delay(1000);


    digitalWrite(buzzer_pin,LOW);
    //buzzer_beep();

    Serial.println(array_lit_amount);

    motor(1,0);
    motor(2,1);

    Left_drive = 155;
    Right_drive = 155;
    Serial.println("in turning mode");

    //detecting if the line contacts the sensor
    if (bend_condition == false){
      if (array_lit_amount >= 2){

        bend_condition = true;
      }
    } else {
      //detecting if the line comes to the middle
      if (IR_array[3] == 1 or IR_array[4] == 1){

        Serial.print("bend over");
        mode = "normal";
        
        
        // motor(1,0);
        // motor(2,0);

        bend_condition = false;

        buzzer_beep();
        delay(100);
        buzzer_beep();

        analogWrite(ENL,0);
        analogWrite(ENR,0);
        delay(500);
      }
    }

  }

    //  // Deviation detection
  if (mode == "deviated"){
    int prev_history_sum = 0;
    for (int p = 0; p < 10; p++) {
      prev_history_sum += prev_error_history[p];
    }

    if(prev_history_sum > 0){
      motor(1,0);
      motor(2,1);
      Left_drive = 140;
      Right_drive = 140;
      analogWrite(ENL,Left_drive);
      analogWrite(ENR,Right_drive);
      delay(300);
  } else if(prev_history_sum < 0){
      motor(1,1);
      motor(2,0);
      Left_drive = 140;
      Right_drive = 140;
      analogWrite(ENL,Left_drive);
      analogWrite(ENR,Right_drive);
      delay(300);
  } else {
      motor(1,0);
      motor(2,0);
      Left_drive = 130;
      Right_drive = 130;
      analogWrite(ENL,Left_drive);
      analogWrite(ENR,Right_drive);
      delay(300);
  }
  mode = "normal";
  }

  if (swich == 1){
  analogWrite(ENL,Left_drive);
  analogWrite(ENR,Right_drive);
  }else{
  analogWrite(ENL,0);
  analogWrite(ENR,0);
  }

  prev_position = position;

}


void caliberate(){

  // Stop motors when caliberating
  analogWrite(ENL,0);
  analogWrite(ENR,0);

  //make sensor_max_values array of length 8 equal to sensor caliberation array
  int sensor_max_values[8] = {0,0,0,0,0,0,0,0};

  //sensor min values array
  int sensor_min_values[8] = {0,0,0,0,0,0,0,0};

  int now_time_for_caliberation = millis();
  Serial.print("now time for caliberation: ");
  Serial.print(now_time_for_caliberation);
  Serial.print("  millis: ");
  Serial.println(millis());

  while (millis() - now_time_for_caliberation < 5000){
    // Serial.println("Caliberating IR");
    digitalWrite(buzzer_pin,1);

    bool allZeros = true;
    for (int i = 0; i < 8; i++) {
      if (sensor_max_values[i] != 0) {
        allZeros = false;
        break; // No need to continue checking if we find a non-zero value
      }
    }

    if (allZeros){
      for (int i = 0; i < 8; i++){
        sensor_max_values[i] = analogRead(i);
        sensor_min_values[i] = analogRead(i);
      }
    }
  
    for (int i = 0; i < 8; i++){
      if (analogRead(i) > sensor_max_values[i]){
        sensor_max_values[i] = analogRead(i);
      }
      if (analogRead(i) < sensor_min_values[i]){
        sensor_min_values[i] = analogRead(i);
      }

    }
  }
  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++){
    Serial.print(sensor_max_values[i]);
    Serial.print(", ");
  }
  Serial.print("};    ");
  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++){
    Serial.print(sensor_min_values[i]);
    Serial.print(", ");
  }
  Serial.print("};    ");

  for (int i = 0; i < 8; i++){
    Ir_thresholds[i] = (sensor_max_values[i] + sensor_min_values[i])/2;
  }

  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++){
    Serial.print(Ir_thresholds[i]);
    Serial.print(", ");
  }
  Serial.println("};    ");

  //save the array to eeprom
  for (int i = 0; i < 8; i++) {
    EEPROM.write(i, byte(Ir_thresholds[i]/4));
  }

  delay(1000);

  digitalWrite(buzzer_pin,0);

}

void buzzer_beep(){
  digitalWrite(buzzer_pin,1);
  delay(100);
  digitalWrite(buzzer_pin,0);
}

void motor(int motor, int direction){ // 1 for right ; 2 for left
  if (motor == 1){
    if (direction == 1){
      digitalWrite(Motor_Right_Forward,1);
      digitalWrite(Motor_Right_Backward,0);
    }else{
      if (direction == 0) {
        digitalWrite(Motor_Right_Forward,0);
        digitalWrite(Motor_Right_Backward,1);        
      }
      else {
      digitalWrite(Motor_Right_Forward,1);
      digitalWrite(Motor_Right_Backward,1);
      }
    }
  }else{
    if (direction == 1){
      digitalWrite(Motor_Left_Forward,1);
      digitalWrite(Motor_Left_Backward,0);
    }else{
      if (direction == 0) {
        digitalWrite(Motor_Left_Forward,0);
        digitalWrite(Motor_Left_Backward,1);        
      }
      else {
      digitalWrite(Motor_Left_Forward,1);
      digitalWrite(Motor_Left_Backward,1);
      }
    }
  }
}