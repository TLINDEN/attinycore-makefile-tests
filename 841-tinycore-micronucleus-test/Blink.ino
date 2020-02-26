/* -*-c++-*-
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

/*
  Nanite841 Pinout from above:

         button
    PA5          PA6
    PA4          PA7
    PA3          PB2
RX0 PA2          PB2
TX0 PA1          PB1
    PA0          PB0
    GND          VCC
          usb

 */

#define LED LED_BUILTIN

void blinken(int howlong) {
  digitalWrite(LED, HIGH);
  delay(howlong);
  digitalWrite(LED, LOW);
  delay(howlong);
}

void lang() {
  blinken(500);
}

void kurz() {
  blinken(250);
}

void sos() {
  int i=0;

  Serial.println("long");
  for(i=0; i<3; i++)
    lang();

  Serial.println("short");
  for(i=0; i<3; i++)
    kurz();

  Serial.println("long");
 for(i=0; i<3; i++)
    lang();  
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.print("init");
}

void loop() {
  Serial.println("blinked");
  sos();
  delay(3000);
}
