#include <PubSubClient.h>

extern unsigned long tempo;

void IRAM_ATTR setRedOn() {
  digitalWrite(RED, !digitalRead(RED));
}

void IRAM_ATTR setGreenOn() {
  digitalWrite(GREEN, !digitalRead(GREEN));
}

void IRAM_ATTR setBlueOn() {
  digitalWrite(BLUE, !digitalRead(BLUE));
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MSG recebida no tópico: ");
  Serial.println(topic);
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.println("Conteudo: " + msg);

  if (String(topic) == String(topico_texto) && msg.indexOf("RED") != -1) {
    int indice = msg.indexOf(": ");
    int valor = msg.substring(indice + 2).toInt();
    valor = constrain(valor, 0, 1000);

    int valorPWM = (valor * 255) / 1000;
    analogWrite(RED, valorPWM);
  } else if (String(topic) == String(topico_texto) && msg.indexOf("GREEN") != -1) {
    int indice = msg.indexOf(": ");
    int valor = msg.substring(indice + 2).toInt();
    valor = constrain(valor, 0, 1000);

    int valorPWM = (valor * 255) / 1000;
    analogWrite(GREEN, valorPWM);
  } else if (String(topic) == String(topico_texto) && msg.indexOf("BLUE") != -1) {
    int indice = msg.indexOf(": ");
    int valor = msg.substring(indice + 2).toInt();
    valor = constrain(valor, 0, 1000);

    int valorPWM = (valor * 255) / 1000;
    analogWrite(BLUE, valorPWM);
  } else if (String(topic) == String(topico_texto) && msg.indexOf("REDB") != -1) {
    int indice = msg.indexOf(": ");
    int valor = msg.substring(indice + 2).toInt();
    valor = valor * 10000;

    timerAttachInterrupt(timer, &setRedOn);
    timerAlarm(timer, valor, true, 0);
  } else if (String(topic) == String(topico_texto) && msg.indexOf("GREENB") != -1) {
    int indice = msg.indexOf(": ");
    int valor = msg.substring(indice + 2).toInt();
    valor = valor * 10000;
    
    timerAttachInterrupt(timer, &setGreenOn);
    timerAlarm(timer, valor, true, 0);
  } else if (String(topic) == String(topico_texto) && msg.indexOf("BLUEB") != -1) {
    int indice = msg.indexOf(": ");
    int valor = msg.substring(indice + 2).toInt();
    valor = valor * 10000;
    
    timerAttachInterrupt(timer, &setBlueOn);
    timerAlarm(timer, valor, true, 0);
  }
}

boolean attemptMqttConnection() {
  String clientId = String(mqtt_client_id) + "_" + String(WiFi.macAddress());
  Serial.print("Tentando MQTT como: " + clientId + "... ");
  if (client.connect(clientId.c_str(), NULL, NULL, topico_status, 0, true, "OFFLINE")){
    Serial.println("CONECTADO!");
    client.publish(topico_status, "ONLINE", true);
    client.subscribe(topico_comando);
    client.subscribe(topico_texto);
    return true;
  } else {
    Serial.print("Falha. rc=");
    Serial.print(client.state());
    return false;
  }
}

void taConectado(){
  if (!client.connected()) {
    isConnected = false;
    if (tempo > 5000) {
      tempo = 0;
      if (attemptMqttConnection()) tempo = 0;
    }
  } else {
    isConnected = true;
    client.loop();
  }
}