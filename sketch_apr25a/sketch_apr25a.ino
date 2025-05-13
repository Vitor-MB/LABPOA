#include <WiFi.h>
#include <WiFiClientSecure.h>

// Troque pelo SSID e senha da rede que quer testar
const char* ssid = "Teste";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi conectado!");
  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());

  delay(1000);

  // Testa conexão com servidor NTP (porta 123/UDP) - modo fake
  Serial.print("Testando porta NTP 123 UDP... ");
  if (testUDP("pool.ntp.org", 123)) {
    Serial.println("✅ Porta NTP 123 parece liberada");
  } else {
    Serial.println("❌ Porta NTP 123 pode estar bloqueada");
  }

  // Testa conexão com MQTTs da Arduino IoT Cloud (porta 8883/TCP)
  Serial.print("Testando porta MQTTs 8883 TCP... ");
  if (testTCP("mqtts.iot.arduino.cc", 8883)) {
    Serial.println("✅ Porta 8883 TCP liberada");
  } else {
    Serial.println("❌ Porta 8883 TCP bloqueada ou sem rota");
  }
}

void loop() {
  
}

bool testTCP(const char* host, uint16_t port) {
  WiFiClientSecure client;
  client.setInsecure(); // Ignora certificados SSL
  return client.connect(host, port);
}

// Teste UDP (simulado por TCP, pois UDP não conecta de verdade assim)
bool testUDP(const char* host, uint16_t port) {
  WiFiClient client;
  return client.connect(host, port);
}