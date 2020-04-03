#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "Arduino.h"
#include "images.h"

#include <GxEPD.h>
#include <GxGDEW029T5/GxGDEW029T5.h>      // 2.9" b/w IL0373
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <Wire.h>
#include <RFReceiver.h>

#include GxEPD_BitmapExamples

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

/*
  /usr/local/esp32/arduino-core/variants/lolin32/pins_arduino.h
                       io   display
  CS   = 5  (= SS)     *
  DC   = 13            *
  RST  = 22            *    *
  MISO = 19 (= BUSY)        *
  MOSI = 23 (= DIN)
  CLK  = 18
  GND
  3V3

  default:
  MOSI	MISO	CLK	CS
  23	19	    18	5
 */
GxIO_Class io(SPI, 5, 13, 22);   // CS, DC, RST
GxEPD_Class display(io, 16, 22); // RST, MISO

RFReceiver receiver(2);


#if CONFIG_AUTOSTART_ARDUINO

void say(const char what[]){
  display.setRotation(3);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(20,20);
  display.println(what);
  display.update();
}

void clean() {
  display.fillScreen(GxEPD_WHITE);
  display.update();
}

void drawUI() {
  // setup basic settings
  display.setRotation(3);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);

  // outer frame
  display.fillScreen(GxEPD_BLACK);
  display.fillRect(2, 2, display.width() -4, display.height() -4, GxEPD_WHITE);

  // separate button displays from view port
  display.fillRect(2, display.height() - 30,
                   display.width() - 2, 2,
                   GxEPD_BLACK);

  // button separator 1
  display.fillRect(display.width() / 4,
                   display.height() - 30,
                   2, 30, GxEPD_BLACK
                   );

  // button separator 2
  display.fillRect(display.width() / 2,
                   display.height() - 30,
                   2, 30, GxEPD_BLACK
                   );

  // button separator 3
  display.fillRect((display.width() / 2) + (display.width() / 3) - 30,
                   display.height() - 30,
                   2, 30, GxEPD_BLACK
                   );

  // fake buttons
  display.setCursor(8, display.height() - 10);
  display.println("MENU     <     >     OK");
}

void setup(){
  Serial.begin(115200);
  receiver.begin();
  display.init(115200); // enable diagnostic output on Serial
  delay(50);
  drawUI();
  pinMode(4, OUTPUT);
}

void loop(){
  char msg[MAX_PACKAGE_SIZE];
  byte senderId = 0;
  byte packageId = 0;

  display.fillScreen(GxEPD_WHITE);
  display.update();

  byte len = receiver.recvPackage((byte *)msg, &senderId, &packageId);

  if (len) {
    Serial.println("");
    Serial.print("Package: ");
    Serial.println(packageId);
    Serial.print("Sender: ");
    Serial.println(senderId);
    Serial.print("Message: ");
    Serial.println(msg);
  }

  delay(20000);
}

#else

void loopTask(void *pvParameters)
{
    setup();
    for(;;) {
        micros(); //update overflow
        loop();
    }
}

void app_main()
{
    initArduino();
    xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
}

#endif
