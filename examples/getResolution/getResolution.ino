#include <microDS18B20.h>

// на пин подключен только один датчик!
MicroDS18B20 <2> sensor;  // Создаем термометр без адреса на пине 2

void setup() {
  Serial.begin(9600);
  
  // read sensor resolution
  sensor.requestResolution();  
  Serial.print("Sensor resolution: ");
  Serial.print(sensor.getResolution());
  
  // read temperature
  sensor.requestTemp();
  delay(1000);
  Serial.print(",  Temp: ");
  Serial.println(sensor.getTemp());  
}

void loop() {
}
