// v1.2 от 26.04.2020. Повышена стабильность

#ifndef microOneWire_h
#define microOneWire_h
#include <Arduino.h>

bool oneWire_reset(uint8_t pin);
void oneWire_write(uint8_t data, uint8_t pin);
uint8_t oneWire_read(uint8_t pin);

//==========================================
/* ~10722CK ~670.1250 us (16MHZ) */
/* max time without interrupts ~90us*/
bool oneWire_reset(uint8_t pin) { 
    pinMode(pin, 1);
    delayMicroseconds(520);
    #ifdef AVR
    uint8_t oldSreg = SREG;
    cli();
    #endif
    pinMode(pin, 0);
    delayMicroseconds(2);
    for (uint8_t c = 80; c; c--) {
        if (!digitalRead(pin)) {
            #ifdef AVR
            SREG = oldSreg;
            #endif
            for (uint8_t i = 200;  !digitalRead(pin) and i; i--) {
                delayMicroseconds(1);
            }
            return false;
        }
        delayMicroseconds(1);
    }
    #ifdef AVR
    SREG = oldSreg;
    #endif
    return true;
}


/* ~9150CK ~571.8750 us (16MHZ) */
/* max time without interrupts ~70us*/
void oneWire_write(uint8_t data, uint8_t pin) {
    for (uint8_t p = 8; p; p--) {
        #ifdef AVR
        uint8_t oldSreg = SREG;
        cli();
        #endif
        pinMode(pin, 1);
        if (data & 1) {
            delayMicroseconds(5);
            pinMode(pin, 0);
            delayMicroseconds(60);
        } else {
            delayMicroseconds(60);
            pinMode(pin, 0);
            delayMicroseconds(5);
        }
        #ifdef AVR
        SREG = oldSreg;
        #endif
        data >>= 1;
    }
}

/* ~9500CK ~593.75 us (16MHZ) */
/* max time without interrupts ~70us*/
uint8_t oneWire_read(uint8_t pin) {
    uint8_t data = 0;
    for (uint8_t p = 8; p; p--) {
        data >>= 1;
        #ifdef AVR
        uint8_t oldSreg = SREG;
        cli();
        #endif
        pinMode(pin, 1);
        delayMicroseconds(2);
        pinMode(pin, 0);
        delayMicroseconds(8);
        if (digitalRead(pin)) data |= 1 << 7;
        delayMicroseconds(60);
        #ifdef AVR
        SREG = oldSreg;
        #endif
    }
    return data;
}
#endif