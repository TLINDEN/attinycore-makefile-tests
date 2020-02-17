
//*************************************************************************
// steffen wrote this stuff for mikrocontrollerspielwiese.de 
// have fun!
//*************************************************************************

#include "delay.h"
#include "digital.h"
#include "analog.h"


#define LED    1

int main(void){
  pinMode(LED,    OUTPUT);
       
  while (1) {
      digitalWrite(LED, HIGH);
      _delay_ms(255);
      digitalWrite(LED, LOW);
      _delay_ms(255);
  }
  
  return 0;
}

