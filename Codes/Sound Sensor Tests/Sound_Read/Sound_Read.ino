void setup() {
  // put your setup code here, to run once:
  // pinMode(A3, INPUT);
  pinMode(8, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println(analogRead(A3));
  Serial.println(digitalRead(8));
  delay(100);
}
