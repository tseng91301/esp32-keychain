#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#include "types/basic.h"
#include "types/list.h"
struct DisplayMode {
    
};
class DisplayLayer {
/* 定義每一層的內容 */
};
class DisplayLayers {
/* 管理顯示的每個層次，做堆疊和穿插等 */
};
class Display
{
    public:
        Adafruit_ST7735 *tft;
        int width;
        int height;
        Display(Adafruit_ST7735 *t, int w, int h) {
            this->tft = t;
            this->width = w;
            this->height = h;
        }
        void init(int r=0) {
            this->tft->initR(INITR_BLACKTAB);  // 如果顏色顛倒可試 INITR_GREENTAB
            this->tft->fillScreen(ST77XX_BLACK);
            this->tft->setRotation(r);
        }
};
#endif