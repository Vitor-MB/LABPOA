#include "arduino_secrets.h"
#include "arduino_secrets.h"
#include "thingProperties.h"

#include <OneWire.h>
#include <DallasTemperature.h>


const int PINO_ONEWIRE = 13; // Define pino do sensor
OneWire oneWire(PINO_ONEWIRE); // Cria um objeto OneWire
DallasTemperature sensor(&oneWire); // Informa a referencia da biblioteca dallas temperature para Biblioteca onewire
DeviceAddress endereco_temp; // Cria um endereco temporario da leitura do sensor



//Adição futura de uma bomba d'água
const int Bomb = 15;

//Usando o millis
const int intervalo_temperatura = 5000;
const int intervalo_H2 = 3000;

unsigned long int tempoAnt = 0;
unsigned long int tempoAtu = 0;

unsigned long int tempoAntH2 = 0;
unsigned long int tempoAtuH2 = 0;

//Configurando nucleos 0 e 1
TaskHandle_t Task1; //para ler os sensores
TaskHandle_t Task2; //para comunicar-se com o cloud


void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(Bomb, OUTPUT);

  sensor.begin();

  // ---------ARDUINO IOT CLOUD-------
  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  
 /*
  while (!ArduinoCloud.connected()) {  //Enquanto o ESP não conectar, não vai sair daqui
    ArduinoCloud.update();
    delay(400); // pequena pausa pra não travar
  }
  */

  Serial.println("Arduino IoT Cloud conectado!");
  //------------------------------------

  
  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Lendo", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */
  Serial.println("Iniciado o núcleo 0");
  delay(500);

  xTaskCreatePinnedToCore(
      Task2code, /* Function to implement the task */
      "Comunicando", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &Task2,  /* Task handle. */
      1); /* Core where the task should run */
  Serial.println("Iniciado o núcleo 1");
  delay(500);
  

  randomSeed(analogRead(0));
  
}


//Task1 -- Ler os sensores ------

void Task1code( void * pvParameters ){
  Serial.print("Task1(leitura) rodando no núcleo ");
  Serial.println(xPortGetCoreID());
  
  for(;;) {
    tempoAtu = millis();
    tempoAtuH2 = tempoAtu;

    //Atualiza a temperatura a cada 1,5s (Aqui é um valor aleatório entre 0 e 50 para testes)
    if(tempoAtu - tempoAnt >= intervalo_temperatura){
      tempoAnt = tempoAtu;
      
      temperatura = TEMP();
      pH = PH();
      
      Serial.print("Temperatura: ");
      Serial.println(temperatura);
      Serial.print("PH: ");
      Serial.println(pH);
    }

    //Atualiza o hidrogenio a cada 500 ms (HIDROGENIO)
    if(tempoAtuH2 - tempoAntH2 >= intervalo_H2){
      tempoAntH2 = tempoAtuH2;
      
      hidrogenio = H2();
      
      Serial.print("Hidrogenio: ");
      Serial.println(hidrogenio);
    }

    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}


//Task2 -- Comunicando ---------------

void Task2code( void * pvParameters ){
  Serial.print("Task2(comunicação) rodando no núcleo ");
  Serial.println(xPortGetCoreID());
  for(;;) {
    ArduinoCloud.update();
    Serial.println("Update enviado para a nuvem");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void onBombaChange()  {
  digitalWrite(Bomb, bomba); //Bomb é o pino do ESP e bomba é a variável booleana que faz ligar e desligar
}

float TEMP(){
     
  sensor.requestTemperatures(); // Envia comando para realizar a conversão de temperatura
  if (!sensor.getAddress(endereco_temp,0)) { // Encontra o endereco do sensor no barramento

    Serial.println("SENSOR NAO CONECTADO (TEMPERATURA)"); // Sensor conectado, imprime mensagem de erro
    return 999;
  }
   else {
    Serial.print("Temperatura = "); // Imprime a temperatura no monitor serial
    
    return sensor.getTempC(endereco_temp); // Busca temperatura para dispositivo
  }
 
}

int H2(){
  return random(0, 1000)/10.0;
}

int PH(){
  return random(1,14);
}

void loop() {
  
}
