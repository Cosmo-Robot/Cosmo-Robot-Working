#define analogReadPin A0
#define digitalReadPin 8
#define LED 13

#define THRESHOLD 300
#define ON_TIME 500

unsigned long int now_time = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(analogReadPin, INPUT);
  pinMode(digitalReadPin, INPUT);
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  now_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(analogReadPin);
  Serial.println(value);
  // Serial.print("\t");
  // Serial.println(digitalRead(digitalReadPin));
  // delay(300);
  if (value > THRESHOLD) {
    now_time = millis();
  }
  if (millis() - now_time <= ON_TIME) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
}
