#include <DS1307.h>

uint8_t set_Sec = 0;           
uint8_t set_Minute = 19;        
uint8_t set_Hour = 3;          
uint8_t set_Day = 29;           
uint8_t set_Month = 8;         
uint16_t set_Year = 2025;     

uint8_t sec, minute, hour, day, month;
uint16_t year;

DS1307 rtc;

void setup(void){
  Serial.begin(115200);       
  rtc.begin();
  
  rtc.set(set_Sec, set_Minute, set_Hour, set_Day, set_Month, set_Year);
  rtc.stop();                 

  rtc.start();                
  delay(1000);                
  Serial.print("Hora Informada: ");
  Serial.print("\nHora: ");
  Serial.printf("%02d", set_Hour);
  Serial.print(":");
  Serial.printf("%02d", set_Minute);
  Serial.print(":");
  Serial.printf("%02d", set_Sec);

  Serial.print("\nData: ");
  Serial.printf("%02d", set_Day);
  Serial.print("/");
  Serial.printf("%02d", set_Month);
  Serial.print("/");
  Serial.printf("%02d", set_Year);
  Serial.println("");
}

void loop(void){}
