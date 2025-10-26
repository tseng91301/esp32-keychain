#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <MadgwickAHRS.h>

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

Adafruit_MPU6050 *mpu;
Madgwick *filter;
unsigned long update_interval = 50000; // us
unsigned long lastMicros = 0;

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

  printf("Initializing TFT...\n");
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft = new Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
  tft->setTextWrap(false);
  d = new Display(tft, 160, 180);
  d->init(2);

  list = new DisplayList(d);
  list->init(0, 0, 180, 160);
  list->append(new DisplayText("MPU6050", 2, ST7735_YELLOW, d));

  mpu = new Adafruit_MPU6050();
  if (!mpu->begin()) {
    list->append(new DisplayText("MPU6050 initialization Failed...", 1, ST7735_RED, d));
    list->append(new DisplayText("Check connection and restart.", 1, ST7735_YELLOW, d));
    list->display();
    while (1) {
      delay(10);
    }
  }
  filter = new Madgwick();
  filter->begin(100);
  lastMicros = micros();

  //setupt motion detection
  mpu->setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu->setMotionDetectionThreshold(1);
  mpu->setMotionDetectionDuration(1);
  
  list->append(new DisplayText("Orientation:", 2, ST7735_GREEN, d));
  list->append(new DisplayText("0.0, 0.0, 0.0", 1, ST7735_GREEN, d));
  list->display();

  delay(500);
}

void loop() {
  float roll, pitch, heading;
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu->getEvent(&a, &g, &temp);

  // NOTE: Madgwick impl 要求 gyro in rad/s; your Adafruit g.gyro is in rad/s already? 
  // (Adafruit_Sensor's gyro often gives rad/s) — 若是 deg/s 要換算。
  // 下面假設 g.gyro.* 已經是 rad/s (如果是 deg/s，乘 PI/180)
  filter->updateIMU(g.gyro.x, g.gyro.y, g.gyro.z, a.acceleration.x, a.acceleration.y, a.acceleration.z);

  unsigned long now = micros();
  if ((now - lastMicros) < update_interval) return;
  lastMicros = now;

  // print the heading, pitch and roll
  roll = filter->getRoll();
  pitch = filter->getPitch();
  heading = filter->getYaw();

  printf("Orientation: ");
  printf("%.3f, ", heading);
  printf("%.3f, ", pitch);
  printf("%.3f\n", roll);

  DisplayText *t1 = (DisplayText*)list->get(2);
  t1->setText(String(heading) + ", " + String(pitch) + ", " + String(roll));
  list->display();
}
