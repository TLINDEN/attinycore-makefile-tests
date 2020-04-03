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
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

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

// BME280 stuff
#define SEALEVELPRESSURE_HPA (1013.25)

#define BME_SCK  18  // == CLK?
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS    5

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);





#if CONFIG_AUTOSTART_ARDUINO



#if defined(_GxGDEW029T5_H_)
void showBitmapExample()
{
  display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1));
  delay(2000);
#if !defined(__AVR)
  display.drawExampleBitmap(BitmapExample2, sizeof(BitmapExample2));
  delay(2000);
  display.drawExampleBitmap(BitmapExample3, sizeof(BitmapExample3));
  delay(2000);
#endif
  delay(5000);
  display.fillScreen(GxEPD_WHITE);
  display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
  display.update();
}
#endif

void showFont(const char name[], const GFXfont* f)
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println(name);
  display.println(" !\"#$%&'()*+,-./");
  display.println("0123456789:;<=>?");
  display.println("@ABCDEFGHIJKLMNO");
  display.println("PQRSTUVWXYZ[\\]^_");
#if defined(HAS_RED_COLOR)
  display.setTextColor(GxEPD_RED);
#endif
  display.println("`abcdefghijklmno");
  display.println("pqrstuvwxyz{|}~ ");
  display.update();
  delay(5000);
}

void showFontCallback()
{
  const char* name = "FreeMonoBold9pt7b";
  const GFXfont* f = &FreeMonoBold9pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println(name);
  display.println(" !\"#$%&'()*+,-./");
  display.println("0123456789:;<=>?");
  display.println("@ABCDEFGHIJKLMNO");
  display.println("PQRSTUVWXYZ[\\]^_");
#if defined(HAS_RED_COLOR)
  display.setTextColor(GxEPD_RED);
#endif
  display.println("`abcdefghijklmno");
  display.println("pqrstuvwxyz{|}~ ");
}

void drawCornerTest()
{
  display.drawCornerTest();
  delay(5000);
  uint8_t rotation = display.getRotation();
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.fillScreen(GxEPD_WHITE);
    display.fillRect(0, 0, 8, 8, GxEPD_BLACK);
    display.fillRect(display.width() - 18, 0, 16, 16, GxEPD_BLACK);
    display.fillRect(display.width() - 25, display.height() - 25, 24, 24, GxEPD_BLACK);
    display.fillRect(0, display.height() - 33, 32, 32, GxEPD_BLACK);
    display.update();
    delay(5000);
  }
  display.setRotation(rotation); // restore
}

/*
  void drawPixel(uint32_t x, uint32_t y, uint32_t color);
  void drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
  void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
*/

void say(const char what[]){
  /*
  display.setRotation(3);
  display.fillScreen(GxEPD_BLACK);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeMonoBold9pt7b);
  */
  
  display.setCursor(20,20);
  display.println(what);
  display.update();
}

void clock(const char time[], const char weather[]) {
  display.setFont(&FreeMonoBold24pt7b);
  display.setCursor(10, 50);
  display.println(time);
  display.setFont(&FreeMonoBold9pt7b);
  display.setCursor(10, 80);
  display.println(weather);
  display.update();
}

void clean() {
  // fail display.eraseDisplay();
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
  //Serial.begin(115200);
  unsigned status;
  display.init(115200); // enable diagnostic output on Serial
  delay(50);
  drawUI();
  status = bme.begin();
  if(! status) {
    say("BME280 failed");
  }
  else {
    say("BME280 initialized");
  }
  pinMode(4, OUTPUT);
}

void loop(){
  int i;
  char zeit[9];
  digitalWrite(4, HIGH);
  /*
  display.setRotation(3);
  display.fillScreen(GxEPD_BLACK);
  display.drawBitmap(0, 0, finlay, 128, 128, GxEPD_WHITE);
  display.update();
  */
  //say("Hallo Leo!");
  //digitalWrite(4, LOW);
  //delay(10000);

  display.fillScreen(GxEPD_WHITE);
  display.update();
  
  delay(20000);
  /*
  drawUI();
  for(i=0; i<=60; i++) {
    sprintf(zeit, "11:49:%02d", i);
    drawUI();
    clock(zeit, "Pisswetter");
    delay(3600);
  }

  clean();
  delay(20000);
  */
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
