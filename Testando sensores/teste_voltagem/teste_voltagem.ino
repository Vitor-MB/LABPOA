// Define analog input
#define ANALOG_IN_PIN 15
 
// Floats for ADC voltage & Input voltage
double adc_voltage = 0.0;
double in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
double R1 = 30000.0;
double R2 = 7500.0; 
 
// Float for Reference Voltage
double ref_voltage = 3.8;
 
// Integer for ADC value
int adc_value = 0;
 
void setup(){
  // Setup Serial Monitor
  Serial.begin(115200);
}
 
void loop(){
  // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);
  
  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 4096.0;
  
  in_voltage = adc_voltage*(R1+R2)/R2;
  
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);
  
 
  delay(500);
}
