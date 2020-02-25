//MyBME280.cpp
//Code fuer Arduino und Attiny85
//Author Retian
//Version 2.1

/*
Abfrage eines Luftdruck-/Luftfeuchtesensors BME280 ueber I2C- oder SPI-Schnittstelle
(SPI-Schnittstelle nicht fuer Attiny)

MyBME280 Name(i2cAdd);        //fuer I2C-Schnittstelle
MyBME280 Name(cs, interface); //fuer SPI-Schnittstelle; interface: BME280_SPI

Beispiel siehe unter:
http://arduino-projekte.webnode.at/meine-libraries/luftdruck-luftfeuchtesensor-bme280/

Funktionen siehe unter:
http://arduino-projekte.webnode.at/meine-libraries/luftdruck-luftfeuchtesensor-bme280/funktionen/

*/

//*************************************************************************

#include "Arduino.h"
#include "MyBME280.h"

MyBME280::MyBME280(byte i2cAdd) : _cs(-1)
{
  _i2cAdd = i2cAdd;
  
  Wire.begin();
  setDefaultParameters();
}

MyBME280::MyBME280(byte cs, byte interface)
{
#if defined_ATtiny
  //Keine Aktivitaet, wenn ein Attiny eingesetzt wird
#else
  //"interface" wird nicht benoetigt, ist nur zur Unterscheidung
  //beim Aufruf der Instanzen zwischen SPI- und I2C-Schnittstelle
  _cs = cs;
    
  pinMode(_cs, OUTPUT);
  SPI.begin();
  setDefaultParameters();
#endif
}

//*************************************************************************
//Setzen der Standard-Parameter
//(Interne Verwendung)

void MyBME280::setDefaultParameters()
{
  controlVal = 0;
  configVal = 0;
  controlHumVal = 0;
   
  //Setzen der Voreinstellungen
  mode = BME280_NORMAL_MODE;
  setMode(mode);
  
  osrs_p = BME280_P_OVERSAMPLING_x8; //Pressure oversampling x8
  setPressOversampling(osrs_p);
  
  osrs_t = BME280_T_OVERSAMPLING_x2; //Temperature oversampling x2
  setTempOversampling(osrs_t);
  
  osrs_h = BME280_H_OVERSAMPLING_x8; //Humidity oversampling x8
  setHumidityOversampling(osrs_h);
  
  t_sb =  BME280_STANDBY_TIME_1000; //Standby time (NORMAL_MODE) = 1000 ms
  setStandbyTime(t_sb);
  
  filter = BME280_FILTER_COEFF_4; //Filter-Koeffizient = 4 (5 Stufen fuer 75%)
  setFilterCoeff(filter);
}


//*************************************************************************
//BME280 vorhanden ?

bool MyBME280::isReady()
{
  if (read8(BMP280_CHIP_ID_REG) == BMP280_CHIP_ID) return true;
  else return false;
}

//*************************************************************************
//BME280 initialisiern
void MyBME280::init()
{
  //BME280 zuruecksetzen
  reset();
  delay(300);
    
  //Kalibrierungskoeffizienten einlesen

  //Wenn der BME280 gerade die Koeffizienten ins interne
  //Image-Register liest, dann warte
  while (read8(BME280_STATUS_REG) & 0x01) delay(1);
	
  dig_T1 = (unsigned int)read16(BME280_DIG_T1_REG);
  dig_T2 = read16(BME280_DIG_T2_REG);
  dig_T3 = read16(BME280_DIG_T3_REG);
  dig_P1 = (unsigned int)read16(BME280_DIG_P1_REG);
  dig_P2 = read16(BME280_DIG_P2_REG);
  dig_P3 = read16(BME280_DIG_P3_REG);
  dig_P4 = read16(BME280_DIG_P4_REG);
  dig_P5 = read16(BME280_DIG_P5_REG);
  dig_P6 = read16(BME280_DIG_P6_REG);
  dig_P7 = read16(BME280_DIG_P7_REG);
  dig_P8 = read16(BME280_DIG_P8_REG);
  dig_P9 = read16(BME280_DIG_P9_REG);
  dig_H1 = (uint8_t)read8(BME280_DIG_H1_REG);
  dig_H2 = read16(BME280_DIG_H2_REG);
  dig_H3 = (uint8_t)read8(BME280_DIG_H3_REG);
  dig_H4 = (read8(BME280_DIG_H4_REG) << 4) | 
	       (read8(BME280_DIG_H4_REG + 1) & 0xF);
  dig_H5 = (read8(BME280_DIG_H5_REG + 1) << 4) | 
	       (read8(BME280_DIG_H5_REG) >> 4);
  dig_H6 = (uint8_t)read8(BME280_DIG_H6_REG);
  
  //Schreiben der gesetzten Parameter in die Register
  //Aenderungen beim Humidity-Controll-Register werden erst nach einer
  //Schreiboperation auf das Control-Register wirksam (lt. Datenblatt 5.4.3)
  write8(BME280_CONFIG_REG, configVal);
  write8(BME280_HUM_CONTROL_REG, controlHumVal);
  write8(BME280_CONTROL_REG, controlVal);
}

//*************************************************************************
//Starte Einzelmessung (FORCED_MODE)

void MyBME280::startSingleMeas()
{
  controlVal &= B11111100;
  controlVal |= BME280_FORCED_MODE;
  write8(BME280_CONTROL_REG, controlVal);
  while (statusMeas()); //Warte bis Messzyklus beendet
}

//*************************************************************************
//Ermittlung der Lufttemperatur

float MyBME280::readTemp()
{
  readRawData();
  if (adc_T == 0x80000)  //Rueckgabe, wenn Temperaturmesseung ausgeschaltet ist
	return NAN;
  return calcTemp();
}

//*************************************************************************
//Ermittlung des Luftdrucks

float MyBME280::readPress()
{
  readRawData();
  if (adc_P == 0x80000) //Rueckgabe, wenn Luftdruckmessung ausgeschaltet ist
	return NAN; 
  calcTemp(); //calcPress() benoetigt t_fine aus calcTemp()
  return calcPress();
}

//*************************************************************************
//Ermittlung der Luftfeuchtigkeit

float MyBME280::readHumidity()
{
  readRawData();
  if (adc_H == 0x8000) //Rueckgabe, wenn Luftfeuchtemessung ausgeschaltet ist
	return NAN; 
  calcTemp(); //calcHumidity() benoetigt t_fine aus calcTemp()
  return calcHumidity();
}

//*************************************************************************
//Reduzierten Luftdruck bezogen auf Meereshoehe ermitteln

float MyBME280::readReducedPress(int hoehe)
{
  //Berechne reduzierten Luftdruck
  int _hoehe = hoehe;
  readRawData();
  if (adc_P == 0x80000) //Rueckgabe, wenn Luftdruckmessung ausgeschaltet ist
	return NAN;
  calcTemp();
  return (calcPress() / (pow(1.0 - ((float)_hoehe / 44330.0), 5.255)));
 }

//*************************************************************************
//Einlesen von 8 Bytes (burst read) ueber die I2C-Schnittstelle
//(Interne Verwendung)

void MyBME280::readRawData()
{
  while (statusMeas()); //Warte bis Messzyklus beendet
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(BME280_DATA_REG);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAdd, (byte)8);
    adc_P = Wire.read();
    adc_P <<= 8;
    adc_P |= Wire.read();
    adc_P <<= 8;
    adc_P |= Wire.read();
    adc_P >>= 4;
    adc_T = Wire.read();
    adc_T <<= 8;
    adc_T |= Wire.read();
    adc_T <<= 8;
    adc_T |= Wire.read();
    adc_T >>= 4;
    adc_H = Wire.read();
    adc_H <<= 8;
    adc_H |= Wire.read();
  }
  
  else
  {
  #if defined_ATtiny
    //Keine Aktivitaet, wenn ein Attiny eingesetzt wird
  #else
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    SPI.transfer(BME280_DATA_REG | 0x80);
    adc_P = SPI.transfer(0);
    adc_P <<= 8;
    adc_P |= SPI.transfer(0);
    adc_P <<= 8;
    adc_P |= SPI.transfer(0);
    adc_P >>= 4;
    adc_T = SPI.transfer(0);
    adc_T <<= 8;
    adc_T |= SPI.transfer(0);
    adc_T <<= 8;
    adc_T |= SPI.transfer(0);
    adc_T >>= 4;
    adc_H = SPI.transfer(0);
    adc_H <<= 8;
    adc_H |= SPI.transfer(0);
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();
  #endif
  }
}

//*************************************************************************
//Temperatur ermitteln
//(Interne Verwendung)

float MyBME280::calcTemp()
{
  int32_t var1, var2;
  int32_t temp;

  //Berechne Temperatur
  var1 = (((adc_T >> 3) - (dig_T1 << 1)) * dig_T2) >> 11;
  var2 = (((((adc_T >> 4) - (dig_T1)) * ((adc_T >> 4) - dig_T1)) >> 12) * dig_T3) >> 14;
  t_fine = var1 + var2;
  temp = (t_fine * 5 + 128) >> 8;
  return (float)temp / 100.0;
}

//*************************************************************************
//Luftdruck ermitteln
//(Interne Verwendung)

float MyBME280::calcPress()
{
  int64_t var1, var2;
  int64_t press;
  
  //Berechne Luftdruck
  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)dig_P6;
  var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
  var2 = var2 + (((int64_t)dig_P4) << 35);
  var1 = ((var1 * var1 + (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
  if (var1 == 0) return 0;
  press= 1048576 - adc_P;
  press= (((press << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)dig_P9) * (press>> 13) * (press>> 13)) >> 25;
  var2 = (((int64_t)dig_P8) * press) >> 19;
  press = ((press + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
  press /= 256;
  return (float) press / 100.0;
}

//*************************************************************************
//Luftfeuchte ermitteln
//(Interne Verwendung)

float MyBME280::calcHumidity()
{
  int32_t v_x1_u32r;
  uint32_t humidity;

//Berechne Luftfeuchtigkeit
  v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768)))  >> 10) +
((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
((int32_t)dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
  humidity = (uint32_t)(v_x1_u32r >> 12);
  return (float)humidity / 1024.0;
}

//*************************************************************************
// Schreiben von 1 Byte ueber die I2C-Schnittstelle
//(Interne Verwendung)

void MyBME280::write8(byte add, byte val)
{
  byte _add = add;
  byte _val = val;
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(_add);
    Wire.write(_val);
    Wire.endTransmission();
  }
  
  else
  {
  #if defined_ATtiny
    //Keine Aktivitaet, wenn ein Attiny eingesetzt wird
  #else
    digitalWrite(_cs, LOW);
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(_add & 0x7F);
    SPI.transfer(val);
    SPI.endTransaction();
    digitalWrite(_cs, HIGH);
  #endif
  }
  delay(5);
}

//*************************************************************************
// Einlesen von 1 Byte ueber die I2C-Schnittstelle
//(Interne Verwendung)

byte MyBME280::read8(byte adr)
{
  byte _adr = adr;
  byte val;
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(_adr);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAdd, (byte)1);
    val = Wire.read();
  }
    
  else
  {
  #if defined_ATtiny
    //Keine Aktivitaet, wenn ein Attiny eingesetzt wird
  #else
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    SPI.transfer(_adr | 0x80);
    val = SPI.transfer(0);
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();
  #endif
  }
  return val;
}

//*************************************************************************
// Einlesen von 2 Bytes ueber die I2C-Schnittstelle 
//(Interne Verwendung)

int MyBME280::read16(byte adr)
{
  byte _adr = adr;
  int val;
  
  if (_cs == -1)
  {
    Wire.beginTransmission(_i2cAdd);
    Wire.write(_adr);
    Wire.endTransmission();
    Wire.requestFrom(_i2cAdd, (byte)2);
    val = Wire.read() | (Wire.read() << 8);
  }
  
  else
  {
  #if defined_ATtiny
    //Keine Aktivitaet, wenn ein Attiny eingesetzt wird
  #else
    SPI.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    SPI.transfer(_adr | 0x80);
    val = SPI.transfer(0) | (SPI.transfer(0) << 8);
    digitalWrite(_cs, HIGH);
    SPI.endTransaction();
  #endif
  }
  return val;
}

//*************************************************************************
//Setzen des Modus

void MyBME280::setMode(byte mode)
{
  byte _mode = mode;
  
  controlVal &= B11111100; //Loesche mode 
  controlVal |= _mode;
}

//*************************************************************************
//Setzen Temperatur-Oversampling

void MyBME280::setTempOversampling(byte osrs_t)
{
  byte _osrs_t = osrs_t;
    
  controlVal &= ~(B111 << 5); //Loesche Temp.oversampling
  controlVal |= (_osrs_t << 5);
}

//*************************************************************************
//Setzen Druck-Oversampling

void MyBME280::setPressOversampling(byte osrs_p)
{
  byte _osrs_p = osrs_p;

  controlVal &= ~(B111 << 2); //Loesche Press.oversampling
  controlVal |= (_osrs_p << 2);
}

//*************************************************************************
//Setzen Feuchte-Oversampling

void MyBME280::setHumidityOversampling(byte osrs_h)
{
  byte _osrs_h = osrs_h;

  controlHumVal &= ~(B111); //Loesche Humid.oversampling
  controlHumVal |= _osrs_h;
}

//*************************************************************************
//Setzen der Standby-Zeit (Normal-Modus)

void MyBME280::setStandbyTime(byte t_sb)
{
  byte _t_sb = t_sb;
  
  configVal &= ~(B111 << 5); //Loesche Standby Time
  configVal |= (_t_sb << 5);
}

//*************************************************************************
//Setzen des Filterkoeffizienten

void MyBME280::setFilterCoeff(byte filter)
{
  byte _filter = filter;
  
  configVal &= ~(B111 << 2); //Loesche Filter
  configVal |= (_filter << 2);
}

//*************************************************************************
//Abfrage des Controll-Register 0xF4

byte MyBME280::readControlReg(void)
{
  return read8(BME280_CONTROL_REG);
}

//*************************************************************************
//Abfrage des Config-Register 0xF5

byte MyBME280::readConfigReg(void)
{
  return read8(BME280_CONFIG_REG);
}

//*************************************************************************
//Abfrage des Humidity-Control-Register 0xF2

byte MyBME280::readHumidityControlReg(void)
{
  return read8(BME280_HUM_CONTROL_REG);
}

//*************************************************************************
//Abfrage des Status des Measuring-Bit
//(Interne Verwendung)

bool MyBME280::statusMeas()
{
  byte reg;
	
  reg = read8(BME280_STATUS_REG);
  return (reg & B00001000) >> 3;
}

//*************************************************************************
//Software-Reset
//(Interne Verwendung)

void MyBME280::reset()
{
  write8(BME280_RESET_REG, BME280_RESET);
}

//*************************************************************************
//Voreinstellung fuer Weather Monitoring

void MyBME280::setWeatherMonitoring()
{
  mode = BME280_FORCED_MODE;
  setMode(mode);
  
  osrs_p = BME280_P_OVERSAMPLING_x1; //Pressure oversampling x1
  setPressOversampling(osrs_p);
  
  osrs_t = BME280_T_OVERSAMPLING_x1; //Temperature oversampling x1
  setTempOversampling(osrs_t);
  
  osrs_h = BME280_H_OVERSAMPLING_x1; //Humidity oversampling x1
  setHumidityOversampling(osrs_h);
  
  /*Keine Bedeutung bei WeatherMonitoring (Forced Mode)
  t_sb =  BME280_STANDBY_TIME_62p5;
  setStandbyTime(t_sb);
  */
  
  filter = BME280_FILTER_COEFF_0; //Filter aus
  setFilterCoeff(filter);
}

//*************************************************************************
//Voreinstellung fuer Humidity Sensing (Luftfeuchtemessung)

void MyBME280::setHumiditySensing()
{
  mode = BME280_FORCED_MODE;
  setMode(mode);
  
  osrs_p = BME280_P_OVERSAMPLING_x0; //Pressure oversampling x0
  setPressOversampling(osrs_p);
  
  osrs_t = BME280_T_OVERSAMPLING_x1; //Temperature oversampling x1
  setTempOversampling(osrs_t);
  
  osrs_h = BME280_H_OVERSAMPLING_x1; //Humidity oversampling x1
  setHumidityOversampling(osrs_h);
  
  /*Keine Bedeutung bei Humidity Sensing (Forced Mode)
  t_sb =  BME280_STANDBY_TIME_62p5;
  setStandbyTime(t_sb);
  */
  
  filter = BME280_FILTER_COEFF_0; //Filter aus
  setFilterCoeff(filter);
}

//*************************************************************************
//Voreinstellung fuer Indoor Navigation

void MyBME280::setIndoorNavigation()
{
  mode = BME280_NORMAL_MODE;
  setMode(mode);
    
  osrs_p = BME280_P_OVERSAMPLING_x16; //Pressure oversampling x16
  setPressOversampling(osrs_p);
  
  osrs_t = BME280_T_OVERSAMPLING_x2; //Temperature oversampling x2
  setTempOversampling(osrs_t);
  
  osrs_h = BME280_H_OVERSAMPLING_x1; //Humidity oversampling x1
  setHumidityOversampling(osrs_h);
  
  t_sb = BME280_STANDBY_TIME_0p5; //Standby Time 0,5 ms
  setStandbyTime(t_sb);
  
  filter = BME280_FILTER_COEFF_16; //Filter-Koeffizient = 16 (22 Stufen fuer 75%)
  setFilterCoeff(filter);
}

//*************************************************************************
//Voreinstellung fuer Gaming

void MyBME280::setGaming()
{
  mode = BME280_NORMAL_MODE;
  setMode(mode);
  
  osrs_p = BME280_P_OVERSAMPLING_x4; //Pressure oversampling x4
  setPressOversampling(osrs_p);
  
  osrs_t = BME280_T_OVERSAMPLING_x1; //Temperature oversampling x1
  setTempOversampling(osrs_t);
  
  osrs_h = BME280_H_OVERSAMPLING_x0; //Humidity oversampling x0
  setHumidityOversampling(osrs_h);
  
  t_sb = BME280_STANDBY_TIME_0p5; //Standby Time 0,5 ms
  setStandbyTime(t_sb);
  
  filter = BME280_FILTER_COEFF_16; //Filter-Koeffizient = 16 (22 Stufen fuer 75%)
  setFilterCoeff(filter);
}