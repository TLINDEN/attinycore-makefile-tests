
//*************************************************************************
// steffen wrote this stuff for mikrocontrollerspielwiese.de 
// have fun!
//*************************************************************************

#include "delay.h"
#include "digital.h"
#include "analog.h"
//#define BAUD_RATE 57600 //115200
//#include "serial.h" // serial Port (TX) defined in serial.S!

#define PU_BAUD_RATE 38400
#include <picoUART.h>

#define LED    PB4

void serOut(const char* str) {
  prints_P(str);
  /*
  while (*str)
    TxByte(*str++);
  */
}

int main(void){
  pinMode(LED,    OUTPUT);
       
  while (1) {
    serOut("Turning on LED\n");
    digitalWrite(LED, HIGH);
    _delay_ms(2550);
    serOut("Turning off LED\n");
    digitalWrite(LED, LOW);
    _delay_ms(2550);
  }
  
  return 0;
}

