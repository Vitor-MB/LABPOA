#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
 
 
double calibracao_ph7 = 2.51;   // Tensão obtida em solução de calibração pH 7
double calibracao_ph4 = 3.29;   // Tensão obtida em solução de calibração pH 4
double calibracao_ph10 = 2.06;  // Tensão obtida em solução de calibração pH 10
 
#define UTILIZAR_PH_10 true  // Habilita calibração entre pH 7 e 10, \
                                 caso contrário utiliza pH 7 e 4.

#define Sensor_PH 15

float m;
float b;

int buf[20];
 
void setup() {
  Serial.begin(115200);
 
  lcd.init();                  // Inicializa o display
  lcd.begin(16, 2);            // Inicializa todos os caracteres
  lcd.backlight();             // Inicializa o backlight
}
 
 
void loop() {
  
  for (int i = 0; i < 20; i++) {  
    buf[i] = analogRead(Sensor_PH);      
    delay(10);
  }

  int valorMedio = 0;
  
  for (int i = 0; i < 20; i++) {  // Realiza o valor médio utilizando 6 amostras
    valorMedio += buf[i];
  }
 
  double tensao = (valorMedio * 3.3) / 4096.0 / 20;  

  if (tensao<=2.55) {
    m = (7.0 - 10.0) / (calibracao_ph7 - calibracao_ph10);
    b = 10.0 - m * calibracao_ph10;
    Serial.println("calibração para PH 10");
  } else {
    m = (4.0 - 7.0) / (calibracao_ph4 - calibracao_ph7);
    b = 7.0 - m * calibracao_ph7;
    Serial.println("calibração para PH 4");
  }

  
  float ph = m * tensao + b; // Converte para pH
  Serial.println(ph);
 
  lcd.setCursor(0, 0);      // Posiciona o cursor no display
  lcd.print("Valor pH: ");  // Escreve no display
  lcd.setCursor(11, 0);     // Posiciona o cursor no display
  lcd.print(ph, 1);         // Escreve o pH com uma casa decimal
  delay(4000);              // Aguarda para próxima leitura
}
