#include <Arduino.h>
#include <EEPROM.h>

// ========== LINE_COLOR DEFINITION ==========
const String LINE_COLOR = "WHITE";

// Define the size of the array and the EEPROM address to start writing data
const int arraySize = 8;     // Adjust this based on your array size
const int eepromAddress = 0; // EEPROM address where you want to start writing

#define Debug_led 13

// PID parameters for Line Following
double Kp = 5;
double Ki = 0;
double Kd = 7;

// PID parameters for Ultrasonic Wall Following
double UKp = 35;
double UKi = 0;
double Ukd = 20;

int Drive_constant = 200;

// Initialize PID variables
double prevError = 0;
double integral = 0;
double derivative = 0;

int Ir_thresholds[] = {300, 300, 100, 100, 100, 105, 400, 400};

int prev_error_history[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int stack_pointer = 0;

int IR_history_count = 0;
int IR_history = 0;
int IR_first_grabbed = 0;

// define trig pin 6 and echo pin 7
#define trigPin 6
#define echoPin 7

// pushbutton input for calibration
#define calibration 7

// pin for buzzer
#define buzzer_pin 8

bool bend_done = false;

// pin for right bend sensor
#define right_bend_sensor 9

bool Switch = 1;

bool bend_start = false;

bool bend_condition = false;

int now_time = 0;

String mode = "normal";
String wall_side = "";

enum Status {LINE = 1, WALL, RAMP, PULL, COLOR, SOUND, GUARD, HALT};
// If initial square is also counted add something first and assign it to TASK.
Status TASK = LINE; // 1 - Line Following, 2 - Wall Following, etc.

// int TASK = 1; // 1 - Line Following, 2 - Wall Following, etc.

// IR_array 8 space empty array
#define IR_ARRAY_LENGTH 12 
int IR_array[IR_ARRAY_LENGTH];
int calibrate_array_size = IR_ARRAY_LENGTH - 4;

int Threshold = 100;

int prev_position = 0;
int prev_history_sum = 0;

// define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2

#define Motor_Right_Forward 28
#define Motor_Right_Backward 26
#define Motor_Left_Forward 22
#define Motor_Left_Backward 24

// Motor and Direction Definitions
#define RIGHT_MOTOR 1
#define LEFT_MOTOR 2
#define FORWARD 1
#define BACKWARD 2

// Sound sensor
#define SOUND_SENSOR A10
#define SOUND_THRESHOLD 300
#define SOUND_WAIT_TIME 500 // milliseconds
#define SOUND_LED 13
unsigned long int sound_now_time;

// bool Switch = 0; //0 for left ; 1 for right;
int x = 0;

// Ultrasonic sensors
#define SENSOR_LEFT 1
#define SENSOR_RIGHT 2

#define SENSOR_LEFT_TRIG_PIN 32
#define SENSOR_LEFT_ECHO_PIN 34

#define SENSOR_RIGHT_TRIG_PIN 36
#define SENSOR_RIGHT_ECHO_PIN 38

float ultrasonic_prev_error = 0;

#define MAX_ULTRASONIC_DISTANCE 30 // in centimeters

// Task selection switch
#define TASK_SWITCH 44
#define SWITCH_MSB 45
#define SWITCH_MID_SB 46
#define SWITCH_LSB 47

void readIRArray();
void motor(int motor, int direction);
void calibrate();
void buzzer_beep();
int ultrasonicDistance(int sensor);
Status getTaskFromSwitches();
bool squareDetected();

void lineFollow();
void wallFollow();

void setup()
{
    pinMode(buzzer_pin, OUTPUT);

    // pin 8 is buzzer, tur it on and off for 2 quick beeps
    buzzer_beep();
    delay(100);
    buzzer_beep();

    // pinmode trig and echo
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Serial begin
    Serial.begin(115200);

    for (int i = 0; i < calibrate_array_size; i++)
    {
        // Ir_thresholds[i] = EEPROM.read(i)*4;
        EEPROM.get(i * sizeof(int), Ir_thresholds[i]);
    }

    Serial.print("int IR_thresholds[] = {");
    for (int i = 0; i < calibrate_array_size; i++)
    {
        Serial.print(Ir_thresholds[i]);
        Serial.print(", ");
    }
    Serial.println("};    ");

    pinMode(Motor_Right_Forward, OUTPUT);
    pinMode(Motor_Right_Backward, OUTPUT);
    pinMode(Motor_Left_Forward, OUTPUT);
    pinMode(Motor_Left_Backward, OUTPUT);

    pinMode(right_bend_sensor, INPUT);

    motor(RIGHT_MOTOR, FORWARD);
    motor(LEFT_MOTOR, FORWARD);

    pinMode(calibration, INPUT);

    sound_now_time = millis();

    pinMode(SENSOR_LEFT_ECHO_PIN, INPUT);
    pinMode(SENSOR_LEFT_TRIG_PIN, OUTPUT);
    pinMode(SENSOR_RIGHT_ECHO_PIN, INPUT);
    pinMode(SENSOR_RIGHT_TRIG_PIN, OUTPUT);

    pinMode(TASK_SWITCH, INPUT);
    pinMode(SWITCH_MSB, INPUT);
    pinMode(SWITCH_MID_SB, INPUT);
    pinMode(SWITCH_LSB, INPUT);
}

int temp = 0;
int Left_drive = 0;
int Right_drive = 0;

void loop()
{

    Serial.println(Switch);
    // while(1) {
    //     analogWrite(ENL, 230);
    //     analogWrite(ENR, 170);
    //     motor(LEFT_MOTOR, FORWARD);
    //     motor(RIGHT_MOTOR, FORWARD);
    // }

    if (digitalRead(calibration) == 1)
    {
        calibrate();
    }

    
    // Gets the task to be started, using switches.
    // Maybe we can use an INTERRUPT.
    // if (digitalRead(TASK_SWITCH) == HIGH) {
    //     TASK = getTaskFromSwitches();

    //     // Buzzer beep count can be used to get the task.
    //     for (int i = 0; i < TASK; i++) {
    //         buzzer_beep();
    //     }
    // }
  
    // Automatically increment task after detecting a white square.
    if (squareDetected()) {
        // TASK = Status(TASK + 1); // This is correct for the final competition.
        TASK = HALT;
        Serial.println("HALTED");
    }
    

    if (TASK == LINE)
    {
        lineFollow();
    }
    else if (TASK == WALL)
    {
        int ultrasonicDistanceLeft = ultrasonicDistance(SENSOR_LEFT);
        int ultrasonicDistanceRight = ultrasonicDistance(SENSOR_RIGHT);

        if (ultrasonicDistanceLeft <= MAX_ULTRASONIC_DISTANCE || ultrasonicDistanceRight <= MAX_ULTRASONIC_DISTANCE)
        {
            if (ultrasonicDistanceLeft <= MAX_ULTRASONIC_DISTANCE) {
                wall_side = "LEFT";
            }
            else if (ultrasonicDistanceRight <= MAX_ULTRASONIC_DISTANCE) {
                wall_side = "RIGHT";
            }
            wallFollow();
        }
        else
        {
            // Maybe we need to go a little bit forward before going to the "deviated" mode.
            if (wall_side == "LEFT") {
                mode = "deviated";
                prev_history_sum = -1000;
            }
            else if (wall_side == "RIGHT") {
                mode = "deviated";
                prev_history_sum = 1000;
            }
            lineFollow();

            // TODO
            // Before changing back to Line Following Mode, depending on the previously stored side, activate "deviated" mode,
            // to get back to the line.
        }
    }
    else if (TASK == RAMP) {
        // Mostly correct
        lineFollow();
    }
    else if (TASK == PULL) {
        
    }
    else if (TASK == COLOR) {
        
    } 
    else if (TASK == SOUND) {
        if (analogRead(SOUND_SENSOR) >= SOUND_THRESHOLD)
        {
            sound_now_time = millis();
        }

        if (millis() - sound_now_time < SOUND_WAIT_TIME)
        {
            digitalWrite(SOUND_LED, HIGH);
            Switch = 0;
        }
        else
        {
            digitalWrite(SOUND_LED, LOW);
            lineFollow();
        }
        
    }
    else if (TASK == GUARD) {
        
    }
    else if (TASK == HALT) {
        Switch = 0;
    }


    if (Switch == 1)
    {
        analogWrite(ENL, Left_drive);
        analogWrite(ENR, Right_drive);
    }
    else
    {
        analogWrite(ENL, 0);
        analogWrite(ENR, 0);
    }
}

void readIRArray() {
    for (int i = 0; i < IR_ARRAY_LENGTH - 4; i++)
    {
        // Serial.print(analogRead(i));
        // Serial.print(" ");
        if (LINE_COLOR == "WHITE")
        {
            temp = analogRead(i) <= Ir_thresholds[i];
        }
        else if (LINE_COLOR == "BLACK")
        {
            temp = analogRead(i) > Ir_thresholds[i];
        }
        // IR_array[IR_ARRAY_LENGTH - 1 - i] = temp;

        // IR_array[i] = temp; // This is correct if pins are connected in order.
        IR_array[i + 2] = temp;

        // This part is needed because the pins are connected not in order.
        // if (i < 8) {
        //     IR_array[i + 2] = temp;
        // }
        // else {
        //     if (i == 8) IR_array[0] = temp;
        //     else if (i == 9) IR_array[1] = temp;
        //     else if (i == 10) IR_array[10] = temp;
        //     else if (i == 11) IR_array[11] = temp;
        // }
    }

    IR_array[0] = LINE_COLOR == "BLACK" ? digitalRead(A11) : 1 - digitalRead(A11);
    IR_array[1] = LINE_COLOR == "BLACK" ? digitalRead(A10) : 1 - digitalRead(A10);
    IR_array[10] = LINE_COLOR == "BLACK" ? digitalRead(A9) : 1 - digitalRead(A9);
    IR_array[11] = LINE_COLOR == "BLACK" ? digitalRead(A8) : 1 - digitalRead(A8);

    
    for (int i = 0; i < IR_ARRAY_LENGTH; i++)
    {
        Serial.print(IR_array[i]);
        Serial.print(" ");
    }
    Serial.println();
}

int ultrasonicDistance(int sensor)
{
    int distance = 0;
    if (sensor == SENSOR_LEFT)
    {
        digitalWrite(SENSOR_LEFT_TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(SENSOR_LEFT_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(SENSOR_LEFT_TRIG_PIN, LOW);
        distance = pulseIn(SENSOR_LEFT_ECHO_PIN, HIGH) / 2 * 0.034;
    }
    else if (sensor == SENSOR_RIGHT)
    {
        digitalWrite(SENSOR_RIGHT_TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(SENSOR_RIGHT_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(SENSOR_RIGHT_TRIG_PIN, LOW);
        distance = pulseIn(SENSOR_RIGHT_ECHO_PIN, HIGH) / 2 * 0.034;
    }
    return distance;
}

void calibrate()
{
    motor(RIGHT_MOTOR, BACKWARD);
    motor(LEFT_MOTOR, FORWARD);

    // Stop motors when calibrating
    analogWrite(ENL, 255);
    analogWrite(ENR, 255);

    // make sensor_max_values array of length 8 equal to sensor calibration array
    int sensor_max_values[calibrate_array_size] = {0};

    // sensor min values array
    int sensor_min_values[calibrate_array_size] = {0};

    int now_time_for_calibration = millis();
    Serial.print("now time for calibration: ");
    Serial.print(now_time_for_calibration);
    Serial.print("  millis: ");
    Serial.println(millis());

    while (millis() - now_time_for_calibration < 5000)
    {
        // Serial.println("Caliberating IR");
        digitalWrite(buzzer_pin, 1);

        bool allZeros = true;
        for (int i = 0; i < calibrate_array_size; i++)
        {
            if (sensor_max_values[i] != 0)
            {
                allZeros = false;
                break; // No need to continue checking if we find a non-zero value
            }
        }

        if (allZeros)
        {
            for (int i = 0; i < calibrate_array_size; i++)
            {
                sensor_max_values[i] = analogRead(i);
                sensor_min_values[i] = analogRead(i);
            }
        }

        for (int i = 0; i < calibrate_array_size; i++)
        {
            if (analogRead(i) > sensor_max_values[i])
            {
                sensor_max_values[i] = analogRead(i);
            }
            if (analogRead(i) < sensor_min_values[i])
            {
                sensor_min_values[i] = analogRead(i);
            }
        }
    }
    Serial.print("int IR_thresholds[] = {");
    for (int i = 0; i < calibrate_array_size; i++)
    {
        Serial.print(sensor_max_values[i]);
        Serial.print(", ");
    }
    Serial.print("};    ");
    Serial.print("int IR_thresholds[] = {");
    for (int i = 0; i < calibrate_array_size; i++)
    {
        Serial.print(sensor_min_values[i]);
        Serial.print(", ");
    }
    Serial.print("};    ");

    for (int i = 0; i < calibrate_array_size; i++)
    {
        Ir_thresholds[i] = (sensor_max_values[i] + sensor_min_values[i]) / 2;
    }

    Serial.print("int IR_thresholds[] = {");
    for (int i = 0; i < calibrate_array_size; i++)
    {
        Serial.print(Ir_thresholds[i]);
        Serial.print(", ");
    }
    Serial.println("};    ");

    // save the array to eeprom
    for (int i = 0; i < calibrate_array_size; i++)
    {
        // EEPROM.write(i, byte(Ir_thresholds[i]/4));
        EEPROM.put(i * sizeof(int), Ir_thresholds[i]);
    }

    delay(1000);

    digitalWrite(buzzer_pin, 0);

    analogWrite(ENL, 0);
    analogWrite(ENR, 0);

    delay(4000);
}

void buzzer_beep()
{
    digitalWrite(buzzer_pin, 1);
    delay(100);
    digitalWrite(buzzer_pin, 0);
}

void motor(int motor, int direction)
{
    if (motor == RIGHT_MOTOR)
    {
        if (direction == FORWARD)
        {
            digitalWrite(Motor_Right_Forward, HIGH);
            digitalWrite(Motor_Right_Backward, LOW);
        }
        else if (direction == BACKWARD)
        {
            digitalWrite(Motor_Right_Forward, LOW);
            digitalWrite(Motor_Right_Backward, HIGH);
        }
    }
    else if (motor == LEFT_MOTOR)
    {
        if (direction == 1)
        {
            digitalWrite(Motor_Left_Forward, HIGH);
            digitalWrite(Motor_Left_Backward, LOW);
        }
        else if (direction == BACKWARD)
        {
            digitalWrite(Motor_Left_Forward, LOW);
            digitalWrite(Motor_Left_Backward, HIGH);
        }
    }
}

Status getTaskFromSwitches() {
    bool msb = digitalRead(SWITCH_MSB);
    bool midsb = digitalRead(SWITCH_MID_SB);
    bool lsb = digitalRead(SWITCH_LSB);

    int level = msb * 4 + midsb * 2 + lsb * 1;

    switch (level){
        case 1: return LINE;
        case 2: return WALL;
        case 3: return RAMP;
        case 4: return PULL;
        case 5: return COLOR;
        case 6: return SOUND;
        case 7: return GUARD;
        default: return HALT;
    }  
}

bool squareDetected() {
    readIRArray();
    int array_lit_amount = 0;
    for (int i = 0; i < IR_ARRAY_LENGTH; i++) {
        array_lit_amount += IR_array[i];
    }

    if (array_lit_amount == IR_ARRAY_LENGTH) {   // Suspect this as a detection of white all long the array.
    // This can either be a T junction or a white square.

        Serial.println("we arrived the square test 1");

        // Go some distance forward and check again.
        motor(LEFT_MOTOR, FORWARD);
        analogWrite(ENL, Drive_constant);
        motor(RIGHT_MOTOR, FORWARD);
        analogWrite(ENR, Drive_constant);

        delay(1500); // Change accordingly.
        
        analogWrite(ENL, 0);
        analogWrite(ENR, 0);

        readIRArray();
        array_lit_amount = 0;
        for (int i = 0; i < IR_ARRAY_LENGTH; i++) {
            array_lit_amount += IR_array[i];
        }

        if (array_lit_amount == IR_ARRAY_LENGTH) {   // 2 here is a safety factor.
            // Should turn and align with a normal line before returning.
            Serial.println("we arrived the square test 2");
            return true;
           
        }
        else {
            // Coming back a little
            motor(LEFT_MOTOR, BACKWARD);
            analogWrite(ENL, Drive_constant);
            motor(RIGHT_MOTOR, BACKWARD);
            analogWrite(ENR, Drive_constant);

            delay(600); // Change accordingly.
            
            analogWrite(ENL, 0);
            analogWrite(ENR, 0);
            return false;
        }
    }
    else {
        return false;
    }
}

void lineFollow() {
    readIRArray();

    motor(RIGHT_MOTOR, FORWARD);
    motor(LEFT_MOTOR, FORWARD);

    int array_lit_amount = 0;
    for (int i = 0; i < IR_ARRAY_LENGTH; i++) {
        array_lit_amount += IR_array[i];
    }

    // The following two lines should change with respect to IR array length.

    // For 8 IR array.
    // float left_sum = -10 * IR_array[0] - 7 * IR_array[1] - 2 * IR_array[2] - 1 * IR_array[3];
    // float right_sum = 1 * IR_array[4] + 2 * IR_array[5] + 7 * IR_array[6] + 10 * IR_array[7];

    // For 12 extended IR array.
    float left_sum = -20 * IR_array[0] - 14 * IR_array[1] - 10 * IR_array[2] - 7 * IR_array[3] - 2 * IR_array[4] - 1 * IR_array[5];
    float right_sum = 1 * IR_array[6] + 2 * IR_array[7] + 7 * IR_array[8] + 10 * IR_array[9] + 14 * IR_array[10] + 20 * IR_array[11];

    int position = left_sum + right_sum;

    prev_error_history[stack_pointer] = position;
    stack_pointer++;
    if (stack_pointer == 10)
    {
        stack_pointer = 0;
    }

    derivative = position - prevError;

    int PID_constant = Kp * position + Kd * derivative;

    prevError = position;

    Drive_constant = 200;

    int offset = 30;    // For correcting motor speeds
    Left_drive = Drive_constant + offset + PID_constant;
    Right_drive = Drive_constant - offset - PID_constant;

    // Limiting to 0 - 255 range
    Left_drive = min(max(Left_drive, 0), 255);
    Right_drive = min(max(Right_drive, 0), 255);

    // if ((array_lit_amount == 0 or (IR_array[0] == 0 and IR_array[1] == 0 and IR_array[2] == 1 and IR_array[3] == 0 and IR_array[4] == 0 and IR_array[5] == 0 and IR_array[6] == 0 and IR_array[7] == 0))and mode != "bend")
    if (array_lit_amount == 0 and mode != "bend")
    {
        Left_drive = 0;
        Right_drive = 0;
        analogWrite(ENL, Left_drive);
        analogWrite(ENR, Right_drive);
        digitalWrite(Debug_led, HIGH);
        delay(50);
        digitalWrite(Debug_led, LOW);

        readIRArray();

        for (int i = 0; i < IR_ARRAY_LENGTH; i++) {
            array_lit_amount += IR_array[i];
        }

        if (array_lit_amount == 0 and mode != "bend")
        {
            mode = "deviated";
            // buzzer_beep();
            // delay(20);
            // buzzer_beep();
            // delay(20);
            // buzzer_beep();
            // delay(20);
            // buzzer_beep();
            // delay(20);
            // buzzer_beep();
            // delay(20);
        }
    }

    // if (mode == "normal" and bend_done == false){
    //   //bend detection
    //   if(right_sum >= 8.5 && left_sum > -4){
    //     //breaking
    //     motor(RIGHT_MOTOR, BACKWARD);
    //     motor(LEFT_MOTOR, BACKWARD);
    //     now_time = millis();
    //     mode = "bend";

    //     bend_done = true;

    //     bend_start = true;

    //   }
    // }

    // Parking
    // if (array_lit_amount == 12){
    //     Switch = 0;
    // }

    if (mode == "bend")
    {
        if (bend_start == true)
        {
            bend_start = false;
            bend_condition = false;

            while (digitalRead(right_bend_sensor) == 1)
            {
                digitalWrite(buzzer_pin, HIGH);
                Serial.print("in loop:  ");
                Serial.println(digitalRead(right_bend_sensor));
                motor(RIGHT_MOTOR, FORWARD);
                motor(LEFT_MOTOR, FORWARD);
                Left_drive = 140;
                Right_drive = 140;
                analogWrite(ENL, Left_drive);
                analogWrite(ENR, Right_drive);
            }
            analogWrite(ENL, 0);
            analogWrite(ENR, 0);
            // delay(800);
            array_lit_amount = 0;
        }
        // analogWrite(ENL,0);
        // analogWrite(ENR,0);
        // delay(1000);

        digitalWrite(buzzer_pin, LOW);
        // buzzer_beep();

        Serial.println(array_lit_amount);

        motor(RIGHT_MOTOR, BACKWARD);
        motor(LEFT_MOTOR, FORWARD);

        Left_drive = 155;
        Right_drive = 155;
        Serial.println("in turning mode");

        // detecting if the line contacts the sensor
        if (bend_condition == false)
        {
            if (array_lit_amount >= 2)
            {

                bend_condition = true;
            }
        }
        else
        {
            // detecting if the line comes to the middle
            if (IR_array[3] == 1 or IR_array[4] == 1)
            {

                Serial.print("bend over");
                mode = "normal";

                // motor(RIGHT_MOTOR, BACKWARD);
                // motor(LEFT_MOTOR, BACKWARD);

                bend_condition = false;

                buzzer_beep();
                delay(100);
                buzzer_beep();

                analogWrite(ENL, 0);
                analogWrite(ENR, 0);
                delay(500);
            }
        }
    }

    //  // Deviation detection
    if (mode == "deviated")
    {
        for (int p = 0; p < 10; p++)
        {
            prev_history_sum += prev_error_history[p];
        }

        if (prev_history_sum > 0)
        {
            motor(RIGHT_MOTOR, BACKWARD);
            motor(LEFT_MOTOR, FORWARD);
            Left_drive = 230;
            Right_drive = 230;
            analogWrite(ENL, Left_drive);
            analogWrite(ENR, Right_drive);
            delay(300);
        }
        else if (prev_history_sum < 0)
        {
            motor(RIGHT_MOTOR, FORWARD);
            motor(LEFT_MOTOR, BACKWARD);
            Left_drive = 230;
            Right_drive = 230;
            analogWrite(ENL, Left_drive);
            analogWrite(ENR, Right_drive);
            delay(300);
        }
        else
        {
            motor(RIGHT_MOTOR, BACKWARD);
            motor(LEFT_MOTOR, BACKWARD);
            Left_drive = 200;
            Right_drive = 200;
            analogWrite(ENL, Left_drive);
            analogWrite(ENR, Right_drive);
            delay(300);
        }
        prev_history_sum = 0;
        mode = "normal";
    }

    prev_position = position;
}

void wallFollow() {
    int left_value = max(pow(2, MAX_ULTRASONIC_DISTANCE - ultrasonicDistance(SENSOR_LEFT)), 2048);
    int right_value = max(pow(2, MAX_ULTRASONIC_DISTANCE - ultrasonicDistance(SENSOR_RIGHT)), 2048);
    int error = right_value - left_value;
    // int error = left_value - right_value;

    int error_derivative = error - ultrasonic_prev_error;

    int PID_constant = UKp * error + Ukd * error_derivative;

    ultrasonic_prev_error = error;

    int Drive_constant = 180; // 100 working

    Left_drive = Drive_constant + PID_constant;
    Right_drive = Drive_constant - PID_constant;

    // if (Left_drive < 0) {
    //     motor(LEFT_MOTOR, BACKWARD);
    //     Left_drive = min(-100, Left_drive);
    // }
    // else {
    //     motor(LEFT_MOTOR, FORWARD);
    // }
    // if (Right_drive < 0) {
    //     motor(RIGHT_MOTOR, BACKWARD);
    //     Right_drive = min(-100, Right_drive);
    // }
    // else {
    //     motor(RIGHT_MOTOR, FORWARD);
    // }

    // Limiting to 0 - 255 range
    Left_drive = min(max(Left_drive, 0), 255);
    Right_drive = min(max(Right_drive, 0), 255);
    // Left_drive = min(abs(Left_drive), 255);
    // Right_drive = min(abs(Right_drive), 255);

    // Serial.print("\nDistanceLeft: ");
    // Serial.print(ultrasonicDistanceLeft);
    // Serial.print("\tDistanceRight: ");
    // Serial.print(ultrasonicDistanceRight);
}