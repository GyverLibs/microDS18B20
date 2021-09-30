#include "DS_raw.h"

int DS_rawToInt(int16_t data) {
    return (data / 16);
}
float DS_rawToFloat(int16_t data) {
    return (data / 16.0);
}