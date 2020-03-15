/*
  Macros for easier usage of vcc measurements.
  For attiny8441 only.

  We use REFS=0 to select VCC as Reference. i.e.  ADMUXB = 0 << REFS0
  We then use channel #13 to read the BandGap.  i.e. ADMUXA = 13 << MUX0

  And calculate the value of VCC from the reading.  i.e. ADCW / 1024 = 1.1 / VCC
  Rearrange i.e. VCC = 1.1 * 1024 / ADCW

  Thanks for the help of the guys at AVR-Freaks:
  https://www.avrfreaks.net/forum/measuring-vcc-attiny841-using-adc-and-internal-bandgap
*/


#include <util/delay.h>

#ifndef VCC_8441_H
#define VCC_8441_H

// disable ADC globally
#define adc_disable() ADCSRA &= ~(1<<ADEN);

// enable
#define adc_enable()  ADCSRA  =  (1<<ADEN);  _delay_ms(10);

// use supply voltage VCC as reference
// measure internal bandgap 1v1 voltage reference
// via channel 13, see table 16-3
// and do not left shift result
#define adc_setup_vcc_measurement() \
  ADMUXB = (0<<REFS0);              \
  ADMUXA = (13<<MUX0);              \
  ADCSRB = (0<<ADLAR);

// start measurement
// and wait till complete
#define adc_start()      \
  ADCSRA |= _BV( ADSC ); \
  while( ( (ADCSRA & (1<<ADSC)) != 0 ) );

/* return measured voltage, adc value between 1-1023
   Calculate like:

   >> 1.1 * 1024 / 225 #=> 5.006222222222223
   >> 1.1 * 1024 / 341 #=> 3.3032258064516133
*/
#define adc_get_adcw() ADCW;

#define adc_get_vcc() 1.1 * 1024 / ADCW;

#endif
