
int buf[20];
 
 
void setup() {
  Serial.begin(9600);
  Serial.println("Usinainfo");
  delay(500);
}
 
 
 
void loop() {
  for (int i = 0; i < 20; i++) {     // 11 amostras
    buf[i] = analogRead(15);  // Ler o sensor PH
    
  }
 
 
  int valorMedio = 0;
  for (int i = 0; i < 20; i++) {  // Realiza o valor médio utilizando 6 amostras
    valorMedio += buf[i];
  }
 
 
  float tensao = (valorMedio * 3.3) / 4095.0 / 20;
  Serial.println(tensao);
  delay(450);
}
