// работа в режиме шины - несколько датчиков на линии, один объект
// количество датчиков для удобства
#define SENS_AMOUNT 5

// создаём двухмерный массив с адресами
const uint8_t addr[][8] PROGMEM = {
  {0x28, 0xFF, 0x78, 0x5B, 0x50, 0x17, 0x4, 0xCF},
  {0x28, 0xFF, 0x99, 0x80, 0x50, 0x17, 0x4, 0x4D},
  {0x28, 0xFF, 0x53, 0xE5, 0x50, 0x17, 0x4, 0xC3},
  {0x28, 0xFF, 0x42, 0x5A, 0x51, 0x17, 0x4, 0xD2},
  {0x28, 0xFF, 0xCD, 0x59, 0x51, 0x17, 0x4, 0xFE},
};

#include <microDS18B20.h>
MicroDS18B20<D2, DS_ADDR_MODE, SENS_AMOUNT, DS_PROGMEM> sensors;

void setup() {
  Serial.begin(9600);
  // устанавливаем адреса
  sensors.setAddress((uint8_t*)addr);
  
  // Установить разрешение 9-12 бит у всех датчиков на линии
  //sensors.setResolutionAll(10);
}

void loop() {
  // конструкция программного таймера на 1c
  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();

    // выводим показания в порт
    for (int i = 0; i < SENS_AMOUNT; i++) {
      Serial.print(sensors.getTemp(i));
      Serial.print(',');
    }
    Serial.println();

    // запрашиваем новые
    sensors.requestTempAll();
  }
}
