#include <WiFi.h>
#include "time.h"

const char* servidor = "pool.ntp.org"; //network time protocol
const long fuso = -10800; //-3h * 3600
const int verao = 0;

struct tm timeinfo;

const char* ssid = "iPhone de Jhenifer";
const char* pass = "quadrado1";

void WiFiEvent(WiFiEvent_t event) {
  unsigned long atual = millis();

  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("Conectado");
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("Desconectado");
      WiFi.reconnect();
      break;
  }
}

void setup() {
  Serial.begin(9600);

  WiFi.onEvent(WiFiEvent);
  WiFi.begin(ssid, pass);

  configTime(fuso, verao, servidor);
}

void loop() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Sem hora");
    return;
  }
    
  Serial.printf("Horário atual: %02d:%02d %02d/%02d/%d\n",
  timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
}