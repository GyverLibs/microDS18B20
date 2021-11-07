#ifndef _DS_raw_h
#define _DS_raw_h
#include <Arduino.h>
int DS_rawToInt(int16_t data);         // преобразовать raw данные в температуру int
float DS_rawToFloat(int16_t data);     // преобразовать raw данные в температуру float

#endif