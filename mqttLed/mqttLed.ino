#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"

unsigned long tempo = 0;
bool isConnected = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  configWifi();      
  configuraTimer();
}

void loop() {
  taConectado();
}