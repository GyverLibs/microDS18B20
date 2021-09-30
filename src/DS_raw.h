#ifndef _DS_raw_h
#define _DS_raw_h
#include <Arduino.h>
int DS_rawToInt(uint16_t data);         // преобразовать raw данные в температуру int
float DS_rawToFloat(uint16_t data);     // преобразовать raw данные в температуру float

#endif