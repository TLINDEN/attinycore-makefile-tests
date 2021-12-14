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
#include "data2wire.h"

#define TINY_BME280_SPI
#include <TinyBME280.h>

// https://andreasrohner.at/posts/Electronics/New-Arduino-library-for-433-Mhz-AM-Radio-Modules/
#include <RFTransmitter.h>

#define NODE_ID       1
#define SENDER_PIN    10 // == PB0 // ARDUINO PIN#
#define LED           2  // LED_BUILTIN  // PORTB2
#define CS            3
#define DELAY         5000
#define WDTREPEATS    3

static tiny::BME280 sensor;
RFTransmitter transmitter(SENDER_PIN, NODE_ID);
const long InternalReferenceVoltage = 1083L;

#define SET_OUTPUT(pin) DDRB  |=  (1 << pin)
#define SET_LOW(pin)    PORTB |=  (1 << pin)
#define SET_HIGH(pin)   PORTB &= ~(1 << pin)

typedef struct _measurements_t {
  uint32_t pres;
  uint32_t humidity;
  int32_t  temp;
  uint16_t vcc;
} measurements_t;

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
  measurements_t ms;
  byte sendms[sizeof(measurements_t)];

  Serial.println("-- Enter Measure");
  SET_LOW(LED);

  Serial.println("-- Read measurementes");
  ms.temp     = sensor.readFixedTempC();
  ms.humidity = sensor.readFixedHumidity();
  ms.pres     = sensor.readFixedPressure();

  Serial.println("-- Read VCC");
  adc_enable();
  adc_start();
  ms.vcc = adc_get_adcw();
  adc_disable();
  
  Serial.print("Voltage: ");
  Serial.println(ms.vcc);

  Serial.print("     Temperature: ");
  print_asifloat(ms.temp, 100);
  Serial.println(" Grad C");

  Serial.print("        Pressure: ");
  print_asufloat(ms.pres, 100);
  Serial.println(" hPa");

  Serial.print("        Humidity: ");
  print_asufloat(ms.humidity, 1000);
  Serial.println(" %");

  data32_to_wire(ms.pres,     &sendms[0]);
  data32_to_wire(ms.humidity, &sendms[sizeof(ms.humidity) + 1]);
  data32_to_wire(ms.temp,     &sendms[sizeof(ms.temp)     + 1]);
  data16_to_wire(ms.vcc,      &sendms[sizeof(ms.vcc)      + 1]);
  
  transmitter.send((byte *)sendms, sizeof(sendms));

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

  if(sensor.beginSPI(CS) == false) {
    Serial.println("Sensor BME280 connect failed, check wiring!");
    halt();
  }

  sleep_setup();
}

void loop_nosleep() {
  uint8_t i;
  
  print_measurements();

  delay(100);
}

void loop_sleep() {
  uint8_t i;
  Serial.println("-- ENTER LOOP");
  
  print_measurements();
  //delay(DELAY);

  for(i=0; i<WDTREPEATS; i++) {
    delay(DELAY);
    Serial.print("----- ENTER SLEEP: ");
    Serial.println(i);
    sleep_enter_keep_spi();
    Serial.println("----- LEAVE SLEEP");
  }
  Serial.println("-- AWAKE");
}

void loop() {
  loop_sleep();
}

sleep_vect();

