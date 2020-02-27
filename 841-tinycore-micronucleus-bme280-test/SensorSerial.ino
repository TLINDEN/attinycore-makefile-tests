/* -*-c++-*-
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

/*
  Nanite841 Pinout from above:

         button
    PA5          PA6 SDA
SCL PA4          PA7
    PA3          PB2
RX0 PA2          PB2
TX0 PA1          PB1
    PA0          PB0
    GND          VCC
          usb

 */

#include <Wire.h>
#include <TinyBME280.h>

int const sda = PA6;
int const scl = PA4;

#define LED LED_BUILTIN

void blinken(int howlong) {
  digitalWrite(LED, HIGH);
  delay(howlong);
  digitalWrite(LED, LOW);
  delay(howlong);
}

void print_measurements() {
  Serial.print("     Temperature: ");
  Serial.print(BME280temperature()/10);
  Serial.println(" Grad C");
  Serial.print("        Pressure: ");
  Serial.print(BME280pressure()/100);
  Serial.println(" hPa");
  Serial.print("        Humidity: ");
  Serial.print(BME280humidity()/100);
  Serial.println(" %");
  Serial.println();
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.print("init");

  Wire.begin();
  BME280setI2Caddress(0x76);
  BME280setup();
}

void loop() {
  print_measurements();
  delay(1000);
}

