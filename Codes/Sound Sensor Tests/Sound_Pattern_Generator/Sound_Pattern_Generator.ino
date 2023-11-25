int x = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(10, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:

  x++;

  if (x < 10) {
    digitalWrite(13, HIGH);
    delay(1500);
    digitalWrite(13, LOW);
    delay(1500);
  }

  else if (x >= 10) {
    digitalWrite(13, HIGH);
    delay(4000);
    digitalWrite(13, LOW);
    delay(4000);
  }

  else if (x == 20) {
    x = 0;
  }
}
