#pragma once

#include "ui/ui.h"

class UIBundle: public IUIElement {
    private:
    IUIElement** elements;
    int numElements;

    public:
    UIBundle(IUIElement** elements, int numElements): elements(elements), numElements(numElements) {}

    void hover(int x, int y, int gameWidth, int gameHeight) {
        for (int i = 0; i < numElements; i++) {
            IUIElement* element = elements[i];
            element->hover(x - element->getX(), y - element->getY(), gameWidth, gameHeight);
        }
    }

    void mouseDown() {
        for (int i = 0; i < numElements; i++) {
            elements[i]->mouseDown();
        }
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
};