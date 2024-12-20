const int downSwitchPin = 10;
const int upSwitchPin = 11;
const int relayA = 8;    
const int relayB = 9;  
const int currentSensorPin = A0;
const int DELAY_OBSTACLE_LOWERING_WINDOW = 500;
const float MAX_CURRENT = 30.0; //Amps

int stateUP = 0;
int stateDOWN = 0;
int counter = 0;
float current = 0.0;
float sensitivity = 0.066;
char command = 's';
char prev_command = 's'; 

void setup() {
  // put your setup code here, to run once:
  pinMode(downSwitchPin, INPUT);
  pinMode(upSwitchPin, INPUT);
  pinMode(currentSensorPin, INPUT);
  // Инициализация реле
  pinMode(relayA, OUTPUT);
  pinMode(relayB, OUTPUT);
  // Убедитесь что реле выключены во время стартапа
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, LOW);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  stateDOWN = digitalRead(downSwitchPin);
  stateUP = digitalRead(upSwitchPin);
  // Считывание силы тока
  current = abs((analogRead(currentSensorPin) * (5 / 1023.0) - 2.5) / sensitivity);
  // Комманда с компьютера
  if (Serial.available() > 0)
  {
    command = Serial.read();
    // Serial.println(command);
    handleSerialCommand(command);
  }
  Serial.print("current: ");
  Serial.print(current, 3);
  Serial.println();
// FAILURE DETECTION 
  if (current >= 0.35){
    Serial.println("Препядствие!");
    stopWindow();
    delay(100);
    lowerWindow();
    delay(DELAY_OBSTACLE_LOWERING_WINDOW);
    stopWindow();
    delay(100);
  }
// ------------------
  if ((stateDOWN == 0) && (stateUP == 1)){
    if (prev_command == 'u'){
      stopWindow();
      // Serial.println("Не могу поднять окно...");
    }
    if (prev_command == 'd'){
      // Serial.println("Невозможный сценарий");
    }
  }
  if ((stateDOWN == 1) && (stateUP == 0)){
    if (prev_command == 'd'){
      stopWindow();
      // Serial.println("Не могу опустить окно...");
    }
    if (prev_command == 'u'){
      // Serial.println("Невозможный сценарий");
    }
  }

  if ((stateUP == 1) && (stateDOWN == 1)){
    // Serial.println("Неисправные сенсоры!");
  }
  if (stateUP == 1){
    // Serial.println("UP");
  }
  if (stateDOWN == 1){
    // Serial.println("DOWN");
  }

  delay(100);
}



void handleSerialCommand(char command) {
  switch (command) {
    case 'u':   // Комманда поднятия окна
      if (prev_command == 'd'){
        stopWindow();
        delay(100);
      }
      raiseWindow();
      prev_command = 'u';
      break;
    case 'd':   // Комманда опускания окна
      if (prev_command == 'u'){
        stopWindow();
        delay(100);
      }
      lowerWindow();
      prev_command = 'd';
      break;
    case 's':   // Комманда остановки окна
      stopWindow();
      prev_command = 's';
      break;
    default:
      break;
  }
}

void raiseWindow() {
  // Установить направление для поднятия окна (Реле 1: LOW для одного направления)
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, HIGH);
  // Serial.println("Поднимаю окно...");
}

void lowerWindow() {
  // Установить направление для опускания окна (Реле 1: HIGH для противоположного направления)
  digitalWrite(relayA, HIGH);
  digitalWrite(relayB, LOW);
  // Serial.println("Опускаю окно...");
}

void stopWindow() {
  // Отключить питание двигателя (Реле 1,2: ВЫКЛ)
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, LOW);
  // Serial.println("Окно остановлено.");
}