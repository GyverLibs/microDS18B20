#include "microOneWire.h"
#ifdef __AVR__
#define MOW_CLI() uint8_t oldSreg = SREG; cli();
#define MOW_SEI() SREG = oldSreg
#else
#define MOW_CLI()
#define MOW_SEI()
#endif

/* ~10722CK ~670.1250 us (16MHZ) */
/* max time without interrupts ~90us*/
bool oneWire_reset(uint8_t pin) { 
    pinMode(pin, 1);
    delayMicroseconds(520);
    MOW_CLI();
    pinMode(pin, 0);
    delayMicroseconds(2);
    for (uint8_t c = 255; c; c--) {
        if (!digitalRead(pin)) {
            MOW_SEI();
            for (uint8_t i = 255; !digitalRead(pin) && i; i--) delayMicroseconds(1);            
            return false;
        }
        delayMicroseconds(1);
    }
    MOW_SEI();
    return true;
}


/* ~9150CK ~571.8750 us (16MHZ) */
/* max time without interrupts ~70us*/
void oneWire_write(uint8_t data, uint8_t pin) {
    for (uint8_t p = 8; p; p--) {        
        pinMode(pin, 1);
		MOW_CLI();
        if (data & 1) {
            delayMicroseconds(5);
            pinMode(pin, 0);
            delayMicroseconds(60);
        } else {
            delayMicroseconds(60);
            pinMode(pin, 0);
            delayMicroseconds(5);
        }
        MOW_SEI();
        data >>= 1;
    }
}

/* ~9500CK ~593.75 us (16MHZ) */
/* max time without interrupts ~70us*/
uint8_t oneWire_read(uint8_t pin) {
    uint8_t data = 0;
    for (uint8_t p = 8; p; p--) {
        data >>= 1;
        MOW_CLI();
        pinMode(pin, 1);
        delayMicroseconds(2);
        pinMode(pin, 0);
        delayMicroseconds(8);
        if (digitalRead(pin)) data |= 1 << 7;
        delayMicroseconds(60);
        MOW_SEI();
    }
    return data;
}