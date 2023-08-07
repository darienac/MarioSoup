#pragma once

#include "ui/ui.h"

class PopupWindow : public IUIElement {
    public:
    typedef void (*cancel_callback)(PopupWindow* popup);

    private:
    const char* label;
    int width;
    int height;
    IUIElement** elements;
    int numElements;

    bool canCancel = false;
    cancel_callback cancelCallback = nullptr;
    void* pointer = nullptr;

    bool isHovered = false;

    public:
    PopupWindow(const char* label, int width, int height, IUIElement** elements, int numElements): label(label), width(width), height(height), elements(elements), numElements(numElements) {}

    void setCanCancel(bool canCancel) {
        this->canCancel = canCancel;
    }

    bool getCanCancel() {
        return canCancel;
    }

    void setCancelCallback(cancel_callback callback) {
        this->cancelCallback = callback;
    }

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        int x0 = (gameWidth - width * 8) / 2;
        int y0 = (gameHeight - height * 8) / 2;
        int x1 = x0 + width * 8;
        int y1 = y0 + height * 8;

        isHovered = (x >= x0 && x < x1 && y >= y0 && y < y1);
        if (canCancel && isHovered && x >= x1 - 8 && y >= y1 - 8) {
            // X button counts as outside the box
            isHovered = false;
        }
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
        if (isHovered) {
            for (int i = 0; i < numElements; i++) {
                elements[i]->click();
            }
        } else if (cancelCallback != nullptr) {
            cancelCallback(this);
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

    void setPointer(void* pointer) {
        this->pointer = pointer;
    }

    void* getPointer() {
        return pointer;
    }
};