// пример работы с двумя датчиками без адресации
// один датчик - один пин

#include <microDS18B20.h>
 // Датчики на D2 и D3
MicroDS18B20<2> sensor1;
MicroDS18B20<3> sensor2;

void setup() {
  Serial.begin(9600);
  //sensor1.setResolution(12);  // разрешение [9-12] бит. По умолч. 12 
}

void loop() {
  // запрос температуры  
  sensor1.requestTemp();
  sensor2.requestTemp();
  
  // вместо delay используй таймер на millis()
  delay(1000);  
  
  // вывод
  Serial.print("t: ");
  Serial.print(sensor1.getTemp());
  Serial.print(", ");
  Serial.println(sensor2.getTemp());
}

/*
  Опрос датчиков асинхронный, т.е. не блокирует выполнение кода, но
  между requestTemp и getTemp должно пройти не менее
  
  точность | время
  12 бит   | 750 мс
  11 бит   | 375 мс
  10 бит   | 187 мс
  9 бит    | 93 мс
  
  Иначе датчик вернёт предыдущее значение
*/
