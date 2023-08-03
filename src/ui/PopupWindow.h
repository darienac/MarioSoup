#pragma once

#include "ui/ui.h"

class PopupWindow : public IUIElement {
    private:
    const char* label;
    int width;
    int height;
    IUIElement** elements;
    int numElements;

    bool isHovered = false;

    public:
    PopupWindow(const char* label, int width, int height, IUIElement** elements, int numElements): label(label), width(width), height(height), elements(elements), numElements(numElements) {}

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        int x0 = (gameWidth - width * 8) / 2;
        int y0 = (gameHeight - height * 8) / 2;
        int x1 = x0 + width * 8;
        int y1 = y0 + height * 8;

        isHovered = (x >= x0 && x < x1 && y >= y0 && y < y1);
        if (isHovered) {
            for (int i = 0; i < numElements; i++) {
                int ex = elements[i]->getX();
                int ey = elements[i]->getY();
                elements[i]->hover(x - x0 - ex, y - y0 - ey, gameWidth, gameHeight);
            }
        }

        return isHovered;
    }
    void click() {
        for (int i = 0; i < numElements; i++) {
            elements[i]->click();
        }
    }
    void charInput(int codepoint) {
        for (int i = 0; i < numElements; i++) {
            elements[i]->charInput(codepoint);
        }
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    const char* getLabel() {
        return label;
    }

    IUIElement** getElements() {
        return elements;
    }

    int getNumElements() {
        return numElements;
    }
};