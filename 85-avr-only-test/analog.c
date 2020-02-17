#include "analog.h"

#ifdef ADCSRA

int analogRead (uint8_t pin){
  int a=1, i=a, j=a;
  long int analogwert=0, analogwert1=0, analogwert2=0 ;
        
  while(j){
    while(i){
      ADCSRA=0x80;            // ADC eingeschaltet, kein Prescale 
      ADMUX=pin;
      ADCSRA |=_BV(ADSC);     // single conversion mode ein
      while (ADCSRA & (1<<ADSC)) {;}  // auf Abschluss der Konvertierung warten 
      analogwert+=ADCW;
      i--;
    }
    analogwert1 = analogwert/a;
    analogwert2 += analogwert1;
    j--;
  }
  analogwert=(analogwert2/a);
  return (analogwert);
}

#else

#include <stdlib.h>
int analogRead (uint8_t pin) {
  // No ADC on this MCU
  abort();
}

#endif
