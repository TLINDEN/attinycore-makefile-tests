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
#include "841vcc.h"
#include "841sleep.h"

#define NODE_ID       1
#define OUTPUT_PIN    7
#define LED           2 //LED_BUILTIN  // PORTB2
#define CS            3
#define DELAY         5000
#define WDTREPEATS    10 // results in 8s x 10 times == 80 seconds sleep

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

  adc_enable();
  adc_start();
  vcc = adc_get_vcc();
  adc_disable();
  
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
  delay(4000);
  adc_setup_vcc_measurement();
  sleep_setup();
}

void loop() {
  uint8_t i;
  print_measurements();
  Serial.println("delay...");
  delay(10000);
  for(i=0; i<WDTREPEATS; i++) {
    Serial.print("----- ENTER SLEEP: ");
    Serial.println(i);
    sleep_enter();
    Serial.println("----- LEAVE SLEEP");
  }
  Serial.println("-- DONE");
}

sleep_vect();

