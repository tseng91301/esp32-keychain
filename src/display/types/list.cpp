#pragma once
#include <Arduino.h>
#include "../display.h"
#include "list.h"
#include "basic.h"

void DisplayList::init(int x, int y, uint16_t h, uint16_t w) {
    this->position.x1 = 0;
    this->position.y1 = 0;
    this->size.height = h;
    this->size.width = w;
    this->items.clear();
    this->output_display->tft->fillRect(x, y, w, h, ST77XX_BLACK);
}

void DisplayList::clear_screen() {
    this->output_display->tft->fillRect(this->position.x1, this->position.y1, this->size.width, this->size.height, ST77XX_BLACK);
}