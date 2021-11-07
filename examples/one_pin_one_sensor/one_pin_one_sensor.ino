// пример работы с двумя датчиками без адресации
// один датчик - один пин

#include <microDS18B20.h>
// Датчики на D2 и D3
MicroDS18B20<2> sensor1;
MicroDS18B20<3> sensor2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // запрос температуры
  sensor1.requestTemp();
  sensor2.requestTemp();

  // вместо delay используй таймер на millis(), пример async_read
  delay(1000);

  // ПЕРВЫЙ ДАТЧИК
  Serial.print("t1: ");
  
  // просто выводим температуру первого датчика
  Serial.print(sensor1.getTemp());

  // ВТОРОЙ ДАТЧИК
  Serial.print(", t2: ");
  
  // проверяем успешность чтения и выводим
  if (sensor2.readTemp()) Serial.println(sensor2.getTemp());
  else Serial.println("error");
}
