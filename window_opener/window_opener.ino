// Пины двух реле
const int relayA = 8;    
const int relayB = 9;   
const int transistorPin = 13;
int dutyCycle = 10; 
int period = 100; 
unsigned long previousMicros1 = 0;
unsigned long previousMicros2 = 0;
void setup() {
  // Инициалищация ШИМ
  pinMode(transistorPin, OUTPUT);
  // Инициализация реле
  pinMode(relayA, OUTPUT);
  pinMode(relayB, OUTPUT);
  
  // Убедитесь что реле выключены во время стартапа
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, LOW);

  // Инициализация сериал
  Serial.begin(9600);
}

void loop() {

  // Комманда с компьютера
  if (Serial.available() > 0) {
    char command = Serial.read();
    handleSerialCommand(command);
  }
  // ШИМ Контроль (не-блокирующий)
  unsigned long currentMicros = micros();
  if (currentMicros - previousMicros1 >= dutyCycle * period / 100) {
    previousMicros1 = currentMicros;
    digitalWrite(transistorPin, HIGH);
  }
  if (currentMicros - previousMicros2 >= (100 - dutyCycle) * period / 100) {
    previousMicros2 = currentMicros;
    digitalWrite(transistorPin, LOW);
  }
}

void handleSerialCommand(char command) {
  switch (command) {
    case 'u':   // Комманда поднятия окна
      raiseWindow();
      break;
    case 'd':   // Комманда опускания окна
      lowerWindow();
      break;
    case 's':   // Комманда остановки окна
      stopWindow();
      break;
    default:
      break;
  }
}

void raiseWindow() {
  // Установить направление для поднятия окна (Реле 1: LOW для одного направления)
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, HIGH);
  Serial.println("Поднимаю окно...");
}

void lowerWindow() {
  // Установить направление для опускания окна (Реле 1: HIGH для противоположного направления)
  digitalWrite(relayA, HIGH);
  digitalWrite(relayB, LOW);
  Serial.println("Опускаю окно...");
}

void stopWindow() {
  // Отключить питание двигателя (Реле 1,2: ВЫКЛ)
  digitalWrite(relayA, LOW);
  digitalWrite(relayB, LOW);
  Serial.println("Окно остановлено.");
}