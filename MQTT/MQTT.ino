#include <ArduinoMqttClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <DS1307.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.0.50";
int        port     = 1883;
const char TopicoSensores[]  = "monitoramento/sensores";
const char motor[]  = "controle/motor";


uint8_t sec, minute, hour, day, month;
uint16_t year;
DS1307 rtc;

const long interval = 10000;
unsigned long TempoAnt = 0;


void setup() {
  
  Serial.begin(115200);
  while (!Serial) {
    ; 
  }
  
  rtc.begin();
  rtc.start();
  
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("Conectado a rede");
  Serial.println();

  mqttClient.setId("TesteEsp");
  // Usuario de Autenticação
  mqttClient.setUsernamePassword("LabPOA", "LabPOA123");
 
  Serial.print("Tentando se conectar ao broker: ");
  Serial.println(broker);

  while(!mqttClient.connect(broker, port)){
      Serial.print("Conexao MQTT falhou! Codigo de erro = ");
      Serial.println(mqttClient.connectError());
      delay(5000);
    }

  Serial.println("Conectado ao broker MQTT!");
  Serial.println();

  //CALLBACK
  mqttClient.onMessage(onMqttMessage);
 
  Serial.print("Assinando o Topico: ");
  Serial.println(motor);
  Serial.println();
  mqttClient.subscribe(motor);
}

void loop() {
  ConfereConexoes();
  
  mqttClient.poll();

  unsigned long currentMillis = millis();
  
  if (currentMillis - TempoAnt >= interval) {
    TempoAnt = currentMillis;
    
    rtc.get(&sec, &minute, &hour, &day, &month, &year);

    
    char timestamp[25];
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02dT%02d:%02d:%02dZ" ,year, month, day, hour, minute, sec);

    // --- Gerar msg_id ---
    String msgId = String("msg_") + String(year)+ String(month)+String(day)+ String(sec) + String(millis());
    
    StaticJsonDocument<200> doc;
    doc["msg_id"] = msgId;
    doc["timestamp"] = timestamp;
    doc["Temperatura"] = 27.2;
    doc["PH"] = 7.1;
    doc["TDS"] = 22;
    doc["Condutividade"] = 0.15;
    doc["H2"] = 27.2;
    doc["Turbidez"] = 2;

    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);
    
    Serial.print("Enviando mensagem ao TopicoSensores: ");
    Serial.println(TopicoSensores);
    Serial.print("Enviando JSON: ");
    Serial.println(jsonBuffer);


    // Envia a mensagem (JSON)
    mqttClient.beginMessage(TopicoSensores, true, 2); //QoS 2 (Exatamente 1 vez)
    mqttClient.print(jsonBuffer);
    mqttClient.endMessage();

    Serial.println();

  }
}


void onMqttMessage(int messageSize) {
  // we received a message, print out the Topico and contents
  Serial.print("Mesagem recebida com Topico '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', Tamanho ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  String payload = "";
  while (mqttClient.available()) {
    char c = (char)mqttClient.read();
    payload += c;
  }
  Serial.println(payload);
  Serial.println();

  Serial.println();
}

void ConfereConexoes(){
  if(WiFi.status() != WL_CONNECTED){
    
    switch(WiFi.status()){
      case WL_NO_SSID_AVAIL:
        Serial.println("Rede WiFi não encontrada");
        break;
        
      case WL_CONNECTION_LOST:
      Serial.println("Conexão WiFi perdida");
      break;
      
    case WL_DISCONNECTED:
      Serial.println("WiFi Desconectado");
      break;
    }
    
    Serial.println("Tentando reestabelecer a conexao WiFi");
    WiFi.begin(ssid, pass);
    
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(5000);
    }
    Serial.println("Conexao WiFi reestabelecida");
  }
  
  if(!mqttClient.connected()){
    Serial.println("Conexao MQTT perdida");
    Serial.println("Tentando reestabelecer a conexao"); 

    while( !mqttClient.connect(broker, port)){
      Serial.print("Conexao MQTT falhou! Codigo de Erro = ");
      Serial.println(mqttClient.connectError());
      delay(5000);
    }
    Serial.println("Conexao MQTT reestabelecida");
  }
}
