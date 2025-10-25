#ifndef DISPLAYTYPE_LIST_H
#define DISPLAYTYPE_LIST_H
#include <Arduino.h>
#include <vector>
#include "basic.h"

class Display;

class DisplayList {
    private:
        std::vector<void*> items; // 每個顯示元素 (pointer) 組成的陣列
        PositionProperties position; // 位置
        SizeProperties size; // 大小
    public:
        Display *output_display;
        DisplayList(Display *d) {
            this->output_display = d;
        }
        DisplayList() {}
        void init(int x, int y, uint16_t h, uint16_t w);
        void append(void *item) {
            this->items.push_back(item);
        }
        void insert(void *item, int index=-1) {
            if (index == -1) items.push_back(item);
            else if (index < 0) index = this->items.size() + index;
            this->items.insert(this->items.begin() + index, item);
        }
        void clear_screen();
        void display() {
            uint16_t y = position.y1;
            for (void *item : this->items) {
                uint16_t y2 = y + ((DisplayItem*)item)->getSize().height;
                if (y2 > this->size.height+position.y1) break;
                ((DisplayItem*)item)->display(0, y);
                y = y2;
            }
        }
        void* get(int index=-1) {
            if (index == -1) return this->items.back();
            else if (index < 0) index = this->items.size() + index;
            return this->items[index];
        }
        void remove(void *item) {
            this->items.erase(std::remove(this->items.begin(), this->items.end(), item), this->items.end());
        }
        void remove(int index=-1) {
            if (index == -1) items.pop_back();
            else if (index < 0) index = this->items.size() + index;
            this->items.erase(this->items.begin() + index);
        }
        uint16_t numElements() {
            return this->items.size();
        }
        void clear() {
            this->items.clear();
        }
        ~DisplayList() {
            this->clear();
        }

};
#endif