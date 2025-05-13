#include <Wire.h> // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 

#define col 16
#define lin  2 
#define ende  0x3F 

LiquidCrystal_I2C lcd(0x27,16,2); 

void setup() 
{  
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear(); 
}
void loop() 
{
  lcd.setCursor(1,0); 
  lcd.print("Testando "); 
  lcd.setCursor(1, 1);
  lcd.print("LABPOA");  
  lcd.setCursor(1,2); 
  delay(3000); 
  lcd.clear(); 
}
