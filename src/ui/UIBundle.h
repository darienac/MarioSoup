#pragma once

#include "ui/ui.h"

class UIBundle: public IUIElement {
    private:
    IUIElement** elements;
    int numElements;

    public:
    UIBundle(IUIElement** elements, int numElements): elements(elements), numElements(numElements) {}

    virtual bool hover(int x, int y, int gameWidth, int gameHeight) {
        bool hovered = false;
        for (int i = 0; i < numElements; i++) {
            IUIElement* element = elements[i];
            hovered |= element->hover(x - element->getX(), y - element->getY(), gameWidth, gameHeight);
        }

        return hovered;
    }

    virtual void mouseDown() {
        for (int i = 0; i < numElements; i++) {
            elements[i]->mouseDown();
        }
    }

    virtual void click() {
        for (int i = 0; i < numElements; i++) {
            elements[i]->click();
        }
    }

    virtual void scroll(double xOff, double yOff) {
        for (int i = 0; i < numElements; i++) {
            elements[i]->scroll(xOff, yOff);
        }
    }

    virtual void charInput(int codepoint) {
        for (int i = 0; i < numElements; i++) {
            elements[i]->charInput(codepoint);
        }
    }
};