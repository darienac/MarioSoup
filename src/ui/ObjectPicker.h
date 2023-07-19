#pragma once

#include "ui/ui.h"
#include "game/GameObject.h"

class ObjectPickerGroup {
    private:
    public:
    ObjectPickerGroup(ObjectPickerGroup* groups, int numGroups, GameObject* items, int numItems) {

    }
};

class ObjectPicker: public IUIElement {
    private:
    int width, height;
    int innerHeight = 1024;
    int scrollY = 0;
    bool isDragScrolling = false;
    int dragScrollOffset;
    int hoverX, hoverY;

    public:
    // Intervals of 8 pixels for width and height
    ObjectPicker(int width, int height): width(width), height(height) {

    }

    void hover(int x, int y, int gameWidth, int gameHeight) {
        hoverX = x;
        hoverY = y;

        if (isDragScrolling) {
            int newScrollBarY = (height * 8 - hoverY) - dragScrollOffset;

            int scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY;
            getScrollBar(scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY);

            scrollY = newScrollBarY * scrollYMax / scrollBarMax;
            if (scrollY < 0) {
                scrollY = 0;
            } else if (scrollY > scrollYMax) {
                scrollY = scrollYMax;
            }
        }
    }
    void click() {
        isDragScrolling = false;
    }
    void mouseDown() override {
        if (hoverX >= width * 8 - 4 && hoverX < width * 8 && hoverY >= 0 && hoverY < height * 8) {
            isDragScrolling = true;
            int scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY;
            getScrollBar(scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY);
            if (hoverY >= height * 8 - scrollBarHeight - scrollBarY && hoverY < height * 8 - scrollBarY) {
                dragScrollOffset = (height * 8 - hoverY) - scrollBarY;
            } else {
                dragScrollOffset = 0;
            }
        }
    }
    void charInput(int codepoint) {}

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    int getInnerHeight() {
        return innerHeight;
    }

    int getScrollY() {
        return scrollY;
    }

    void getScrollBar(int& scrollBarHeight, int& scrollYMax, int& scrollBarMax, int& scrollBarY) {
        int width = getWidth() * 8;
        int height = getHeight() * 8;
        int innerHeight = getInnerHeight();
        int scrollY = getScrollY();

        scrollBarHeight = height * height / innerHeight;
        scrollYMax = innerHeight - height;
        scrollBarMax = height - scrollBarHeight;
        scrollBarY = scrollY * scrollBarMax / scrollYMax;
    }
};