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
    this->output_display->tft->setTextColor(this->color);
    this->output_display->tft->setTextSize(this->size);
    this->output_display->tft->setCursor(x, y);
    this->output_display->tft->print(this->text);
}