#include <Servo.h>
Servo servo;

// Pin-pin Modul Ultrasonik
const int trigPin = 13; // Pulsa tinggi 10 mikrodetik menyebabkan chirp, tunggu 50 us
const int echoPin = 12; // Lebar pulsa tinggi menunjukkan jarak

// Motor servo yang mengarahkan sensor ultrasonik
const int servoPin = 11; // Output PWM untuk servo hobby

// Pin-pin kontrol motor: H bridge L298N
const int enAPin = 6;    // Kontrol kecepatan PWM motor kiri
const int in1Pin = 7;    // Arah 1 motor kiri
const int in2Pin = 5;    // Arah 2 motor kiri
const int in3Pin = 4;    // Arah 1 motor kanan
const int in4Pin = 2;    // Arah 2 motor kanan
const int enBPin = 3;    // Kontrol kecepatan PWM motor kanan

#define NUM_ANGLES 7
unsigned char sensorAngle[NUM_ANGLES] = {60, 70, 80, 90, 100, 110, 120};
unsigned int distance[NUM_ANGLES];

enum Motor { LEFT, RIGHT };

// Atur kecepatan motor: 255 maju penuh, -255 mundur penuh, 0 berhenti
void go(enum Motor m, int speed) {
  digitalWrite(m == LEFT ? in1Pin : in3Pin, speed > 0 ? HIGH : LOW);
  digitalWrite(m == LEFT ? in2Pin : in4Pin, speed <= 0 ? HIGH : LOW);
  analogWrite(m == LEFT ? enAPin : enBPin, speed < 0 ? -speed : speed);
}

// Pengujian awal motor
void testMotors() {
  static int speed[8] = {128, 255, 128, 0, -128, -255, -128, 0};
  go(RIGHT, 0);
  for (unsigned char i = 0; i < 8; i++) {
    go(LEFT, speed[i]);
    delay(200);
  }
  for (unsigned char i = 0; i < 8; i++) {
    go(RIGHT, speed[i]);
    delay(200);
  }
}

// Baca jarak dari sensor ultrasonik, mengembalikan jarak dalam mm
unsigned int readDistance() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long period = pulseIn(echoPin, HIGH);
  return period * 343 / 2000;
}

// Pindai area di depan dengan menggerakkan sensor ultrasonik ke kiri dan kanan
void readNextDistance() {
  static unsigned char angleIndex = 0;
  static signed char step = 1;
  distance[angleIndex] = readDistance();
  angleIndex += step;
  if (angleIndex == NUM_ANGLES - 1) step = -1;
  else if (angleIndex == 0) step = 1;
  servo.write(sensorAngle[angleIndex]);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);
  servo.attach(servoPin);
  servo.write(90);
  go(LEFT, 0);
  go(RIGHT, 0);
  testMotors();
  servo.write(sensorAngle[0]);
  delay(200);
  for (unsigned char i = 0; i < NUM_ANGLES; i++) {
    readNextDistance();
    delay(200);
  }
}

void loop() {
  readNextDistance();
  // Periksa apakah ada sesuatu yang terlalu dekat di sudut manapun
  unsigned char tooClose = 0;
  for (unsigned char i = 0; i < NUM_ANGLES; i++) {
    if (distance[i] < 300) {
      tooClose = 1;
    }
  }
  
  if (tooClose) {
    // Ada sesuatu di dekat: mundur ke kiri
    go(LEFT, -180);
    go(RIGHT, -80);
  } else {
    // Tidak ada halangan: maju
    go(LEFT, 255);
    go(RIGHT, 255);
  }
  delay(50);
}
