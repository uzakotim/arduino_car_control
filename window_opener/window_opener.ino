// Первый реле замыкает направление мотора (UP DOWN)
// Второй реле замыкает питание мотора(PWR GRND)


// Пины двух реле
const int relayDirection = 8;    // Relay 1 для контроля направления
const int relayPower = 9;        // Relay 2 для контроля питания (on/off)

void setup() {
  // Инициализация реле
  pinMode(relayDirection, OUTPUT);
  pinMode(relayPower, OUTPUT);
  
  // Убедитесь что реле выключены во время стартапа
  digitalWrite(relayDirection, LOW);
  digitalWrite(relayPower, LOW);

  // Инициализация сериал
  Serial.begin(9600);
}

void loop() {
  // Считывание комманд (кнопок) с компьютера
  if (Serial.available() > 0) {
    char command = Serial.read();
    handleSerialCommand(command);
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
  digitalWrite(relayDirection, LOW);

  // Включить питание (Реле 2: ВКЛ)
  digitalWrite(relayPower, HIGH);
  Serial.println("Поднимаю окно...");
}

void lowerWindow() {
  // Установить направление для опускания окна (Реле 1: HIGH для противоположного направления)
  digitalWrite(relayDirection, HIGH);

  // Включить питание (Реле 2: ВКЛ)
  digitalWrite(relayPower, HIGH);
  Serial.println("Опускаю окно...");
}

void stopWindow() {
  // Отключить питание двигателя (Реле 2: ВЫКЛ)
  digitalWrite(relayPower, LOW);
  Serial.println("Окно остановлено.");
}