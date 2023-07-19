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
    int width;
    int height;
    int innerHeight = 1024;
    int scrollY = 0;

    public:
    // Intervals of 8 pixels for width and height
    ObjectPicker(int width, int height): width(width), height(height) {

    }

    void hover(int x, int y, int gameWidth, int gameHeight) {}
    void click() {}
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