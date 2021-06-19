/*
   Пример с максимальной экономией flash при использовании библиотеки
   Если датчик один - не используйте адресацию
   Если датчиков несколько - рекомендуется использование адресации и ОДНОЙ линии для экономии flash
*/

#define DS_TEMP_TYPE int   // целочисленный тип данных для температуры
#define DS_CHECK_CRC false // отключить проверку подлинности принятых данных

#include <microDS18B20.h>

// Датчик один - не используем адресацию
#define DS_PIN 2
MicroDS18B20 <DS_PIN> sensor; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor.requestTemp();
  delay(1000);
  Serial.print("t: ");
  Serial.print(sensor.getTemp());
  Serial.println(" *C");
}
