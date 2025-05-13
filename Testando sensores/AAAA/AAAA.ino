#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <OneWire.h>
#include <DallasTemperature.h>

const int PINO_ONEWIRE = 12; // Define pino do sensor
OneWire oneWire(PINO_ONEWIRE); // Cria um objeto OneWire
DallasTemperature sensor(&oneWire); // Informa a referencia da biblioteca dallas temperature para Biblioteca onewire
DeviceAddress endereco_temp; // Cria um endereco temporario da leitura do sensor

double temperatura;

#define col 16
#define lin  2 
#define ende  0x3F 

LiquidCrystal_I2C lcd(0x27,16,2); 

void setup() 
{  
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear();

  Serial.begin(115200); // Inicia a porta serial
  Serial.println("Medindo Temperatura"); // Imprime a mensagem inicial
  sensor.begin(); ; // Inicia o sensor
}
void loop() 
{
  sensor.requestTemperatures(); // Envia comando para realizar a conversão de temperatura
  if (!sensor.getAddress(endereco_temp,0)) { // Encontra o endereco do sensor no barramento
    Serial.println("SENSOR NAO CONECTADO"); // Sensor conectado, imprime mensagem de erro
  } else {
    temperatura = sensor.getTempC(endereco_temp), 1;
    Serial.print("Temperatura = "); // Imprime a temperatura no monitor serial
    Serial.println(temperatura); // Busca temperatura para dispositivo
  }
  delay(200);
  
  lcd.setCursor(1,0); 
  lcd.print("Temperatura "); 
  lcd.setCursor(1, 1);
  lcd.print(temperatura);  
  lcd.setCursor(1,2); 
  delay(600); 
}
