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
    bool isHover = false;
    bool isDragScrolling = false;
    int dragScrollOffset;
    int hoverX, hoverY;

    void limitScroll() {
        if (scrollY < 0) {
            scrollY = 0;
        } else if (scrollY > innerHeight - height * 8) {
            scrollY = (innerHeight - height * 8);
        }
    }

    public:
    // Intervals of 8 pixels for width and height
    ObjectPicker(int width, int height): width(width), height(height) {

    }

    void hover(int x, int y, int gameWidth, int gameHeight) {
        hoverX = x;
        hoverY = y;
        isHover = (hoverX >= 0 && hoverX < width * 8 && hoverY >= 0 && hoverY < height * 8);

        if (isDragScrolling) {
            int newScrollBarY = (height * 8 - hoverY) - dragScrollOffset;

            int scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY;
            getScrollBar(scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY);

            scrollY = newScrollBarY * scrollYMax / scrollBarMax;
            limitScroll();
        }
    }
    void click() {
        isDragScrolling = false;
    }

    void scroll(double xOff, double yOff) {
        if (isHover) {
            scrollY += yOff * -10.0;
            limitScroll();
        }
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