#pragma once

enum class UIElementType {
    NONE,
    BUTTON,
    TEXT_INPUT
};

// All elements should assume x and y are relative to the element's own coordinate system (despite setting an x or y position)
class IUIElement {
    protected:
    int x = 0;
    int y = 0;

    public:
    virtual void hover(int x, int y, int gameWidth, int gameHeight) {};
    virtual void mouseDown() {};
    virtual void click() {};
    virtual void charInput(int codepoint) {};
    virtual UIElementType getElementType() {
        return UIElementType::NONE;
    }

    void setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void getPosition(int& x, int& y) {
        x = this->x;
        y = this->y;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }
};