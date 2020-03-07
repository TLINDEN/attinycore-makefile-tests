/* -*-c++-*-
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

/*
  ./hardware/ATTinyCore/avr/cores/tinymodern/core_pins.h
  Caution: reverse Pinout: PA3 => Pin 3

  Nanite841 Pinout from above:

              button
SDO/MISO PA5          PA6 SDI/MOSI
     SCK PA4          PA7
     CS  PA3          PB2
     RX0 PA2          PB2
     TX0 PA1          PB1
         PA0          PB0
         GND          VCC
               usb

  Bosch BME280 Breakout from above pin header left:

    VCC
    GND
    SCL  => SCK
    SDI  => MOSI
    CSB  => PA3/3
    SDO  => MISO
  
 */

#include <avr/io.h>
#include <util/delay.h>

#define NODE_ID       1
#define OUTPUT_PIN    7
#define LED           2 //LED_BUILTIN  // PORTB2
#define CS            3
#define DELAY         2000

#define SET_OUTPUT(pin) DDRB  |=  (1 << pin)
#define SET_HIGH(pin)   PORTB |=  (1 << pin)
#define SET_LOW(pin)    PORTB &= ~(1 << pin)

void print_asufloat(uint32_t val, uint16_t factor) {
  Serial.print(val / factor);
  Serial.print(".");
  Serial.print(val % factor);
}

void print_asifloat(int32_t val, uint16_t factor) {
  Serial.print(val / factor);
  Serial.print(".");
  Serial.print(val % factor);
}

void print_measurements() {
  float vcc;
  
  SET_LOW(LED);

  ADCSRA = (1 << ADEN);                   // enable ADC
  _delay_ms(10);                          // give it some time to settle
  ADCSRA |= _BV( ADSC );                  // start measuring
  while( ( (ADCSRA & (1<<ADSC)) != 0 ) ); // till it's completed
  // >> 1.1 * 1024 / 225 #=> 5.006222222222223
  // >> 1.1 * 1024 / 341 #=> 3.3032258064516133
  vcc = 1.1 * 1024 / ADCW;                // read actual measurement
  //  ADCSRA &= ~(1<<ADEN);                   // we're done, disable ADC

  Serial.print("Voltage: ");
  Serial.println(vcc);
  
  SET_HIGH(LED);
}

void halt() {
  while(1);
}


void setup() {
  SET_OUTPUT(LED);
  Serial.begin(115200);
  Serial.println("init");

  // Setup the ADC
  // https://www.avrfreaks.net/forum/measuring-vcc-attiny841-using-adc-and-internal-bandgap#comment-2862601
  ADMUXB = (0<<REFS0);  // enable REFS0 bit == internal 1.1 bandgap
  ADMUXA = (13<<MUX0);  // use channel #13 to read the bandgap (binary 1101, see table 16-3)
  ADCSRB = (0<<ADLAR);  // do not left shift result

  // disable ADC for powersaving
  //  ADCSRA &= ~(1<<ADEN);

  // disable analog comperators for powersaving
  ACSR0A |= _BV(ACD0);
  ACSR1A |= _BV(ACD1);
}

void loop() {
  print_measurements();
  delay(DELAY);
}

