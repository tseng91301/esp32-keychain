#ifndef DISPLAYTYPE_BASIC_H
#define DISPLAYTYPE_BASIC_H
#include <Arduino.h>
#include <Adafruit_ST7735.h>

class Display;

struct PositionProperties {
    int x;
    int y;
    int width;
    int height;
};

class DisplayText {
    /* 文字方塊 */
    public:
        String text;
        int size;
        int color;
        int background_color;
        PositionProperties position;
        Display *output_display; // 綁定的 display 實體
        DisplayText(String t, int s=1, int c=ST7735_WHITE, Display *d=NULL) {
            this->setText(t, s, c);
            this->output_display = d;
        }
        void setText(String t, int s=1, int c=ST7735_WHITE) {
            this->text = t;
            this->size = s;
            this->color = c;
        }
        void display(int x, int y);
};
#endif