#include "microOneWire.h"
#ifdef __AVR__
#define MOW_CLI() uint8_t oldSreg = SREG; cli();
#define MOW_SEI() SREG = oldSreg
#else
#define MOW_CLI()
#define MOW_SEI()
#endif

bool oneWire_reset(uint8_t pin) { 
    pinMode(pin, OUTPUT);
    delayMicroseconds(600);
    pinMode(pin, INPUT);
    MOW_CLI();
    delayMicroseconds(60);
    bool pulse = digitalRead(pin);
    MOW_SEI();
    delayMicroseconds(600);
    return !pulse;
}

void oneWire_write(uint8_t data, uint8_t pin) {
    for (uint8_t i = 8; i; i--) {
        pinMode(pin, OUTPUT);
        MOW_CLI();
        if (data & 1) {
            delayMicroseconds(5);
            pinMode(pin, INPUT);
            delayMicroseconds(60);
        } else {
            delayMicroseconds(60);
            pinMode(pin, INPUT);
            delayMicroseconds(5);
        }
        MOW_SEI();
        data >>= 1;
    }
}

uint8_t oneWire_read(uint8_t pin) {
    uint8_t data = 0;
    for (uint8_t i = 8; i; i--) {
        data >>= 1;
        MOW_CLI();
        pinMode(pin, OUTPUT);
        delayMicroseconds(2);
        pinMode(pin, INPUT);
        delayMicroseconds(8);
        if (digitalRead(pin)) data |= (1 << 7);
        delayMicroseconds(60);
        MOW_SEI();
    }
    return data;
}