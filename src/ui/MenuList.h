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
    bool allowCloseList = true;

    public:
    MenuList(MenuListButton* buttons, int numButtons, int width): buttons(buttons), numButtons(numButtons), width(width) {}

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        if (x < 0 || x >= (width * 8)) {
            numHovered = -1;
            allowCloseList = true;
            return false;
        }
        if (y >= -4 || y < -4 - (numButtons * 8)) {
            numHovered = -1;
            allowCloseList = true;
            return false;
        }

        numHovered = -((y + 5) / 8);
        allowCloseList = buttons[numHovered].isAllowCloseList();

        return true;
    }
    
    void click() {
        if (numHovered != -1) {
            buttons[numHovered].click();
            if (buttons[numHovered].isAllowCloseList()) {
                numHovered = -1;
            }
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

    bool isAllowCloseList() {
        return allowCloseList;
    }

    void setPointer(void* pointer) {
        for (int i = 0; i < numButtons; i++) {
            buttons[i].setPointer(pointer);
        }
    }

    void listOpen() {
        for (int i = 0; i < numButtons; i++) {
            buttons[i].listOpen();
        }
    }
};