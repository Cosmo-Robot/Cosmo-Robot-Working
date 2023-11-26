#include <Arduino.h>

#define IN1 41
#define IN2 43
#define IN3 45
#define IN4 47

#define ENA 3
#define ENB 2

#define BUZZER 8

int bw_threshold = 0;

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
        Serial.print(values[i - 1]);
        Serial.print("\t");
    }
    digitalWrite(BUZZER, LOW);
    Serial.println();
    
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
    bw_threshold = (current_min + current_max) / 2;
    Serial.print("Current Threshold: ");
    Serial.println(bw_threshold);
}

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(A0, INPUT);     // Rightmost
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);     // Leftmost

    pinMode(BUZZER, OUTPUT);

    Serial.begin(9600);
    calibrate();
}

void loop() {

}