/*
   Чтение уникального адреса термометра для последующего использования
   ВНИМАНИЕ! Устанавливать не более одного датчика на линию (пин)
*/
#include <microDS18B20.h>

MicroDS18B20 <2> sensor;  // Создаем термометр без адреса на пине D2
uint8_t address[8];       // Создаем массив для адреса

void setup() {
  Serial.begin(9600);
}

void loop() {
  // читаем адрес термометра в указанный массив
  if (sensor.readAddress(address)) {  // если успешно, выводим
    Serial.print('{');
    for (uint8_t i = 0; i < 8; i++) {
      Serial.print("0x");
      Serial.print(address[i], HEX);  // Выводим адрес
      if (i < 7) Serial.print(", ");
    }
    Serial.println('}');

  } else Serial.println("Not connected");
  delay(1000);
}
