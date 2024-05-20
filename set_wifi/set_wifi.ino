#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

const char* SSID = "CLAP";
const char* PASSWORD = "00966783708";
const char* SERVER_URL = "https://iot-handler.tunnelto.dev/api/add-track";
const char* FINGERPRINT = "E1D5751E3FA43C4C286520697F34EA8308289DE9";

void sendPostRequest(const String& items) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    HTTPClient http;
    client.setFingerprint(FINGERPRINT);
    http.begin(client, SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    String postData = "{\"items\":[" + items + "]}";
    int httpResponseCode = http.POST(postData);
    Serial.println(String(httpResponseCode));
    http.end();
  } else {
    Serial.println("Error: No conectado a WiFi");
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red WiFi: " + String(SSID));
  Serial.println("Dirección IP: " + WiFi.localIP().toString());
}

void loop() {
  if (Serial.available()) {
    String arduinoData = Serial.readStringUntil('\n');
    delay(5);
    sendPostRequest(arduinoData);
  }
}