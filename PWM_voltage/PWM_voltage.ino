const int transistorPin = 9;
int dutyCycle = 20;
int period = 1000;
void setup() {
  // put your setup code here, to run once:
  pinMode(transistorPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(transistorPin, HIGH);
  delay(dutyCycle * period / 100);
  digitalWrite(transistorPin, LOW);
  delay((100-dutyCycle) * period / 100);
}
