#include "digital.h"
#include <avr/interrupt.h>

void pinMode(uint8_t pin, uint8_t mode) {
  uint8_t sreg_local = SREG;
  cli();
  if(mode) {
    DDRB |= _BV(pin);
  }
  else {
    DDRB &= ~_BV(pin);
  }
  SREG = sreg_local;
}

void digitalWrite(uint8_t pin, uint8_t val) {
  uint8_t sreg_local = SREG;
  cli();
  if(val) {
    PORTB |= _BV(pin);
  }
  else {
    PORTB &= ~_BV(pin);
  }
  SREG = sreg_local;
}

int digitalRead(uint8_t pin) {
  if ( PINB & _BV(pin) ) {
    return 1;
  }
  else {
    return 0;
  }
}
