#ifndef DISPLAYTYPE_BASIC_H
#define DISPLAYTYPE_BASIC_H
#include <Arduino.h>
#include <Adafruit_ST7735.h>

class Display;

struct PositionProperties {
    int16_t x1;
    int16_t y1;
};

struct SizeProperties {
    int width;
    int height;
};

class DisplayItem {
    public:
        Display *output_display; // 綁定的 display 實體
        DisplayItem(Display *d=NULL) {
            this->output_display = d;
        }
        virtual void display(int x, int y) = 0;
        virtual SizeProperties getSize() = 0;
};

class DisplayText : public DisplayItem {
    /* 文字方塊 */
    private:
        String text;
        int textSize;
        int color;
        int background_color;
        bool needFlush = false; // 是否需要刷新
    public:
        Display *output_display; // 綁定的 display 實體
        DisplayText(String t, int s=1, int c=ST7735_WHITE, Display *d=NULL) {
            this->setTextProperty(t, s, c);
            this->output_display = d;
            this->needFlush = true;
        }
        void setTextProperty(String t, int s=1, int c=ST7735_WHITE) {
            this->text = t;
            this->textSize = s;
            this->color = c;
            this->needFlush = true;
        }
        String getText() { return this->text; }
        void setText(String t) {
            this->text = t;
            this->needFlush = true;
        }
        int getTextSize() { return this->textSize; }
        int getTextColor() { return this->color; }
        void setTextColor(int c) {
            this->color = c;
            this->needFlush = true;
        }
        void setTextSize(int s) {
            this->textSize = s;
            this->needFlush = true;
        }
        SizeProperties getSize();
        void display(int x, int y);
};
#endif