#pragma once

#include "ui/ui.h"

class Button: public IUIElement {
    private:
    const char* label;
    int width;
    void (*callback)(Button* button, UIButtonValue& value);
    UIButtonValue value = {};

    bool hovered = false;

    public:
    Button(const char* label, int width, void (*callback)(Button* button, UIButtonValue& value)): label(label), width(width), callback(callback) {}

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

    bool isHovered() {
        return hovered;
    }
};