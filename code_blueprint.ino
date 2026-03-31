
#include "DHT.h"

#define DHTPIN 32
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// -------- INPUT PINS --------
int motionPin = 34;   // IR sensor
int tiltPin = 33;
int soundPin = 35;
int waterPin = 39;

int trigPin = 5;      // Ultrasonic
int echoPin = 18;

// -------- OUTPUT PINS --------
int buzzer = 26;
int redLED = 27;
int greenLED = 14;
int yellowLED1 = 12;
int yellowLED2 = 13;
int mainLight = 25;

// -------- ULTRASONIC FUNCTION --------
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // timeout safety
  long distance = duration * 0.034 / 2;

  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(motionPin, INPUT);
  pinMode(tiltPin, INPUT);
  pinMode(soundPin, INPUT);
  pinMode(waterPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED1, OUTPUT);
  pinMode(yellowLED2, OUTPUT);
  pinMode(mainLight, OUTPUT);

  dht.begin();
}

void loop() {

  // -------- READ ALL SENSORS --------
  int motion = digitalRead(motionPin);
  int tilt = digitalRead(tiltPin);
  int sound = digitalRead(soundPin);
  int water = digitalRead(waterPin);

  float temp = dht.readTemperature();
  long distance = getDistance();

  // -------- RESET EVERYTHING --------
  digitalWrite(buzzer, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED1, LOW);
  digitalWrite(yellowLED2, LOW);
  digitalWrite(mainLight, LOW);

  // -------- 1. ENTRY SYSTEM --------
  if (motion == HIGH) {
    digitalWrite(mainLight, HIGH);     // light ON
    digitalWrite(yellowLED1, HIGH);    // welcome lights
    digitalWrite(yellowLED2, HIGH);
  }

  // -------- 2. ULTRASONIC (6 cm boundary) --------
  if (distance > 0 && distance <= 6) {
    digitalWrite(buzzer, HIGH);
  }

  // -------- 3. TILT (artifact moved) --------
  if (tilt == HIGH) {
    digitalWrite(buzzer, HIGH);
  }

  // -------- 4. FIRE (HIGH TEMP) --------
  if (temp > 35) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLED, HIGH);     // danger
    digitalWrite(greenLED, HIGH);   // safe exit
  }

  // -------- 5. SOUND (disturbance) --------
  if (sound == HIGH) {
    digitalWrite(redLED, HIGH);
  }

  // -------- 6. FLOOD (water high) --------
  if (water == HIGH) {
    digitalWrite(yellowLED1, HIGH);
  }

  delay(150);
}+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++