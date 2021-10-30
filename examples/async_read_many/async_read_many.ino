// пример компактного асинхронного опроса датчиков на программном таймере
// https://alexgyver.ru/lessons/time/

// количество датчиков для удобства
#define DS_SENSOR_AMOUNT 5

// создаём двухмерный массив с адресами
uint8_t addr[][8] = {
  {0x28, 0xFF, 0x78, 0x5B, 0x50, 0x17, 0x4, 0xCF},
  {0x28, 0xFF, 0x99, 0x80, 0x50, 0x17, 0x4, 0x4D},
  {0x28, 0xFF, 0x53, 0xE5, 0x50, 0x17, 0x4, 0xC3},
  {0x28, 0xFF, 0x42, 0x5A, 0x51, 0x17, 0x4, 0xD2},
  {0x28, 0xFF, 0xCD, 0x59, 0x51, 0x17, 0x4, 0xFE},
};

#include <microDS18B20.h>
// указываем DS_ADDR_MODE для подключения блока адресации
// и создаём массив датчиков на пине D2
MicroDS18B20<2, DS_ADDR_MODE> sensor[DS_SENSOR_AMOUNT];

void setup() {
  Serial.begin(9600);
  // устанавливаем адреса
  for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
    sensor[i].setAddress(addr[i]);
  }
}

void loop() {
  // конструкция программного таймера на 1c
  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();

    // выводим показания в порт
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
      Serial.print(sensor[i].getTemp());
      Serial.print(',');
    }
    Serial.println();

    // запрашиваем новые
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
      sensor[i].requestTemp();
    }
  }
}
