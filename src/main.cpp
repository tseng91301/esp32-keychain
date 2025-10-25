#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include <display/display.h>

// === SPI 腳位設定 (ESP32-C3 SuperMini) ===
#define TFT_CS   7
#define TFT_DC   10
#define TFT_RST  3
#define TFT_MOSI 2
#define TFT_SCLK 6

// 建立螢幕物件
Adafruit_ST7735 *tft;
Display *d;

DisplayList *list;

uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// HSV → RGB
void HSVtoRGB(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
  float c = v * s;
  float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
  float m = v - c;
  float r1, g1, b1;

  if (h < 60)      { r1=c; g1=x; b1=0; }
  else if (h<120)  { r1=x; g1=c; b1=0; }
  else if (h<180)  { r1=0; g1=c; b1=x; }
  else if (h<240)  { r1=0; g1=x; b1=c; }
  else if (h<300)  { r1=x; g1=0; b1=c; }
  else             { r1=c; g1=0; b1=x; }

  r = (r1+m)*255;
  g = (g1+m)*255;
  b = (b1+m)*255;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing TFT...");
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft = new Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
  tft->setTextWrap(false);
  d = new Display(tft, 160, 180);
  d->init(2);
  list = new DisplayList(d);
  list->init(0, 0, 180, 160);
  list->append(new DisplayText("Hello world", 2, ST7735_RED, d));
  list->append(new DisplayText("0", 3, ST7735_YELLOW, d));
  list->append(new DisplayText("0", 2, ST7735_CYAN, d));

  delay(500);
}

void loop() {
  static int now = 0;
  static int now2 = 0;
  DisplayText *t = (DisplayText*)list->get(1);
  t->setText(String(now++));
  DisplayText *t2 = (DisplayText*)list->get(2);
  t2->setText(String(now2+=2));
  list->display();
  delay(50);
}
