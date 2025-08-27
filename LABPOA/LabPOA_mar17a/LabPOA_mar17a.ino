#include "arduino_secrets.h"
#include "arduino_secrets.h"
#include "thingProperties.h"

//Adição futura de uma bomba d'água
const int Bomb = 15;

//MILLIS
unsigned long int tempoComunicacao = 0;
unsigned long int tempoMedicao = 0;
//Intervalos
unsigned long int IntervaloMedicao = 6000;
unsigned long int IntervaloComunicacao = 20000;



void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(Bomb, OUTPUT);

  // ---------ARDUINO IOT CLOUD-------
  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(DBG_INFO);
  ArduinoCloud.printDebugInfo();
  //------------------------------------

  tempoMedicao = millis();
  tempoComunicacao = millis();

  Serial.print("Conectando: ");
  while(!ArduinoCloud.connected()){
    ArduinoCloud.update();
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conectado");
  
  randomSeed(analogRead(0));
  
}

void loop() {
    
    ArduinoCloud.update();
    
    if(millis() - tempoMedicao >= IntervaloMedicao){
      
      temperatura = TEMP();
      pH = PH();
      hidrogenio = H2();
      
      Serial.print("Temperatura: ");
      Serial.println(temperatura);
      Serial.print("PH: ");
      Serial.println(pH);
      Serial.print("H2: ");
      Serial.println(hidrogenio);

      Serial.println("\nMedidas Atualizadas\n");
      tempoComunicacao = millis();
    }
}

void onBombaChange()  {
  Serial.print("Bomba :");
  Serial.println(bomba);
  digitalWrite(Bomb, bomba); //Bomb é o pino do ESP e bomba é a variável booleana que faz ligar e desligar
}

float TEMP(){
  return random(100, 500)/10.0;
}

int H2(){
  return random(0, 1000)/10.0;
}

int PH(){
  return random(1,14);
}
