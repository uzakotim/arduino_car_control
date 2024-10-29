const int downSwitchPin = 10;
const int upSwitchPin = 11;
int stateUP = 0;
int stateDOWN = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(downSwitchPin, INPUT);
  pinMode(upSwitchPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  stateDOWN = digitalRead(downSwitchPin);
  stateUP = digitalRead(upSwitchPin);
  if (stateUP == 1){
    Serial.println("UP pressed");
  }
  if (stateDOWN == 1){
    Serial.println("DOWN pressed");
  }
  delay(100);
}
