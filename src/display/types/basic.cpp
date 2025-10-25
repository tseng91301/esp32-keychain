#pragma once
#include "basic.h"
#include "../display.h"

void DisplayText::display(int x, int y) {
    if (this->output_display == NULL) return; // 未綁定 display
    // printf("Displaying...\n");
    // printf("Text: %s\n", this->text.c_str());
    // printf("Size: %d\n", this->size);
    // printf("Color: %d\n", this->color);
    // printf("X: %d, Y: %d\n", x, y);
    if (this->needFlush) {
        SizeProperties size = this->getSize();
        this->output_display->tft->fillRect(x, y, size.width, size.height, ST77XX_BLACK);
        this->needFlush = false;
    }
    this->output_display->tft->setTextColor(this->color);
    this->output_display->tft->setTextSize(this->textSize);
    this->output_display->tft->setCursor(x, y);
    this->output_display->tft->print(this->text);
}
SizeProperties DisplayText::getSize() {
    // 取得占用寬高 (pixels)
    int16_t x1, y1;
    uint16_t w, h;
    this->output_display->tft->setTextSize(this->textSize);
    this->output_display->tft->getTextBounds(this->text, 0, 0, &x1, &y1, &w, &h);
    return {w, h};
}