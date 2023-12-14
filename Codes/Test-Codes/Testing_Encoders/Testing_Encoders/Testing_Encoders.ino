volatile unsigned long int stepsL = 0;
volatile unsigned long int stepsR = 0;

#define LeftMotorInterrupt 18
#define RightMotorInterrupt 19
#define NUMBER_OF_HOLES 20

void incrementLeft () {
  stepsL++;
}

void incrementRight () {
  stepsR++;
}


void setup() {
  // put your setup code here, to run once:

  // pinMode(2, INPUT);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(LeftMotorInterrupt), incrementLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RightMotorInterrupt), incrementRight, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Rotations Left: ");
  Serial.print(stepsL / NUMBER_OF_HOLES);
  Serial.print("\t Steps Left: ");
  Serial.print(stepsL);
  
  Serial.print("\t\t\tRotations Right: ");
  Serial.print(stepsR / NUMBER_OF_HOLES);
  Serial.print("\t Steps Right: ");
  Serial.println(stepsR);
  // delay(200);
}

