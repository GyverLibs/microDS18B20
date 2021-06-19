#include <microDS18B20.h>
/*
   Чтение уникального адреса термометра для последующего использования
   ВНИМАНИЕ! Устанавливать не более одного датчика на линию (пин) во избежание конфликта
*/

MicroDS18B20 <2> sensor;  // Создаем термометр без адреса на пине D2
uint8_t addressBuf[8];    // Создаем массив для адреса

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Читаем адрес термометра в наш массив
  sensor.readAddress(addressBuf);

  // выводим в порт
  Serial.print("Address: {");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    Serial.print(addressBuf[i], HEX);  // Выводим адрес для копирования
    if (i < 7) Serial.print(", ");
  }
  Serial.println("};");

  delay(1000);
}
