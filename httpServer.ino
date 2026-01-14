#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h> 
#include "env.h"    

#define LED_PIN 22                                           

AsyncWebServer server(80);                                      //cria uma nova instância do objeto e inicia na porta 80 (padrão mundial para http)

float temperaturaAtual = 0.0;                                   //variável para guardar os valores de temperatura
bool motorLigado = false;                                       //variável para guardar o valor de ligado/desligado

void lerSensores() {                                            //função para simular os valores de temperatura (vocês deverão substituir por uma leitura real)
  temperaturaAtual = random(200, 450) / 10.0;
  temperaturaAtual = digitalRead(sensorPin);
}

void WiFiEvent(WiFiEvent_t event) {                                           //callback para tentar reconectar no wifi                                
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP: 
          Serial.println("conectado no wifi, IP: " + WiFi.localIP());                     
          break;
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
          Serial.println("desconectado do wifi, tentando novamente...");
          WiFi.reconnect();                                                 //tenta voltar sozinho
          break;
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);                            //inicia o wifi com os argumentos do arquivo "env.h"
  
  Serial.print("Conectando ao Wi-Fi...");
}

void loop() {
  //n precisa de nada pois tá tudo com "callback"
}

void criaServidor(){
  /*
  DefaultHeaders -> classe definida em "ESPAsyncWebServer"
  Instance()     -> retorna a instancia do objeto (design pattern ->> singleton)
  .addHeader     -> chama o método dele 
  Access-Control-Allow-Origin     ->> é uma string do protocolo global http que significa "permitir a origem de..."
  como usamos "*" dentro do método, signfica que não temos restrição de origem, então qualquer um pode ler a resposta (acessar os dados)

  do mesmo modo, "Access-Control-Allow-Methods" permitir que a origem utilize esses métodos, ai restringimos para GET, POST e PUT
  */
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){  //define Rota + Verbo (GET)
 
    lerSensores();                                                        //atualiza as variáveis globais AGORA 
    
    JsonDocument doc;                                                     //instancia o objeto manipulador de JSON (Gerencia alocação de memória).
    
    doc["temp"] = temperaturaAtual;                                       //a biblioteca detecta o tipo da variável automaticamente.
    doc["rele"] = motorLigado;                                            //booleanos são convertidos automaticamente para "true"/"false".
    
    doc["ip"] = WiFi.localIP().toString();                                //IP da placa para debug.
    doc["uptime"] = millis() / 1000;                                      //tempo de atividade em segundos.
    doc["memoria_livre"] = ESP.getFreeHeap();                             //monitoramento de RAM livre

    /* visualização do objeto "doc"
       {
         "temp": 24.5,
         "rele": true,
         "ip": "192.168.0.105",
         "uptime": 150,
         "memoria_livre": 200000
       }
    */

    String response;                                                    //string vazia que servirá de buffer.
    
    serializeJson(doc, response);                                       //transforma o Objeto C/C++ em Texto
    
    /*
    Envia o pacote HTTP de volta:
    - Status 200: OK (Sucesso).
    - "application/json": Avisa o cliente para tratar como Objeto, não como HTML.
    - Payload: O texto JSON gerado acima.
    */
    request->send(200, "application/json", response);
  });

  server.on("/api/control", HTTP_GET, [](AsyncWebServerRequest *request){
    
    //verifica se o parâmetro "state" existe na URL para evitar erros de ponteiro nulo
    if (request->hasParam("state")) {
      
      
      String valor = request->getParam("state")->value();            // Busca o valor associado à chave "state". O operador "->" acessa o método do objeto apontado
      
      if (valor == "on") {
        motorLigado = true;                                          // Atualiza estado lógico (Memória)
        digitalWrite(LED_PIN, HIGH);                                 // Atualiza estado físico (Hardware)
        Serial.println("CMD: Motor LIGADO");
      } 
      else if (valor == "off") {
        motorLigado = false;
        digitalWrite(LED_PIN, LOW);
        Serial.println("CMD: Motor DESLIGADO");
      }
      
      
      request->send(200, "text/plain", "OK");                       // HTTP 200 (OK): Confirma para o Frontend que o comando foi processado
    } 
    else {
      request->send(400, "text/plain", "Erro: Parametro 'state' obrigatorio");  // HTTP 400 (Bad Request): O cliente fez uma requisição malformada (esqueceu o parâmetro).
    }
  });

  /*isso aqui só serve pra avisar quem acessar "192.168.1.0/" que na verdade o correto é "192.168.1.0/api/status"
  */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){                  
    request->send(200, "text/plain", "API bigode online. Acesse /api/status no seu Dashboard.");
  });

  server.begin(); //lógicamente inicia o servidor
}