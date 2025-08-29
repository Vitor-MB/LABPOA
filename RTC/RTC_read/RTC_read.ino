#include <DS1307.h>

uint8_t sec, minute, hour, day, month;
uint16_t year;
DS1307 rtc;

void setup(void){
  Serial.begin(115200); 
  rtc.begin();
  rtc.start();         
  delay(1000);          
}

void loop(void){
  
  rtc.get(&sec, &minute, &hour, &day, &month, &year);

  Serial.print("Hora Informada: ");
  Serial.print("\nHora: ");
  Serial.printf("%02d", hour);
  Serial.print(":");
  Serial.printf("%02d", minute);
  Serial.print(":");
  Serial.printf("%02d", sec);

  Serial.print("\nData: ");
  Serial.printf("%02d", day);
  Serial.print("/");
  Serial.printf("%02d", month);
  Serial.print("/");
  Serial.printf("%02d", year);
  Serial.println("");
  
  delay(1000);
}
