#include <Arduino.h>

/*
Use this class to create a motor object separately for each and every motor.
*/

class Motor {
    private:
        const int INA, INB, ENA, ENCA, ENCB;
        // ENB - HIGH - Motor turns, LOW - Motor stops
        // ENCA - Triggers the interrupt
        // ENCB - Gives the encoder value (Use analogRead())
        volatile int stepCount; // Volatile because this variable might be changed externally.
        int motorState;    // 0 - Forward, 1 - Backward, 2 - At rest

        void readEncoder(){
            int val = digitalRead(ENCB);
            if(val > 0) {
                stepCount++;
            }
            else {
                stepCount--;
            }
        }

    public:
    Motor(int IN_A, int IN_B, int ENABLE, int ENC_A, int ENC_B) {
        INA = IN_A;
        INB = IN_B;
        ENA = ENABLE;
        ENCA = ENC_A;
        ENCB = ENC_B;

        attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
    }

    int getSteps() {
        return stepCount;
    }

    void goForward(int speed) { // speed: 0 - 255
        analogWrite(INA, speed);
        digitalWrite(INB, HIGH);
    }

    // It is assumed that in this direction, the step count increases.
    void goForwardSteps(int speed, int steps) {
        int startPos = getSteps();
        while (getSteps() - startPos < steps) {
            analogWrite(INA, speed);
            digitalWrite(INB, HIGH);
        }
    }

    void goBackward(int speed) { // speed: 0 - 255
        analogWrite(INA, speed);
        digitalWrite(INB, LOW);
    }

    // It is assumed that in this direction, the step count decreases.
    void goForwardSteps(int speed, int steps) {
        int startPos = getSteps();
        while (startPos - getSteps() < steps) {
            analogWrite(INA, speed);
            digitalWrite(INB, LOW);
        }
    }
};