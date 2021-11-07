/*
   Пример нескольких термометров на одном пине с использованием адресации
   Использование адресации для нескольких датчиков значительно экономит память
*/

#include <microDS18B20.h>
#define DS_PIN 2 // пин для термометров

// Уникальные адреса датчиков - считать можно в примере address_read
uint8_t s1_addr[] = {0x28, 0xFF, 0xCD, 0x59, 0x51, 0x17, 0x4, 0xFE};
uint8_t s2_addr[] = {0x28, 0xFF, 0x36, 0x94, 0x65, 0x15, 0x2, 0x80};

MicroDS18B20<DS_PIN, s1_addr> sensor1;  // Создаем термометр с адресацией
MicroDS18B20<DS_PIN, s2_addr> sensor2;  // Создаем термометр с адресацией

void setup() {
  Serial.begin(9600);
}

void loop() {
  // асинхронное чтение нескольких датчиков смотри в примере async_read_many
  sensor1.requestTemp();      // Запрашиваем преобразование температуры
  sensor2.requestTemp();

  delay(1000);           // ожидаем результат 
  
  Serial.print("t1: ");
  if (sensor1.readTemp()) Serial.println(sensor1.getTemp());
  else Serial.println("error");

  Serial.print("t2: ");
  if (sensor2.readTemp()) Serial.println(sensor2.getTemp());
  else Serial.println("error");
}
