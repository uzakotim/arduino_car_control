// Pin assignments for relays
const int relayDirection = 8;    // Relay 1 for controlling direction
const int relayPower = 9;        // Relay 2 for controlling power (on/off)

void setup() {
  // Initialize the relay pins as outputs
  pinMode(relayDirection, OUTPUT);
  pinMode(relayPower, OUTPUT);
  
  // Ensure both relays are off at startup
  digitalWrite(relayDirection, LOW);
  digitalWrite(relayPower, LOW);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read serial commands
  if (Serial.available() > 0) {
    char command = Serial.read();
    handleSerialCommand(command);
  }
}

void handleSerialCommand(char command) {
  switch (command) {
    case 'u':   // Command to raise the window
      raiseWindow();
      break;
    case 'd':   // Command to lower the window
      lowerWindow();
      break;
    case 's':   // Command to stop the window
      stopWindow();
      break;
    default:
      break;
  }
}

void raiseWindow() {
  // Set direction for raising window (Relay 1: LOW for one direction)
  digitalWrite(relayDirection, LOW);

  // Turn on power (Relay 2: ON)
  digitalWrite(relayPower, HIGH);
  Serial.println("Raising window...");
}

void lowerWindow() {
  // Set direction for lowering window (Relay 1: HIGH for opposite direction)
  digitalWrite(relayDirection, HIGH);

  // Turn on power (Relay 2: ON)
  digitalWrite(relayPower, HIGH);
  Serial.println("Lowering window...");
}

void stopWindow() {
  // Turn off power to the motor (Relay 2: OFF)
  digitalWrite(relayPower, LOW);
  Serial.println("Window stopped.");
}