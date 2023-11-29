// THIS IS FIRST AND SIMPLEST WORKING CODE FOR CALIBERATION AND BEND TAKING

#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

#include <EEPROM.h>

// Define the size of the array and the EEPROM address to start writing data
const int arraySize = 8; // Adjust this based on your array size
const int eepromAddress = 0; // EEPROM address where you want to start writing


// Define PID parameters
double Kp = 7;
double Ki = 1.5;
double Kd = 10;

// Initialize PID variables
double prevError = 0;
double integral = 0;
double derivative = 0;

//recorrect
int Ir_thresholds[] = {300, 300, 100, 100, 100, 105, 400, 400};


LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//define trig pin 6 and echo pin 7
#define trigPin 6
#define echoPin 7

//pushbutton input for caliberation
#define calibration 7

//pin for buzzer
#define buzzer_pin 8

bool swich = 1; 

int now_time = 0;

String mode = "normal";

//IR_array 8 space empty array
int IR_array[8];

int Threshold = 100;

bool PID_disable = false;
int Left_drive = 0, Right_drive = 0;
int turning_factor = 1.2;

//define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2

#define Motor_Right_Forward 41
#define Motor_Right_Backward 43
#define Motor_Left_Forward 45
#define Motor_Left_Backward 47

// defines variables
long duration;
int distance;

// bool Switch = 0; //0 for left ; 1 for right;
int x = 0;

void motor(int motor, int direction);
void calibrate();
void buzzer_beep();
void LCD_initial();

void setup() {

  pinMode(buzzer_pin, OUTPUT);

  //pin 8 is buzzer, tur it on and off for 2 quick beeps
  buzzer_beep();
  delay(100);
  buzzer_beep();
  

  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();

  LCD_initial();

  //pinmode trig and echo
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Serial begin
  Serial.begin(9600);

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


  // digitalWrite(Motor_Right_Forward,1);

  motor(1,1);
  motor(2,1);

  pinMode(calibration, INPUT);
  
}

int temp = 0;

void loop() {

  if (digitalRead(calibration) == 1){
    calibrate();
  }


  //serial print from A0 to A7
   for (int i = 0; i < 8; i++){
    // Serial.print(analogRead(i));
    // Serial.print(" ");
    if (analogRead(i) > Ir_thresholds[i]){
      temp = 1;}
    else{
      temp = 0;}
     IR_array[i] = temp;

   }

//  for (int i = 0; i < 8; i++){
//    Serial.print(IR_array[i]);
//    Serial.print(" ");
//   }
  // Serial.println(" ");

  int array_lit_amount = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3] + IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
  int left_sum = -(-4*IR_array[0] -3*IR_array[1] -2*IR_array[2] -1*IR_array[3]);
  int right_sum = -(1*IR_array[4] +2*IR_array[5] +3*IR_array[6] +4*IR_array[7]);
  int position = left_sum + right_sum;

   derivative = position - prevError;

  int PID_constant = position*Kp + Kd * derivative;

  prevError = position;

  
  //Serial.println(position);
  // lcd.setCursor(2,2);
  // lcd.print("The IR weight: ");
  // lcd.print(position);

  int Drive_constant = 40;

  if (!PID_disable) {
    Left_drive = Drive_constant + PID_constant;
    Right_drive = Drive_constant - PID_constant;
  }

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

  // lcd.setCursor(0,3);
  // lcd.print("Right: ");
  // lcd.print(Right_drive);
  // lcd.print(" Left: ");
  // lcd.print(Left_drive);

  //end detection
  if (array_lit_amount == 8){
    swich = 0;
  }

  int new_left_sum = 0, new_right_sum = 0;
  if (mode == "normal"){
    //bend detection
    new_left_sum = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3];
    new_right_sum = IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
    if(new_right_sum >= 3 && new_left_sum <= 1){  // _| - right bend detected
      //breaking
      motor(1,0);
      motor(2,0);
      now_time = millis();
      mode = "right_bend";
    }
    else if (new_right_sum <= 1 && new_left_sum >= 3) { // |_ - left bend detected
      //breaking
      motor(1,0);
      motor(2,0);
      now_time = millis();
      mode = "left_bend";
    }

    lcd.setCursor(3,3);
    lcd.print("Mode: ");
    lcd.setCursor(10,3);
    lcd.print(mode);

  }

  if (mode == "right_bend" || mode == "left_bend"){
    //  if(millis() - now_time < 200){
    //   Left_drive = 0;
    //   Right_drive = 0;
    // }else

    PID_disable = true;

    if (millis() - now_time < 800){ // This value can vary depending on the speed. If encoders are used, this approach will change.
      motor(1,1);
      motor(2,1);   
      Left_drive = 40;
      Right_drive = 40;
    }else{
      if (mode == "right_bend") {
        buzzer_beep();
        motor(1,0);
        motor(2,1);

        Left_drive = turning_factor * (Drive_constant + 10);
        Right_drive = turning_factor * Drive_constant;
      }
      else if (mode == "left_bend") {
        buzzer_beep();
        motor(1,1);
        motor(2,0);

        Left_drive =  turning_factor * Drive_constant;
        Right_drive = turning_factor * (Drive_constant + 10);
      }

      if (new_left_sum > 0 || new_right_sum > 0){   // Turning is complete.
        // Check again after a little while.
        delay(200);
        if (new_left_sum > 0 || new_right_sum > 0) {
          mode = "normal";
          PID_disable = false;

          Serial.print("bend over");
          lcd.setCursor(3,3);
          lcd.print("Mode: ");
          lcd.print(mode);

          motor(1,1);
          motor(2,1);
        }
      }
    }
  }

  
  if (swich == 1){
  analogWrite(ENL,Left_drive);
  analogWrite(ENR,Right_drive);
  }else{
  analogWrite(ENL,0);
  analogWrite(ENR,0);
  }
  
  // analogWrite(ENL,200);
  // analogWrite(ENR,0);

  

  

  //Serial.println(IR_array[2]);
  //Serial.println(IR_array[5]);
  
  // //read the distance from ultrasonic sensor
  // // Clears the trigPin
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);

  // // Sets the trigPin on HIGH state for 10 micro seconds
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);

  // // Reads the echoPin, returns the sound wave travel time in microseconds
  // duration = pulseIn(echoPin, HIGH);    
  
  // // Calculating the distance
  // distance= duration*0.034/2;

  // // Prints the distance on the Serial Monitor
  // //Serial.print("Distance: ");
  // //Serial.println(distance);
  
  // if (distance < 8){
  //   analogWrite(ENL,255);
  //   analogWrite(ENR,255);
  //   motor(1,0);
  //   motor(2,0);
  //   delay(100);
  //   analogWrite(ENL,0);
  //   analogWrite(ENR,0);
  // } else {
  //   analogWrite(ENL,00);
  //   analogWrite(ENR,00);
  //   motor(1,1);
  //   motor(2,1);
  // }

}


void calibrate(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Caliberating IR");
  lcd.setCursor(0,1);
  lcd.print("Please wait");

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

  LCD_initial();
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

void LCD_initial(){

  lcd.setCursor(2,0);
  lcd.print("Cosmo: Testing!");
  lcd.setCursor(2,1);

  lcd.setCursor(1,1);
  lcd.print("Kp: ");
  lcd.print(int(Kp));

  lcd.print(" Kd: ");
  lcd.print(int(Kd));

  lcd.print(" Ki: ");
  lcd.print(int(Ki));
  
  lcd.setCursor(3,3);
  lcd.print("Mode: ");
  lcd.print(mode);


}