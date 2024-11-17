#include <SoftwareSerial.h>

// Software Serial untuk Bluetooth RC-06
SoftwareSerial BTSerial(0, 1); // RX, TX

// Motor control pins untuk Shield Sensor V5.0
const int enAPin = 6;    // Kontrol kecepatan PWM motor kiri
const int in1Pin = 7;    // Arah 1 motor kiri
const int in2Pin = 5;    // Arah 2 motor kiri
const int in3Pin = 4;    // Arah 1 motor kanan
const int in4Pin = 2;    // Arah 2 motor kanan
const int enBPin = 3;    // Kontrol kecepatan PWM motor kanan

// Command definitions
#define FORWARD 'F'
#define BACKWARD 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define STOP 'S'
#define START 'X'    
#define PAUSE 'P'    

// Motor speed settings
const int NORMAL_SPEED = 150;
const int TURN_SPEED = 130;
const int ZERO_SPEED = 0;

// Status LED pin untuk debugging
const int statusLED = 13;

// Variable untuk tracking robot state
bool isRunning = false;
bool isPaused = false;
char lastCommand = STOP;  // Menyimpan command terakhir sebelum pause

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Tunggu serial port tersambung
  }
  
  // Inisialisasi Bluetooth dengan baud rate 9600 (standar untuk RC-06)
  BTSerial.begin(9600);
  
  // Configure motor control pins
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  pinMode(statusLED, OUTPUT);
  
  stopMotors();
  
  // Startup indication
  digitalWrite(statusLED, HIGH);
  delay(1000);
  digitalWrite(statusLED, LOW);
  
  Serial.println(F("Robot Ready! Waiting for commands..."));
  BTSerial.println("Robot Ready!");
  
  testMotors();
}

void loop() {
  // Status LED blink
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 1000) {
    digitalWrite(statusLED, !digitalRead(statusLED));
    lastBlink = millis();
  }
  
  if (BTSerial.available()) {
    char command = BTSerial.read();
    
    Serial.print(F("Received command: "));
    Serial.println(command);
    
    // Flash LED untuk indikasi
    digitalWrite(statusLED, HIGH);
    delay(50);
    digitalWrite(statusLED, LOW);
    
    handleCommand(command);
  }
  
  // Echo dari Serial Monitor ke Bluetooth (untuk testing)
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
  }
}

void handleCommand(char command) {
  switch (command) {
    case START:
      if (!isRunning || isPaused) {
        isRunning = true;
        isPaused = false;
        Serial.println(F("Robot Started"));
        if (lastCommand != STOP) {
          executeCommand(lastCommand);  // Resume last action
        }
      }
      break;
      
    case PAUSE:
      if (isRunning && !isPaused) {
        isPaused = true;
        stopMotors();
        Serial.println(F("Robot Paused"));
      }
      break;
      
    default:
      if (isRunning && !isPaused) {
        lastCommand = command;
        executeCommand(command);
      }
      break;
  }
}

void executeCommand(char command) {
  Serial.print(F("Executing command: "));
  Serial.println(command);
  
  switch (command) {
    case FORWARD:
      Serial.println(F("Moving Forward"));
      moveForward();
      break;
    case BACKWARD:
      Serial.println(F("Moving Backward"));
      moveBackward();
      break;
    case LEFT:
      Serial.println(F("Turning Left"));
      turnLeft();
      break;
    case RIGHT:
      Serial.println(F("Turning Right"));
      turnRight();
      break;
    case STOP:
      Serial.println(F("Stopping"));
      stopMotors();
      break;
    default:
      Serial.print(F("Unknown command: "));
      Serial.println(command);
      stopMotors();
      break;
  }
}

void testMotors() {
  Serial.println(F("Testing motors..."));
  
  // Test left motor
  Serial.println(F("Left motor forward"));
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, 100);
  delay(500);
  analogWrite(enAPin, 0);
  
  // Test right motor
  Serial.println(F("Right motor forward"));
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enBPin, 100);
  delay(500);
  analogWrite(enBPin, 0);
  
  Serial.println(F("Motor test complete"));
}

void moveForward() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, NORMAL_SPEED);
  
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enBPin, NORMAL_SPEED);
}

void moveBackward() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enAPin, NORMAL_SPEED);
  
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enBPin, NORMAL_SPEED);
}

void turnLeft() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enAPin, TURN_SPEED);
  
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enBPin, TURN_SPEED);
}

void turnRight() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enAPin, TURN_SPEED);
  
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enBPin, TURN_SPEED);
}

void stopMotors() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enAPin, ZERO_SPEED);
  analogWrite(enBPin, ZERO_SPEED);
}
