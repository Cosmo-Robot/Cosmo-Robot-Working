volatile unsigned long int steps = 0;

#define NUMBER_OF_HOLES 20

void increment () {
  steps++;
}


void setup() {
  // put your setup code here, to run once:

  // pinMode(2, INPUT);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2), increment, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Rotations: ");
  Serial.print(steps / NUMBER_OF_HOLES);
  Serial.print("\t Steps: ");
  Serial.println(steps);
  delay(200);
}

