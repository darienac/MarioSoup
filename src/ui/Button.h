#pragma once

#include "ui/ui.h"

class Button: public IUIElement {
    public:
    typedef void (*button_callback)(Button* button, UIButtonValue& value);

    private:
    const char* label;
    int width;
    button_callback callback;
    UIButtonValue value = {};
    void* pointer;

    bool hovered = false;

    public:
    Button(const char* label, int width, button_callback callback): label(label), width(width), callback(callback) {}

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        hovered = !(x < 0 || x >= width * 8 || y < 0 || y >= 16);
        return hovered;
    }

    void click() {
        if (hovered) {
            if (callback != nullptr) {
                callback(this, value);
            }
        }
    }

    UIElementType getElementType() {
        return UIElementType::BUTTON;
    }

    const char* getLabel() {
        return label;
    }

    int getWidth() {
        return width;
    }

    void setCallback(button_callback callback) {
        this->callback = callback;
    }

    bool isHovered() {
        return hovered;
    }

    void setHovered(bool value) {
        hovered = value;
    }

    void setPointer(void* pointer) {
        this->pointer = pointer;
    }

    void* getPointer() {
        return pointer;
    }
};