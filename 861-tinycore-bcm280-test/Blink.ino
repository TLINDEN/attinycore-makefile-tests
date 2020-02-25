/* -*-c++-*-
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

#include <MyBME280.h>

// via https://arduino-projekte.webnode.at/meine-libraries/luftdruck-luftfeuchtesensor-bme280/
#define BME280_I2cAdd 0x76
MyBME280 BME(BME280_I2cAdd);

unsigned long wait_time;

float temperature, pressure, reduced_pressure, humidity;
int altitude = 8;

void take_measurement();
void print_measurements();

void setup() {
  Serial.begin(9600);
  
  if (BME.isReady()) {
    Serial.println("BME280 ok");

    BME.setWeatherMonitoring();
    BME.init();

    take_measurement();
    print_measurements();
  }
  else {
    Serial.println("BME280 Error");
    while (1);
  }
  wait_time = millis();
}

void loop() {
  if (millis() > wait_time + 60000) {
    BME.startSingleMeas();
    take_measurement();
    print_measurements();
    wait_time = millis();
  }
}



void take_measurement() {
  temperature = BME.readTemp();
  pressure = BME.readPress();
  reduced_pressure = BME.readReducedPress(altitude);
  humidity = BME.readHumidity();
}



void print_measurements() {
  Serial.print("     Temperature: ");
  Serial.print(temperature);
  Serial.println(" Grad C");
  Serial.print("        Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Reduced Pressure: ");
  Serial.print(reduced_pressure);
  Serial.println(" hPa");
  Serial.print("        Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.println();
}
