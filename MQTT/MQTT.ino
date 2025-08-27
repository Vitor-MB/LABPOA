#include <ArduinoMqttClient.h>
#include <WiFi.h>

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.0.50";
int        port     = 1883;
const char Topico[]  = "teste";
const char motor[]  = "controle/motor";

const long interval = 10000;
unsigned long TempoAnt = 0;

int count = 0;

void setup() {
  
  Serial.begin(115200);
  while (!Serial) {
    ; 
  }

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
      Serial.print("Conexao MQTT falhou! Error code = ");
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

    Serial.print("Enviando mensagem ao Topico: ");
    Serial.println(Topico);
    Serial.print("EspComunicando ");
    Serial.println(count);

    // Envia a mensagem
    mqttClient.beginMessage(Topico, false, 2); //QoS 2 (Exatamente 1 vez)
    mqttClient.print("ESPComunicando ");
    mqttClient.print(count);
    mqttClient.endMessage();

    Serial.println();

    count++;
  }
}


void onMqttMessage(int messageSize) {
  // we received a message, print out the Topico and contents
  Serial.print("Received a message with Topico '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
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
      Serial.print("Conexao MQTT falhou! Error code = ");
      Serial.println(mqttClient.connectError());
      delay(5000);
    }
    Serial.println("Conexao MQTT reestabelecida");
  }
}
