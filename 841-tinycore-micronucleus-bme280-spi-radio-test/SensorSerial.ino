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

#define TINY_BME280_SPI
#include <TinyBME280.h>

// https://andreasrohner.at/posts/Electronics/New-Arduino-library-for-433-Mhz-AM-Radio-Modules/
#include <RFTransmitter.h>

#define NODE_ID       1
#define OUTPUT_PIN    7
#define LED           2 //LED_BUILTIN  // PORTB2
#define CS            3
#define DELAY         5000

static tiny::BME280 sensor;
RFTransmitter transmitter(OUTPUT_PIN, NODE_ID);
const long InternalReferenceVoltage = 1083L;

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
  uint32_t pres, humidity;
  int32_t  temp;

  temp     = sensor.readFixedTempC();
  humidity = sensor.readFixedHumidity();
  pres     = sensor.readFixedPressure();

  SET_LOW(LED);

  Serial.print("     Temperature: ");
  print_asifloat(temp, 100);
  Serial.println(" Grad C");

  Serial.print("        Pressure: ");
  print_asufloat(pres, 100);
  Serial.println(" hPa");

  Serial.print("        Humidity: ");
  print_asufloat(humidity, 1000);
  Serial.println(" %");

  transmitter.send((byte *)temp, sizeof(temp));
  SET_HIGH(LED);
}

void halt() {
  while(1);
}

void print_voltage() {
  int value;
  ADCSRA |= _BV( ADSC );
  while( ( (ADCSRA & (1<<ADSC)) != 0 ) );
  value = (((InternalReferenceVoltage * 1024L) / ADC) + 5L) / 10L;
  Serial.print("         Voltage: ");
  Serial.println(value);
  Serial.println();
}

void setup() {
  SET_OUTPUT(LED);
  Serial.begin(115200);
  Serial.println("init");
  // Enable ADC
  ADCSRA |= (1 << ADEN);
  ADMUXB = (0<<REFS2) | (1<<REFS1) | (0<<REFS0);
  ADMUXA = (0<<MUX5) | (0<<MUX4) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (1<<MUX0);
  ADCSRB = (0<<ADLAR);
  //analogReference( INTERNAL );

  if(sensor.beginSPI(CS) == false) {
    Serial.println("Sensor BME280 connect failed, check wiring!");
    halt();
  }  
}

void loop() {
  print_measurements();
  print_voltage();
  delay(DELAY);
}

