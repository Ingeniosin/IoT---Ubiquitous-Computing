#include <SoftwareSerial.h>

#define TRIGGER 5
#define ECHO 6

long duration;
bool isBusy;
String items;

const int rxPin = 10;
const int txPin = 11;

SoftwareSerial espSerial(rxPin, txPin);


void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void handleMessages() {
  if (espSerial.available()) {
    String espData = espSerial.readStringUntil('\n');
    Serial.println("Mensaje desde ESP8266: " + espData + "");
    isBusy = false;
  }

  if (Serial.available()) {
    String arduinoData = Serial.readStringUntil('\n');
    espSerial.print(arduinoData);
    espSerial.print('\n');
    Serial.println("Mensaje enviado a ESP8266: " + arduinoData + "");
  }
}

void handleItems() {
  items = "";

  items = items + obtenerItemDouble("distancia", medirDistancia());


  Serial.println("Enviando items: " + items + "");

  espSerial.print(items);
  espSerial.print('\n');
  isBusy = true;
}


void loop() {
  handleMessages();

  if (isBusy) {
    return;
  }

  handleItems();

  delay(2000);
}


String obtenerItemDouble(String name, double value) {
  return "{ \"name\": \"distancia\", \"doubleValue\": " + String(value) + " },";
}


double medirDistancia() {
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  duration = pulseIn(ECHO, HIGH);

  double distancia = duration * 0.034 / 2;

  return distancia;
}