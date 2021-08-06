![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# microDS18B20
Легкая и простая в обращении библиотека для работы с 1-Wire термометрами DS18B20
- Возможность работы с несколькими датчиками на одном пине при использовании 8-байтного уникального адреса
- Возможность работы с одним датчиком на пине без использования адресации
- Настраиваемое разрешение преобразования
- Чтение сырых данных (без float) для случаев сильной экономии памяти
- Проверка подлинности данных "на лету", с использованием CRC8 от Dallas [#define DS_CHECK_CRC]
- Расчет CRC8 (~6 мкс) или чтение из таблицы (<1 мкс + 256 байт flash) [#define DS_CRC_USE_TABLE]
- Расчет температуры как в целых числах, так и с плавающей точкой [#define DS_TEMP_TYPE float / int]
- Чтение уникального адреса подключенного термометра в указанный массив типа byte

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **microDS18B20** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/microDS18B20/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
MicroDS18B20<uint8_t pin> ds;                   // один датчик на одном пине без адресации
MicroDS18B20<uint8_t pin, uint8_t *address> ds; // несколько датчиков на одном пине с адресацией
MicroDS18B20<uint8_t pin, DS_ADDR_MODE>;        // говорим, что датчик с адресацией, но не указываем адрес
```

<a id="usage"></a>
## Использование
```cpp
// Дефайны настроек (перед подключением библиотеки)
#define DS_TEMP_TYPE [float / int]          // Тип данных для температуры (точность / экономия flash) (По умолч. float)
#define DS_CHECK_CRC [true / false]         // Проверка подлинности принятых данных (По умолч. true)
#define DS_CRC_USE_TABLE [true / false]     // Использовать таблицу для CRC. Быстрее, но +256 байт flash (<1мкс VS ~6мкс) (По умолч. false)

// Методы
void setAddress(uint8_t *addr);             // установить (сменить) адрес
void setResolution(uint8_t resolution);     // Установить разрешение термометра 9-12 бит
void readAddress(uint8_t *addressArray);    // Прочитать уникальный адрес термометра в массив
void requestTemp(void);                     // Запросить новое преобразование температуры
uint16_t getRaw(void)                       // Прочитать "сырое" значение температуры
[int/float] getTemp(void);                  // Прочитать значение температуры
[int/float] calcRaw(uint16_t data);         // Преобразовать "сырое" значение в температуру
[int/float] - настраивается дефайном. По умолчанию float

// внешние функции
int DS_rawToInt(uint16_t data);             // преобразовать raw данные в температуру int
float DS_rawToFloat(uint16_t data);         // преобразовать raw данные в температуру float

// Время преобразования от точности
точность | время
12 бит   | 750 мс
11 бит   | 375 мс
10 бит   | 187 мс
9 бит    | 93 мс
```

<a id="example"></a>
## Подключение
![scheme](/doc/scheme.png)
P.S. Вместо резистора на 4.7к можно использовать параллельно два по 10к =)

Остальные примеры смотри в **examples**!
## Один датчик без адресации
```cpp
// один датчик лучше читать без адресации - 
// это сильно экономит память!
#include <microDS18B20.h>
MicroDS18B20<2> sensor1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // запрос температуры  
  sensor1.requestTemp();
  
  // вместо delay используй таймер на millis()
  delay(1000);
  Serial.print("t: ");
  Serial.println(sensor1.getTemp());
}
```

## Несколько датчиков без адресации
```cpp
// 2 и более датчиков НЕВЫГОДНО использовать в таком режиме! Но можно

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
  
  // вместо delay используй таймер на millis()
  delay(1000);  
  
  // вывод
  Serial.print("t: ");
  Serial.print(sensor1.getTemp());
  Serial.print(", ");
  Serial.println(sensor2.getTemp());
}
```

## Несколько датчиков с адресацией
```cpp
// 2 и более датчиков выгоднее использовать с адресацией
// на одном пине
#include <microDS18B20.h>
#define DS_PIN 2 // пин для термометров

// Уникальные адреса датчиков - считать можно в примере address_read
uint8_t s1_addr[] = {0x28, 0xE1, 0x57, 0xCF, 0x4E, 0x20, 0x1, 0x3D};
uint8_t s2_addr[] = {0x28, 0xFF, 0x36, 0x94, 0x65, 0x15, 0x2, 0x80};

MicroDS18B20<DS_PIN, s1_addr> sensor1;  // Создаем термометр с адресацией
MicroDS18B20<DS_PIN, s2_addr> sensor2;  // Создаем термометр с адресацией

void setup() {
  Serial.begin(9600);
}

void loop() {
  // датчик 1
  sensor1.requestTemp();      // Запрашиваем преобразование температуры
  delay(1000);                // Ожидаем окончания преобразования
  Serial.print("Sensor1 temp: ");
  Serial.print(sensor1.getTemp());  // Выводим температуру
  Serial.println(" *C");

  // датчик 2
  sensor2.requestTemp();
  delay(1000);
  Serial.print("Sensor2 temp: ");
  Serial.print(sensor2.getTemp());
  Serial.println(" *C");
}
```

## Чтение адреса
```cpp
#include <microDS18B20.h>
// на пин подключен только один датчик!
MicroDS18B20 <2> sensor;  // Создаем термометр без адреса на пине D2
uint8_t addressBuf[8];    // Создаем массив для адреса

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor.readAddress(addressBuf);

  // выводим в порт
  Serial.print("Address: {");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print("0x");
    Serial.print(addressBuf[i], HEX);
    if (i < 7) Serial.print(", ");
  }
  Serial.println("};");

  delay(1000);
}
```

<a id="versions"></a>
## Версии
- v3.0 - Библиотека переехала на шаблон! Старые примеры НЕСОВМЕСТИМЫ. Оптимизация, новые трюки.
- v3.0.1 - добавлен пример
- v3.1 - добавлена возможность смены адреса на лету

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!