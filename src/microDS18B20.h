/*
    Легкая и простая в обращении библиотека для работы с 1-Wire термометрами DS18B20
    Документация:
    GitHub: https://github.com/GyverLibs/microDS18B20
    Возможности:
    - Работа с несколькими датчиками на одном пине (режим адресации)
    - Работа с одним датчиком на пине (без использования адресации)
    - Расчет температуры в целых числах и с плавающей точкой
    - Проверка корректности полученной температуры
    - Настраиваемое разрешение преобразования
    - Чтение сырых данных для случаев сильной экономии памяти
    - Проверка подлинности данных "на лету", с использованием CRC8 от Dallas
    - Расчет CRC8 (~6 мкс) или чтение из таблицы (<1 мкс + 256 байт flash)
    
    Egor 'Nich1con' Zakharov for AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v3.0 - Библиотека переехала на шаблон! Старые примеры НЕСОВМЕСТИМЫ. Оптимизация, новые трюки.
    v3.1 - добавлена возможность смены адреса на лету
    v3.1.1 - microOneWire разбит на .h и .cpp
    v3.2 - исправлены отрицательные температуры
    v3.3 - разбил на файлы
    v3.4 - добавлена проверка онлайна датчика и буфер, при ошибке чтения возвращается последнее прочитанное значение
    v3.5 - оптимизация, повышение стабильности, проверка правильности чтения, online() работает с адресацией, добавлен метод getTempInt() и readTemp(), упразднён DS_TEMP_TYPE
    v3.6 - исправлена ошибка компиляции, добавлена поддержка GyverCore (спасибо ArtemiyKolobov)
*/

#ifndef _microDS18B20_h
#define _microDS18B20_h
#include <Arduino.h>
#include "microOneWire.h"
#include "DS_raw.h"

#ifndef DS_CHECK_CRC
#define DS_CHECK_CRC true		  // true/false - проверка контрольной суммы принятых данных - надежнее, но тратит немного больше flash
#endif

#ifndef DS_CRC_USE_TABLE
#define DS_CRC_USE_TABLE false    // true/false - использовать готовую таблицу контрольной суммы - значительно быстрее, +256 байт flash
#endif

/*
Время исполнения функций для работы с датчиком (частота ядра - 16 МГц):
_________________________________________________________________________________
| Датчик без адресации (один на линии) | Датчик с адресацией (несколько на линии) |
|______________________________________|__________________________________________|
| .setResolution(...) ~ 3591.125 мкс   | .setResolution(...) ~ 8276.0625 мкс 	  |
| .requestTemp() ~ 1839.9375 мкс	   | .requestTemp() ~ 6522.1875 мкс 		  |
|______________________________________|__________________________________________|
|							С использованием CRC8								  |
|_________________________________________________________________________________|
| .readAddress(...) ~ 6467.3125 мкс	   | float .getTemp() ~ 12250.25 мкс	 	  | 
| float .getTemp() ~ 7620.25 мкс	   | int .getTemp() ~ 12250.2500 мкс 		  |	
| int .getTemp() ~ 7574.0625 мкс	   | 									      |
|______________________________________|__________________________________________|
|							Без использования CRC8								  |
|_________________________________________________________________________________|
| .readAddress(...) ~ 6394.3125 мкс    | float .getTemp() ~ 7809.1250 мкс	 	  |
| float .getTemp() ~ 3132.9375 мкс	   | int .getTemp() ~ 7809.1250 мкс	 		  |
| int .getTemp() ~ 3132.9375 мкс	   | 								  		  |
|______________________________________|__________________________________________|
*/

// ====================== CRC TABLE ======================
#if (DS_CRC_USE_TABLE == true)
static const uint8_t PROGMEM _ds_crc8_table[] = {
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35
};
#endif

static uint8_t _empDsAddr[1] = {1};
#define DS_ADDR_MODE _empDsAddr

// ====================== CLASS ======================
template <uint8_t DS_PIN, uint8_t *DS_ADDR = (uint8_t*)nullptr>
class MicroDS18B20 {
public:
    MicroDS18B20() {
        pinMode(DS_PIN, INPUT);
        digitalWrite(DS_PIN, LOW);
    }

    // Установить разрешение термометра 9-12 бит
    void setResolution(uint8_t res) {
        if (oneWire_reset(DS_PIN)) return;          // Проверка присутствия
        addressRoutine();                           // Процедура адресации
        oneWire_write(0x4E, DS_PIN);                // Запись RAM
        oneWire_write(0xFF, DS_PIN);                // Максимум в верхний регистр тревоги
        oneWire_write(0x00, DS_PIN);                // Минимум в верхний регистр тревоги
        oneWire_write(((constrain(res, 9, 12) - 9) << 5) | 0x1F, DS_PIN); // Запись конфигурации разрешения
    }
    
    // установить адрес
    void setAddress(uint8_t *addr) {
        _addr = addr;
    }
    
    // Прочитать уникальный адрес термометра в массив
    bool readAddress(uint8_t *addr) {
        if (oneWire_reset(DS_PIN)) return 0;        // Проверка присутствия
        oneWire_write(0x33, DS_PIN);                // Запрос адреса
        int16_t sum = 0;                            // контрольная сумма
        crc = 0;                                    // обнуляем crc
        for (uint8_t i = 0; i < 8; i++) {           // Прочитать 8 байт адреса
            addr[i] = oneWire_read(DS_PIN);         // Записать в массив
            sum += addr[i];                         // контрольная сумма
            _ds_crc8_upd(crc, addr[i]);             // Обновить значение CRC8
        }
        if (crc || !sum) return 0;                  // CRC не сошелся или адрес нулевой - ошибка
        return 1;                                   // всё ок
    }
    
    // Запрос температуры
    void requestTemp() {
        state = 0;                                  // запрошена новая температура
        if (oneWire_reset(DS_PIN)) return;          // Проверка присутствия
        addressRoutine();                           // Процедура адресации
        oneWire_write(0x44, DS_PIN);                // Запросить преобразование
    }
    
    // получить температуру float
    float getTemp() {
        if (!state) readTemp();
        return (_buf / 16.0);
    }
    
    // получить температуру int
    int16_t getTempInt() {
        if (!state) readTemp();
        return (_buf >> 4);
    }
    
    // получить "сырое" значение температуры
    int16_t getRaw() {
        if (!state) readTemp();
        return _buf;
    }
    
    // прочитать температуру с датчика. true если успешно
    bool readTemp() {
        state = 1;
        if (oneWire_reset(DS_PIN)) return 0;        // датчик оффлайн
        addressRoutine();                   		// Процедура адресации
        oneWire_write(0xBE, DS_PIN);                // Запросить температуру
        crc = 0;                                    // обнуляем crc
        int16_t temp;                               // переменная для расчёта температуры
        int16_t sum = 0;                            // контрольная сумма
        uint8_t data[9];                            // Временный массив для данных (9 байт)
        for (uint8_t i = 0; i < 9; i++) {           // Считать RAM
            data[i] = oneWire_read(DS_PIN);         // Прочитать данные
            sum += data[i];
            _ds_crc8_upd(crc, data[i]);             // Обновить значение CRC8
        }
        temp = (int16_t)(data[1] << 8) | data[0];
        if (temp == 0xFFFF || sum == 0 || crc) return 0;   // датчик оффлайн или данные повреждены        
        if (temp != 0x0550) _buf = temp;            // пропускаем первое чтение (85 градусов)
        return 1;
    }

    // проверить связь с датчиком (true - датчик на линии). ЛИНИЯ ДОЛЖНА БЫТЬ ПОДТЯНУТА
    bool online() {
        if (DS_ADDR) {
            oneWire_reset(DS_PIN);
            addressRoutine();
            oneWire_write(0xBE, DS_PIN);
            return !(oneWire_read(DS_PIN) == 0xFF && oneWire_read(DS_PIN) == 0xFF);   // вернул 0xFFFF
        } else return !oneWire_reset(DS_PIN);
    }

private:
    uint8_t crc = 0;
    bool state = 0;
    uint8_t *_addr = DS_ADDR;
    int16_t _buf = 0;
    
    void addressRoutine() {                   	    // Процедура адресации
        if (DS_ADDR) {                        		// Адрес определен?
            oneWire_write(0x55, DS_PIN);            // Говорим термометрам слушать адрес
            for (uint8_t i = 0; i < 8; i++) {       // Отправляем 8 байт уникального адреса
                oneWire_write(_addr[i], DS_PIN);
            }
        } else oneWire_write(0xCC, DS_PIN);         // Адреса нет - пропускаем адресацию на линии
    }

    void _ds_crc8_upd(uint8_t &crc, uint8_t data) {
#if (DS_CHECK_CRC == true)                                  // разрешено
#if (DS_CRC_USE_TABLE == true)                              // Используем таблицу?
        crc = pgm_read_byte(&_ds_crc8_table[crc ^ data]);   // Тогда берем готовое значение
#else                                                       // считаем вручную
#ifdef __AVR__
        // резкий алгоритм для AVR
        uint8_t counter;
        uint8_t buffer;
        asm volatile (
        "EOR %[crc_out], %[data_in] \n\t"
        "LDI %[counter], 8          \n\t"
        "LDI %[buffer], 0x8C        \n\t"
        "_loop_start_%=:            \n\t"
        "LSR %[crc_out]             \n\t"
        "BRCC _loop_end_%=          \n\t"
        "EOR %[crc_out], %[buffer]  \n\t"
        "_loop_end_%=:              \n\t"
        "DEC %[counter]             \n\t"
        "BRNE _loop_start_%="
        : [crc_out]"=r" (crc), [counter]"=d" (counter), [buffer]"=d" (buffer)
        : [crc_in]"0" (crc), [data_in]"r" (data)
        );
#else
        // обычный для всех остальных
        uint8_t i = 8;
        while (i--) {
            crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
            data >>= 1;
        }
#endif
#endif
#endif
    }
};
#endif