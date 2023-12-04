#include <Arduino.h>
#include <EEPROM.h>

// ========== LINE_COLOR DEFINITION ==========
const String LINE_COLOR = "BLACK";

// Define the size of the array and the EEPROM address to start writing data
const int arraySize = 8;     // Adjust this based on your array size
const int eepromAddress = 0; // EEPROM address where you want to start writing

#define Debug_led 13

// PID parameters for Line Following
double Kp = 15;
double Ki = 0;
double Kd = 10;

// PID parameters for Ultrasonic Wall Following
double UKp = 35;
double UKi = 0;
double Ukd = 20;

// Initialize PID variables
double prevError = 0;
double integral = 0;
double derivative = 0;

// recorrect
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

int TASK = 1; // 1 - Line Following, 2 - Wall Following

// IR_array 8 space empty array
int IR_array[8];

int Threshold = 100;

int prev_position = 0;

// define right motor enable pin is 3 and left 2
#define ENR 3
#define ENL 2

#define Motor_Right_Forward 22
#define Motor_Right_Backward 24
#define Motor_Left_Forward 26
#define Motor_Left_Backward 28

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

void motor(int motor, int direction);
void calibrate();
void buzzer_beep();
int ultrasonicDistance(int sensor);

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

    for (int i = 0; i < 8; i++)
    {
        byte temp = 0;
        // Ir_thresholds[i] = EEPROM.read(i)*4;
        EEPROM.get(i, temp);
        Ir_thresholds[i] = temp;
    }

    Serial.print("int IR_thresholds[] = {");
    for (int i = 0; i < 8; i++)
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

    // digitalWrite(Motor_Right_Forward,1);

    motor(RIGHT_MOTOR, FORWARD);
    motor(LEFT_MOTOR, FORWARD);

    pinMode(calibration, INPUT);

    sound_now_time = millis();

    pinMode(SENSOR_LEFT_ECHO_PIN, INPUT);
    pinMode(SENSOR_LEFT_TRIG_PIN, OUTPUT);
    pinMode(SENSOR_RIGHT_ECHO_PIN, INPUT);
    pinMode(SENSOR_RIGHT_TRIG_PIN, OUTPUT);
}

int temp = 0;

void loop()
{
    // ========== DETECTS SOUND LEVEL AND HALTS THE ROBOT ==========
    // if (analogRead(SOUND_SENSOR) >= SOUND_THRESHOLD)
    // {
    //     sound_now_time = millis();
    // }

    // if (millis() - sound_now_time < SOUND_WAIT_TIME)
    // {
    //     digitalWrite(SOUND_LED, HIGH);
    //     Switch = 0;
    // }
    // else
    // {
    //     digitalWrite(SOUND_LED, LOW);
    //     Switch = 1;
    // }
    // ========== SOUND SENSOR PART DONE ==========

    if (digitalRead(calibration) == 1)
    {
        calibrate();
    }

    int ultrasonicDistanceLeft = ultrasonicDistance(SENSOR_LEFT);
    int ultrasonicDistanceRight = ultrasonicDistance(SENSOR_RIGHT);

    if (ultrasonicDistanceLeft <= MAX_ULTRASONIC_DISTANCE || ultrasonicDistanceRight <= MAX_ULTRASONIC_DISTANCE)
    {
        // Change this to 1 to always enable Line Following Mode, temporaly disabling the Wall Following Mode.
        TASK = 2;

        // TODO
        // Before going to the Wall Following Mode, store the side where the obstacle is detected.
    }
    else
    {
        // Change this to 2 to always enable Wall Following Mode, temporaly disabling the Line Following Mode.
        TASK = 1;

        // TODO
        // Before changing back to Line Following Mode, depending on the previously stored side, activate "deviated" mode,
        // to get back to the line.
    }

    int Left_drive = 0;
    int Right_drive = 0;

    if (TASK == 1)
    {
        // Serial print from A0 to A7
        for (int i = 0; i < 8; i++)
        {
            // Serial.print(analogRead(i));
            // Serial.print(" ");

            // ========== CHANGE ONLY "LINE_COLOR" DEFINITION. ==========
            if (LINE_COLOR == "WHITE")
            {
                temp = analogRead(i) > Ir_thresholds[i];
            }
            else if (LINE_COLOR == "BLACK")
            {
                temp = analogRead(i) <= Ir_thresholds[i];
            }
            IR_array[7 - i] = temp;
        }

        motor(RIGHT_MOTOR, FORWARD);
        motor(LEFT_MOTOR, FORWARD);

        //  for (int i = 0; i < 8; i++){
        //    Serial.print(IR_array[i]);
        //    Serial.print(" ");
        //   }
        //   Serial.println(" ");

        int array_lit_amount = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3] + IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
        float left_sum = -8 * IR_array[0] - 5.5 * IR_array[1] - 2 * IR_array[2] - 1 * IR_array[3];
        float right_sum = 1 * IR_array[4] + 2 * IR_array[5] + 5.5 * IR_array[6] + 8 * IR_array[7];
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

        // Serial.println(position);
        //  lcd.setCursor(2,2);
        //  lcd.print("The IR weight: ");
        //  lcd.print(position);

        int Drive_constant = 200; // 100 working

        Left_drive = Drive_constant + PID_constant;
        Right_drive = Drive_constant - PID_constant;

        // Serial.println(mode);

        // Limiting to 0 - 255 range
        Left_drive = min(max(Left_drive, 0), 255);
        Right_drive = min(max(Right_drive, 0), 255);

        // end detection
        //  if (array_lit_amount == 8 or bend_done == true){
        //    Switch = 0;
        //  }

        if (array_lit_amount == 0 and mode != "bend")
        {
            Left_drive = 0;
            Right_drive = 0;
            analogWrite(ENL, Left_drive);
            analogWrite(ENR, Right_drive);
            digitalWrite(Debug_led, HIGH);
            delay(50);
            digitalWrite(Debug_led, LOW);
            for (int i = 0; i < 8; i++)
            {
                // Serial.print(analogRead(i));
                // Serial.print(" ");
                if (LINE_COLOR == "WHITE")
                {
                    temp = analogRead(i) > Ir_thresholds[i];
                }
                else if (LINE_COLOR == "BLACK")
                {
                    temp = analogRead(i) <= Ir_thresholds[i];
                }
                IR_array[7 - i] = temp;
            }

            array_lit_amount = IR_array[0] + IR_array[1] + IR_array[2] + IR_array[3] + IR_array[4] + IR_array[5] + IR_array[6] + IR_array[7];
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
            int prev_history_sum = 0;
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
            mode = "normal";
        }

        prev_position = position;
    }
    else if (TASK == 2)
    {
        int left_value = max(pow(2, MAX_ULTRASONIC_DISTANCE - ultrasonicDistanceLeft), 2048);
        int right_value = max(pow(2, MAX_ULTRASONIC_DISTANCE - ultrasonicDistanceRight), 2048);
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
    int sensor_max_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    // sensor min values array
    int sensor_min_values[8] = {0, 0, 0, 0, 0, 0, 0, 0};

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
        for (int i = 0; i < 8; i++)
        {
            if (sensor_max_values[i] != 0)
            {
                allZeros = false;
                break; // No need to continue checking if we find a non-zero value
            }
        }

        if (allZeros)
        {
            for (int i = 0; i < 8; i++)
            {
                sensor_max_values[i] = analogRead(i);
                sensor_min_values[i] = analogRead(i);
            }
        }

        for (int i = 0; i < 8; i++)
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
    for (int i = 0; i < 8; i++)
    {
        Serial.print(sensor_max_values[i]);
        Serial.print(", ");
    }
    Serial.print("};    ");
    Serial.print("int IR_thresholds[] = {");
    for (int i = 0; i < 8; i++)
    {
        Serial.print(sensor_min_values[i]);
        Serial.print(", ");
    }
    Serial.print("};    ");

    for (int i = 0; i < 8; i++)
    {
        Ir_thresholds[i] = (sensor_max_values[i] + sensor_min_values[i]) / 2;
    }

    Serial.print("int IR_thresholds[] = {");
    for (int i = 0; i < 8; i++)
    {
        Serial.print(Ir_thresholds[i]);
        Serial.print(", ");
    }
    Serial.println("};    ");

    // save the array to eeprom
    for (int i = 0; i < 8; i++)
    {
        // EEPROM.write(i, byte(Ir_thresholds[i]/4));
        EEPROM.put(i, byte(Ir_thresholds[i]));
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