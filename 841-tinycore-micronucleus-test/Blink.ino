/* -*-c++-*-
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
 */

#define LED LED_BUILTIN

void blinken(int howlong) {
  digitalWrite(LED, HIGH);
  delay(howlong);
  digitalWrite(LED, LOW);
  delay(howlong);
}

void lang() {
  blinken(1000);
}

void kurz() {
  blinken(500);
}

void sos() {
  int i=0;

  for(i=0; i<3; i++)
    lang();

  for(i=0; i<3; i++)
    kurz();

 for(i=0; i<3; i++)
    lang();  
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.print("Press any key: ");
}

void loop() {
  if (Serial.available()) {
    Serial.println("blinked\n");
  }
  sos();
  delay(3000);
}
