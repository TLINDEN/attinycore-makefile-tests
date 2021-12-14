/* -*-c++-*-
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

/*
  ./hardware/ATTinyCore/avr/cores/tinymodern/core_pins.h
  https://raw.githubusercontent.com/SpenceKonde/ATTinyCore/master/avr/extras/ATtiny_x41.png
  Caution: reverse Pinout: PA3 => Pin 3

  Nanite841 Pinout from above (ARDUINO pins in the middle):

                button
SDO/MISO PA5  5/5    4/6   PA6 SDI/MOSI
     SCK PA4  4/6    3/7   PA7
     CS  PA3  3/7    2/8   PB2
     RX0 PA2  8/2   11/11  PB3
     TX0 PA1  1/9    9/1   PB1
         PA0 0/10   10/0   PB0
         GND               VCC
                 usb

attiny841 pins => Arduino layout

TOCC0   OC0B    pin 9 / PA1 - 841 pin 12    disabled
TOCC1   OC0A    pin 8 / PA2 - 841 pin 11    disabled
TOCC2   OC1B    pin 7 / PA3 - 841 pin 10
TOCC3   OC0A    pin 6 / PA4 - 841 pin 9
TOCC4   OC0B    pin 5 / PA5 - 841 pin 8
TOCC5   OC1A    pin 4 / PA6 - 841 pin 7
TOCC6   OC2B    pin 3 / PA7 - 841 pin 6
TOCC7   OC2A    pin 2 / PB2 - 841 pin 5

*/

#include <avr/io.h>
#include <util/delay.h>

#define LED           10 // PB2

#define SET_OUTPUT(pin) DDRB  |=  (1 << pin)
#define SET_HIGH(pin)   PORTB |=  (1 << pin)
#define SET_LOW(pin)    PORTB &= ~(1 << pin)

void setup() {
  uint8_t i = 0;
  SET_OUTPUT(LED);
  for (i=0; i<11; i++) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(115200);
  Serial.println("init");
}

void loop_ard() {
  uint8_t i = 0;
  for (i=0; i<11; i++) {
    Serial.print("Turning on pin: ");
    Serial.println(i);
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i, LOW);
    delay(1000);
  }
  delay(100);
}

void loop_man() {
  SET_HIGH(LED);
  delay(1000);
  SET_LOW(LED);
  delay(100);
}


void loop() {
  loop_man();
}
