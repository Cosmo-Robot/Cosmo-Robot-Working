#include <Arduino.h>
#include <EEPROM.h>


// Define the size of the array and the EEPROM address to start writing data
const int arraySize = 8; // Adjust this based on your array size
const int eepromAddress = 0; // EEPROM address where you want to start writing

// Define PID parameters
double Kp = 7;
double Ki = 0;
double Kd = 15;




// Initialize PID variables (Erorrs)
double prevError = 0;
double integral = 0;
double derivative = 0;


//recorrect (if there are no data in EEPROM)
int Ir_thresholds[] = {300, 300, 100, 100, 100, 105, 400, 400};
//IR_array 8 space empty array
int IR_array[8];


//define trig pin 6 and echo pin 7 (wall detection UltraSonic)
#define trigPin 6
#define echoPin 7
// defines variables for UltraSonic
long duration;
int distance;

//pushbutton pin input for calibration
#define calibration_push_button 7


//pin for buzzer to check state updates
#define buzzer_pin 8

//To stop entire robot
bool PID_Switch = 1;  // defualt on

//
int now_time = 0;

// default mode= normal. other modes junction (Bending modes), parking
String mode = "normal";

//define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2

// motor pins
#define Motor_Right_Forward 41   //If Right forward=1, then Left forward=0
#define Motor_Right_Backward 43
#define Motor_Left_Forward 45
#define Motor_Left_Backward 47

// x bit values for juctions bool Switch = 0; //0 for left ; 1 for right;
int x = 0;

//functions declarations here:
void motor(int motor, int direction);   //if forward direction =1 and backward=0
void caliberate();
void buzzer_beep();


int temp = 0;





////////////////////////Setup//////////////////////////////
void setup() {

  // Buzzer setup
  pinMode(buzzer_pin, OUTPUT);
  //pin 8 is buzzer, tur it on and off for 2 quick beeps
  buzzer_beep();
  delay(100);
  buzzer_beep();


  //pinmode trig and echo for UltraSonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Serial begin for serial monitor setup
  Serial.begin(9600);

  //import ir thresholds for each ir sensor in the array
  for (int i = 0; i < 8; i++) {
    Ir_thresholds[i] = EEPROM.read(i)*4;
  }

  //check the improted ir values in serial monitor
  Serial.print("int IR_thresholds[] = {");
  for (int i = 0; i < 8; i++){
    Serial.print(Ir_thresholds[i]);
    Serial.print(", ");}
    Serial.println("};    ");

// Motor contro pins as outputs
  pinMode(Motor_Right_Forward, OUTPUT);
  pinMode(Motor_Right_Backward, OUTPUT);
  pinMode(Motor_Left_Forward, OUTPUT);
  pinMode(Motor_Left_Backward, OUTPUT);


  // Both motors forward; digitalWrite(Motor_Right_Forward,1);  // motor(int motor, int direction);
  motor(1,1);
  motor(2,1);

  //Calibaration button as input
  pinMode(calibration_push_button, INPUT);
  
}


////////////////////////Loop///////////////////////////
void loop() {


  // if press the calibration button => caliberate
  if (digitalRead(calibration_push_button) == 1){
    caliberate();
  }


  //serial print from A0 to A7 for IR sensor array connected to atmega board
   for (int i = 0; i < 8; i++){
    if (analogRead(i) > Ir_thresholds[i]){
      temp = 0;}
    else{
      temp = 1 ;}
     IR_array[i] = temp; // updates IR_array with sensor input

   }

// PID control

   // how many sensors lit?
  int array_lit_amount = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3] + IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
  //Error ; distanse variation from the line
  int left_sum = -4*IR_array[0] -3*IR_array[1] -2*IR_array[2] -1*IR_array[3];
  int right_sum = 1*IR_array[4] +2*IR_array[5] +3*IR_array[6] +4*IR_array[7];
  int Error = left_sum + right_sum;

  derivative = Error - prevError; // For D
  integral = Error + integral; // For I

  int PID_signal = Kp * Error + Kd * derivative + Ki * integral ;

  prevError = Error;


int Drive_constant = 35;

  int Left_drive = Drive_constant + 3*PID_signal;
  int Right_drive = Drive_constant - 3*PID_signal;



// Caping the top limits
  if (Left_drive > 255){
    Left_drive = 255;
  }

  if (Left_drive < 0){
    Left_drive = 0;
  }

  if (Right_drive > 255){
    Right_drive = 255;
  }
  
  if (Right_drive < 0){
    Right_drive = 0;
  }



  //Parking mode will on for End detection or check point detection
  // if (array_lit_amount >= 6){
  //   mode = "parking";
  // }


  //Switching to Bend detection modes; 
  // if (mode == "normal"){
  //   //right bend detects when;
  //   if(right_sum >= 9 && left_sum > -4){
  //     //breaking (quick revverse)
  //     motor(1,0);
  //     motor(2,0);
  //     now_time = millis();
  //     mode = "bend_right";
  //   }
  //   //left bend detects when;
  //   if(right_sum < 4 && left_sum <= -9){
  //     //breaking
  //     motor(1,0);
  //     motor(2,0);
  //     now_time = millis();
  //     mode = "bend_left";
  //   }
  // }


  //Modes definitions are here:

  // 1] normal mode;
  // if (mode == "normal"){
  //   PID_Switch = 1;
  // }

  // 2] parking mode;
  // if (mode == "parking"){
  //   PID_Switch = 0;
  // }

  // 3] bend_right mode; 
  // if (mode == "bend_right"){
  //   // go forward a little bit, don't get a quick turn
  //   if (millis() - now_time < 800){
  //     motor(1,1);
  //     motor(2,1);   
  //     Left_drive = 40;
  //     Right_drive = 40;
  //   }
  //   // after millis become larger this happens in the next void loop run
  //   else{
  //     buzzer_beep();
  //     motor(1,0);
  //     motor(2,1);

  //     Left_drive = 50;
  //     Right_drive = 60;
  //     if (array_lit_amount >= 1){
  //       delay(50);
  //       array_lit_amount = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3] + IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
  //       if (array_lit_amount >= 1){

  //         Serial.print("bend over");
  //         mode = "normal";

  //         motor(1,1);
  //         motor(2,1);
  //       }
  //     }
  //   }
  // }

  // 4] bend_right mode; 
  // if (mode == "bend_left"){
  //   // go forward a little bit, don't get a quick turn
  //   if (millis() - now_time < 800){
  //     motor(1,1);
  //     motor(2,1);   
  //     Left_drive = 40;
  //     Right_drive = 40;
  //   }
  //   // after millis become larger this happens in the next void loop run
  //   else{
  //     buzzer_beep();
  //     motor(1,0);
  //     motor(2,1);

  //     Left_drive = 50;
  //     Right_drive = 60;
  //     if (array_lit_amount >= 1){
  //       delay(50);
  //       array_lit_amount = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3] + IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
  //       if (array_lit_amount >= 1){

  //         Serial.print("bend over");
  //         mode = "normal";

  //         motor(1,1);
  //         motor(2,1);
  //       }
  //     }
  //   }
  // }
 

 //PID_Switch definitions is here:
  if (PID_Switch == 1){
  analogWrite(ENL,Left_drive); // according to PID signal control= speed of the motor
  analogWrite(ENR,Right_drive);
  }else{
  analogWrite(ENL,0);
  analogWrite(ENR,0);
  }

}





//Function definitions here:

// motor function (only for direction control)
void motor(int motor, int direction){ // 1 for right ; 2 for left
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

// buzzer on off
void buzzer_beep(){
  digitalWrite(buzzer_pin,1);
  delay(100);
  digitalWrite(buzzer_pin,0);
}



void caliberate(){
  //make sensor_max_values array of length 8 equal to sensor calibration array
  int sensor_max_values[8] = {0,0,0,0,0,0,0,0}; //c++ list

  //sensor min values array
  int sensor_min_values[8] = {0,0,0,0,0,0,0,0};

  int now_time_for_calibration = millis();
  Serial.print("now time for calibration: ");
  Serial.print(now_time_for_calibration);

  while (millis() - now_time_for_calibration < 5000){   //Used whil becaus it blocks everything else not like if else, therefore now_time_for_calibration is a constant in whil loop
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




  //printing the max, min arrays
  Serial.print("int sensor_max_value[] = {");
  for (int i = 0; i < 8; i++){
    Serial.print(sensor_max_values[i]);
    Serial.print(", ");
  }
  Serial.print("};    ");
  Serial.print("int sensor_min_values[] = {");
  for (int i = 0; i < 8; i++){
    Serial.print(sensor_min_values[i]);
    Serial.print(", ");
  }
  Serial.print("};    ");

// creating the thresholds array
  for (int i = 0; i < 8; i++){
    Ir_thresholds[i] = (sensor_max_values[i] + sensor_min_values[i])/2;
  }
// pints thresholds
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