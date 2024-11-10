const int downSwitchPin = 10;
const int upSwitchPin = 11;
const int relayA = 8;    
const int relayB = 9;  
const int currentSensorPin = A0;
const int WINDOW_SIZE = 10;
const int DELAY_OBSTACLE_LOWERING_WINDOW = 500;
const float MAX_CURRENT = 30.0; //Amps
const float SLOPE_THRESHOLD = 2.0;

int stateUP = 0;
int stateDOWN = 0;
int counter = 0;
float current = 0.0;
float voltage = 0.0;
float current_readings[WINDOW_SIZE];

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
  for (int i=0;i<WINDOW_SIZE;i++){
    current_readings[i] = 0;
  }
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  stateDOWN = digitalRead(downSwitchPin);
  stateUP = digitalRead(upSwitchPin);
  // Считывание силы тока
  voltage = analogRead(currentSensorPin);
  current = (voltage/ 1024.0) * MAX_CURRENT;
  addCurrentReading(current);
  for (int i=0;i<WINDOW_SIZE;i++){
    Serial.print(current_readings[i]); 
    Serial.print(" ");
  }
  Serial.print("\n");
  // Комманда с компьютера
  if (Serial.available() > 0)
  {
    command = Serial.read();
    // Serial.println(command);
    handleSerialCommand(command);
  }

// FAILURE DETECTION 
  // if (isSlopeGreaterThan(current_readings, WINDOW_SIZE, SLOPE_THRESHOLD)){
    // Serial.println("Препядствие!!!");
    // stopWindow();
    // delay(100);
    // lowerWindow();
    // delay(DELAY_OBSTACLE_LOWERING_WINDOW);
  // }
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

void addCurrentReading(float new_reading) {
    // Shift all elements to the left
    for (int i = 0; i < WINDOW_SIZE - 1; i++) {
        current_readings[i] = current_readings[i + 1];
    }
    // Add the new reading at the end of the array
    current_readings[WINDOW_SIZE - 1] = new_reading;
}

float calculateSlope(const float currentReadings[], int n) {
    if (n <= 1) {
        return 0; // Not enough data to calculate slope
    }
    
    float sumX = 0;
    float sumY = 0;
    float sumXY = 0;
    float sumX2 = 0;

    for (int i = 0; i < n; i++) {
        float x = i;                // Time can be represented as the index
        float y = currentReadings[i]; // Current measurement

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    // Calculate slope (m) using the formula: m = (n * Σ(x*y) - Σx * Σy) / (n * Σ(x^2) - (Σx)^2)
    float slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);

    return slope;
}

bool isSlopeGreaterThan(const float currentReadings[], int n, float threshold) {
    float slope = calculateSlope(currentReadings, n);
    return abs(slope) > threshold;
}