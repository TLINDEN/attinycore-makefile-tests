#ifndef DIGITAL_H
#define DIGITAL_H

#define INPUT  0
#define OUTPUT 1
#define LOW    0
#define HIGH   1

#include <avr/io.h>

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);


#endif
