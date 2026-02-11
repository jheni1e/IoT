#include <WiFi.h>
#include "time.h"

const int verde = 4;
const int vermelho = 16;

const char* ssid = "iPhone de Jhenifer";
const char* pass = "quadrado1";

hw_timer_t *timer = NULL;

void IRAM_ATTR setRedOn() {
  digitalWrite(vermelho, !digitalRead(vermelho));
  digitalWrite(verde, 0);
}

void IRAM_ATTR setGreenOn() {
  digitalWrite(verde, !digitalRead(verde));
  digitalWrite(vermelho, 0);
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("Conectado");
      timerAttachInterrupt(timer, &setGreenOn);
      timerAlarm(timer, 100000, true, 0);
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("Desconectado");
      timerAttachInterrupt(timer, &setRedOn);
      timerAlarm(timer, 500000, true, 0);
      WiFi.reconnect();
      break;
  }
}

void setup() {
  Serial.begin(9600);

  WiFi.onEvent(WiFiEvent);
  WiFi.begin(ssid, pass);

  pinMode(verde, OUTPUT);
  pinMode(vermelho, OUTPUT);

  timer = timerBegin(1000000);
}

void loop() {}