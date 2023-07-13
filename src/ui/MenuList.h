#pragma once

#include "ui/ui.h"

// Assumed to be located at 0, 0 unless inside of MenuBar where hover is done using relative coordinates

class MenuList : public IUIElement {
    private:
    MenuListButton* buttons;
    int numButtons;
    int width;

    int numHovered = -1;
    int numSelected = -1;

    public:
    MenuList(MenuListButton* buttons, int numButtons, int width): buttons(buttons), numButtons(numButtons), width(width) {}

    void hover(int x, int y, int gameHeight) {
        if (x < 0 || x >= (width * 8)) {
            numHovered = -1;
            return;
        }
        if (y >= -4 || y < -4 - (numButtons * 8)) {
            numHovered = -1;
            return;
        }

        numHovered = -((y + 5) / 8);
    }
    
    void click() {
        if (numHovered != -1) {
            buttons[numHovered].click();
        }
    }

    MenuListButton* getButtons() {
        return buttons;
    }

    int getNumButtons() {
        return numButtons;
    }

    int getWidth() {
        return width;
    }

    int getNumHovered() {
        return numHovered;
    }
};