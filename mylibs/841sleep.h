/*
  Macros for easier sleep mode management
  For Attiny841 only
*/

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "841vcc.h"

#ifndef SLEEPMODE_8441_H
#define SLEEPMODE_8441_H

#ifndef SLEEPMODE
  #define SLEEPMODE SLEEP_MODE_PWR_DOWN
#endif

#ifndef SLEEP_DURATION_BITS
#define SLEEP_DURATION_BITS (_BV(WDP0) | _BV(WDP3)) // 8s
#endif

// define sleep mode
// disable analog comparator 0 and 1
// disable SPI
// unlock Watchdog configuration register
// enable the WD interrupt, set watchdog timeout prescaler value (interrupt period)
// unlock clock configuration change
// change clock prescaler to 8
#define sleep_setup()                        \
  set_sleep_mode(SLEEPMODE);                 \
  ACSR0A |= _BV(ACD0);                       \
  ACSR1A |= _BV(ACD1);                       \
  CCP = 0xD8;                                \
  WDTCSR = _BV(WDIE) | SLEEP_DURATION_BITS;  \
  CCP = 0xD8;                                \
  CLKPR = (_BV(CLKPS0) | _BV(CLKPS1));

// FIXME: check!
// PRR = 0xFF;


// go into sleep mode and wake up after wtd timeout
// also disable SPI
#define sleep_enter()  \
  adc_disable();       \
  SPCR   &= ~_BV(SPE); \
  sleep_enable();      \
  sleep_mode();        \
  sleep_disable();     \
  SPCR = (1<<SPE)

// nothing extra to do on wake up, just wake up
#define sleep_vect() ISR(WDT_vect){}


#endif
