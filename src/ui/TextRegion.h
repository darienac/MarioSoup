#pragma once

#include "ui/IUIElement.h"

class TextRegion: public IUIElement {
    private:
    const char* text;
    int maxWidth;

    public:
    TextRegion(const char* text, int maxWidth): text(text), maxWidth(maxWidth) {}

    const char* getText() {
        return text;
    }

    void setText(const char* text) {
        this->text = text;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getMaxWidth() {
        return maxWidth;
    }

    UIElementType getElementType() override {
        return UIElementType::TEXT_REGION;
    }
};