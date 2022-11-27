#include <microDS18B20.h>

// на пин подключен только один датчик!
MicroDS18B20 <2> sensor;  // Создаем термометр без адреса на пине 2

void setup() {
  Serial.begin(9600);
  uint8_t res = sensor.getResolution();  // Чтение разрешенa датчикa
  Serial.print("Resolution: ");
  Serial.println(res);  
}

void loop() {
}
