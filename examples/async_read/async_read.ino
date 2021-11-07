// пример асинхронного опроса датчика на программном таймере
// https://alexgyver.ru/lessons/time/
// один датчик - один пин

#include <microDS18B20.h>
MicroDS18B20<2> sensor;  // датчик на D2

void setup() {
  Serial.begin(9600);
}

void loop() {
  // конструкция программного таймера на 800 мс
  static uint32_t tmr;
  if (millis() - tmr >= 800) {
    tmr = millis();

    // читаем прошлое значение
    if (sensor.readTemp()) Serial.println(sensor.getTemp());
    else Serial.println("error");

    // запрашиваем новое измерение
    sensor.requestTemp();
  }
}
