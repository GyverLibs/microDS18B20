// пример с чтением сырого 16 бит значения
// и расшифровкой его в температуру
// может быть использовано для чтения и передачи

#include <microDS18B20.h>
 // Датчик на D2
MicroDS18B20<2> sensor;

void setup() {
  Serial.begin(9600);  
  //sensor.setResolution(12);  // разрешение [9-12] бит. По умолч. 12 
}

void loop() {
  sensor.requestTemp();
  delay(1000);  
  // прпочитали сырое значение
  uint16_t rawVal = sensor.getRaw();
  Serial.print(rawVal);  
  Serial.print(" ");

  // преобразовали
  Serial.println(DS_rawToFloat(rawVal));
}
