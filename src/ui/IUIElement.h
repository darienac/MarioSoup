#pragma once

class IUIElement {
    public:
    virtual void hover(int x, int y, int gameHeight) = 0;
    virtual void click() = 0;
};