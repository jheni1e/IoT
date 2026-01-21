#ifndef ENV_H
#define ENV_H

#define WIFI_SSID ""
#define WIFI_PASS ""
#define LED_PIN 22                                                             //led que já vem soldado na esp32 

const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_client_id = "ESP32_patrick"; 
const char* topico_comando = "bosch/ets/dta/patrick/luz";
const char* topico_status  = "bosch/ets/dta/patrick/status";
const int   mqtt_port = 1883;

#endif