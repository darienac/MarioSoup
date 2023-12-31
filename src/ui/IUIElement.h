#pragma once

enum class UIElementType {
    NONE,
    BUTTON,
    TEXT_INPUT,
    TEXT_REGION
};

// All elements should assume x and y are relative to the element's own coordinate system (despite setting an x or y position)
class IUIElement {
    protected:
    int x = 0;
    int y = 0;

    public:
    virtual bool hover(int x, int y, int gameWidth, int gameHeight) {
        return false;
    };
    virtual void mouseDown() {};
    virtual void mouseRightDown() {};
    virtual void mouseMiddleDown() {};
    virtual void click() {};
    virtual void clickRight() {};
    virtual void clickMiddle() {};
    virtual void scroll(double xOff, double yOff) {};
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

    virtual ~IUIElement() {}
};