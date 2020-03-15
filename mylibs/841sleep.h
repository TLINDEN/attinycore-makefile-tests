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
  CLKPR = (_BV(CLKPS0) | _BV(CLKPS1));


// go into sleep mode
// disable ADC
// wait a little for the last serial out (if any) to flush
// disable SPI
// set all bits in power reduction register to one (shutdown everything)
// actually sleep
// wake up after WDT timeout
// wait a little
// re-enable SPI
#define sleep_enter()  \
  adc_disable();       \
  delay(10);           \
  SPCR   &= ~_BV(SPE); \
  PRR = 0xFF;          \
  sleep_mode();        \
  PRR = 0x00;          \
  delay(10);           \
  SPCR = (1<<SPE)

// nothing extra to do on wake up, just wake up
#define sleep_vect() ISR(WDT_vect){}


#endif
