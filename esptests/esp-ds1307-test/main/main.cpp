/* DS1307 Test

*/

// c libs
#include <stdio.h>
#include <time.h>
#include <stdint.h>

// esp32 libs
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "Arduino.h"

// hw interface libs
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>

// C++ libs
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

bool dst(time_t t) {
  /*
   * calculate daylight savings time.
   * returns true if given time_t is within sommertime.
   *
   * via: http://www.mikrocontroller.net/attachment/8391/TIME.C
   */
  int monat     = month(t);
  int tag       = day(t);
  int wochentag = weekday(t);
  int stunde    = hour(t);
  
  if( monat < 3 || monat > 10 ) {
    return false;
  }
  
  if( tag - wochentag >= 25 && (wochentag || stunde >= 2) ) {
    if( monat == 10 ) {
       return false;
     }
  }
  else {
    if( monat == 3 ) {
      return false;
    }
  }

  return true;
}

time_t gettimeofday() {
  /* return current time with dst() applied */
  time_t t = now();
  if(dst(t)) {
    return t + 3600; 
  }
  else {
    return t;
  }
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

time_t time_when_compiled() {
  tm t;
  string str_month;
  int day, year, month, hour, min, sec;
    
  string datestr = __DATE__;
  string timestr = __TIME__;

  istringstream iss_date( datestr );
  iss_date >> str_month >> day >> year;

  if     ( str_month == "Jan" ) month = 1;
  else if( str_month == "Feb" ) month = 2;
  else if( str_month == "Mar" ) month = 3;
  else if( str_month == "Apr" ) month = 4;
  else if( str_month == "May" ) month = 5;
  else if( str_month == "Jun" ) month = 6;
  else if( str_month == "Jul" ) month = 7;
  else if( str_month == "Aug" ) month = 8;
  else if( str_month == "Sep" ) month = 9;
  else if( str_month == "Oct" ) month = 10;
  else if( str_month == "Nov" ) month = 11;
  else if( str_month == "Dec" ) month = 12;
  else exit(-1);

  for( string::size_type pos = timestr.find( ':' ); pos != string::npos; pos = timestr.find( ':', pos ) )
    timestr[ pos ] = ' ';
  istringstream iss_time( timestr );
  iss_time >> hour >> min >> sec;

  t.tm_mon = month-1;
  t.tm_mday = day;
  t.tm_year = year - 1900;
  t.tm_hour = hour - 1;
  t.tm_min = min;
  t.tm_sec = sec;
  return mktime(&t);
}

void setClock(time_t t) {
  if (t != 0) {
    RTC.set(t);
    setTime(t);          
  }
}

void setup()  {
  Serial.begin(9600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");

  setClock(time_when_compiled());
}

void loop()
{
  if (timeStatus() == timeSet) {
    digitalClockDisplay();
  } else {
    Serial.println("The time has not been set.  Please run the Time");
    Serial.println("TimeRTCSet example, or DS1307RTC SetTime example.");
    Serial.println();
    delay(4000);
  }
  delay(1000);
}
