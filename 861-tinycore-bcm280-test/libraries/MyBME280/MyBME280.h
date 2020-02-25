/*
 MyBME280.h

 Modified to work with Attiny861+I2C+AttinyCore+Arduino-Makefile

*/

#ifndef MyBME280_h
#define MyBME280_h

#define defined_ATtiny (defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny861__)) 

#if defined_ATtiny
  #include <Wire.h>
#else
  #include <Wire.h>
  #include <SPI.h>
#endif

#include "Arduino.h"

#define BME280_SPI 1 //SPI-Schnittstelle

#define BMP280_CHIP_ID_REG 0xD0
#define BMP280_CHIP_ID 0x60

#define BME280_DIG_T1_REG 0x88
#define BME280_DIG_T2_REG 0x8A
#define BME280_DIG_T3_REG 0x8C
#define BME280_DIG_P1_REG 0x8E
#define BME280_DIG_P2_REG 0x90
#define BME280_DIG_P3_REG 0x92
#define BME280_DIG_P4_REG 0x94
#define BME280_DIG_P5_REG 0x96
#define BME280_DIG_P6_REG 0x98
#define BME280_DIG_P7_REG 0x9A
#define BME280_DIG_P8_REG 0x9C
#define BME280_DIG_P9_REG 0x9E
#define BME280_DIG_H1_REG 0xA1
#define BME280_DIG_H2_REG 0xE1
#define BME280_DIG_H3_REG 0xE3
#define BME280_DIG_H4_REG 0xE4
#define BME280_DIG_H5_REG 0xE5
#define BME280_DIG_H6_REG 0xE7

#define BME280_HUM_MSB_REG     0xFD
#define BME280_TEMP_MSB_REG    0xFA
#define BME280_PRESS_MSB_REG   0xF7
#define BME280_HUM_CONTROL_REG 0xF2
#define BME280_STATUS_REG      0xF3
#define BME280_CONTROL_REG     0xF4
#define BME280_CONFIG_REG      0xF5
#define BME280_DATA_REG        0xF7
#define BME280_RESET_REG       0xE0

#define BME280_NORMAL_MODE B11
#define BME280_FORCED_MODE B01
#define BME280_SLEEP_MODE  B00

#define BME280_RESET 0xB6

#define BME280_T_OVERSAMPLING_x0  B000 
#define BME280_T_OVERSAMPLING_x1  B001 
#define BME280_T_OVERSAMPLING_x2  B010
#define BME280_T_OVERSAMPLING_x4  B011
#define BME280_T_OVERSAMPLING_x8  B100
#define BME280_T_OVERSAMPLING_x16 B101

#define BME280_P_OVERSAMPLING_x0  B000 
#define BME280_P_OVERSAMPLING_x1  B001 
#define BME280_P_OVERSAMPLING_x2  B010
#define BME280_P_OVERSAMPLING_x4  B011
#define BME280_P_OVERSAMPLING_x8  B100
#define BME280_P_OVERSAMPLING_x16 B101

#define BME280_H_OVERSAMPLING_x0  B000
#define BME280_H_OVERSAMPLING_x1  B001
#define BME280_H_OVERSAMPLING_x2  B010
#define BME280_H_OVERSAMPLING_x4  B011
#define BME280_H_OVERSAMPLING_x8  B100
#define BME280_H_OVERSAMPLING_x16 B101

#define BME280_STANDBY_TIME_0p5  B000
#define BME280_STANDBY_TIME_62p5 B001
#define BME280_STANDBY_TIME_125  B010
#define BME280_STANDBY_TIME_250  B011
#define BME280_STANDBY_TIME_500  B100
#define BME280_STANDBY_TIME_1000 B101
#define BME280_STANDBY_TIME_10   B110
#define BME280_STANDBY_TIME_20   B111

#define BME280_FILTER_COEFF_0  B000
#define BME280_FILTER_COEFF_2  B001
#define BME280_FILTER_COEFF_4  B010
#define BME280_FILTER_COEFF_8  B011
#define BME280_FILTER_COEFF_16 B100



class MyBME280
{
  public:
    MyBME280(byte);
    MyBME280(byte, byte);
    bool isReady(void);
    void init();
    void startSingleMeas(void);
    float readTemp(void);
    float readPress(void);
    float readHumidity(void);
    float readReducedPress(int);
    void setMode(byte);
    void setTempOversampling(byte);
    void setPressOversampling(byte);
    void setHumidityOversampling(byte);
    void setStandbyTime(byte);
    void setFilterCoeff(byte);
    byte readControlReg(void);
    byte readConfigReg(void);
    byte readHumidityControlReg(void);
    void setWeatherMonitoring(void);
    void setHumiditySensing(void);
    void setIndoorNavigation(void);
    void setGaming(void);
     
  private:
    void readRawData(void);
    float calcTemp(void);
    float calcPress(void);
    float calcHumidity(void);
    byte read8(byte);
    int read16(byte);
    void write8(byte, byte);
    bool statusMeas(void);
    void reset(void);
    void setDefaultParameters(void);
    
    unsigned int dig_T1;
    int dig_T2;
    int dig_T3;
    unsigned int dig_P1;
    int dig_P2;
    int dig_P3;
    int dig_P4;
    int dig_P5;
    int dig_P6;
    int dig_P7;
    int dig_P8;
    int dig_P9;
    uint8_t dig_H1;
    int dig_H2;
    uint8_t dig_H3;
    int dig_H4;
    int dig_H5;
    int8_t dig_H6;
	
    long t_fine;
    long adc_T;
    long adc_P;
    long adc_H;
	
    byte _i2cAdd;
    int8_t _cs;

    byte mode;
    byte osrs_t;
    byte osrs_p;
    byte osrs_h;
    byte t_sb;
    byte filter;
   
    byte controlVal;
    byte configVal;
    byte controlHumVal;
    
};

#endif

