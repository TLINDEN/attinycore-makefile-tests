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

#define TINY_BME280_SPI
#include <TinyBME280.h>

#define LED LED_BUILTIN
#define CS  3

tiny::BME280 sensor;
uint32_t pres, humidity;
int32_t  temp;


void print_measurements() {
  temp     = sensor.readFixedTempC();
  humidity = sensor.readFixedHumidity();
  pres     = sensor.readFixedPressure();

  Serial.print("     Temperature: ");
  Serial.print(temp / 100.0);
  Serial.println(" Grad C");
  Serial.print("        Pressure: ");
  Serial.print(pres / 100.0);
  Serial.println(" hPa");
  Serial.print("        Humidity: ");
  Serial.print(humidity / 1000.0);
  Serial.println(" %");
  Serial.println();
}

void halt() {
  while(1);
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("init");
  howlong = 500;
  if(sensor.beginSPI(CS) == false) {
    Serial.println("Sensor BME280 connect failed, check wiring!");
    halt();
  }
  
}

void loop() {
  print_measurements();
  delay(2000);
}

