#ifndef ANALOG_H
#define ANALOG_H

/*
 * via http://www.sachsendreier.com/msw/projekte/attiny13_analog_bargraph/atiny13_analog_bargraph.html
 * Alternative: http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Analoge_Ein-_und_Ausgabe#Nutzung_des_ADC
 */

#include <avr/io.h>

int analogRead(uint8_t pin);
#endif
