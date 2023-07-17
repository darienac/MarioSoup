#pragma once

#include <iostream>

#include "ui/IUIElement.h"

class MenuBar : public IUIElement {
    private:
    const char** labels;
    int numLabels;
    MenuList* lists;
    int numHovered = -1;
    int numSelected = -1;
    int width;

    public:
    MenuBar(const char** labels, int numLabels, MenuList* lists, int width) : MenuBar(labels, numLabels, lists, 0, 0, width) {}
    MenuBar(const char** labels, int numLabels, MenuList* lists, int x, int y, int width) {
        this->labels = labels;
        this->numLabels = numLabels;
        this->lists = lists;
        this->width = width;
        setPosition(x, y);
    }

    const char** getLabels() {
        return labels;
    }

    MenuList* getLists() {
        return lists;
    }

    int getNumLabels() {
        return numLabels;
    }

    int getNumHovered() {
        return numHovered;
    }

    int getNumSelected() {
        return numSelected;
    }

    int getWidth() {
        return width;
    }

    void hover(int x, int y, int gameWidth, int gameHeight) {
        if (numSelected != -1) {
            int xMax = 0;
            int xMin;
            for (int i = 0; i <= numSelected; i++) {
                xMin = xMax;
                xMax = xMin + 8 * (strlen(labels[i]) + 1);
            }
            lists[numSelected].hover(x - xMin - this->x, y - gameHeight + 16 - this->y, gameWidth, gameHeight);
        }
        if (y >= gameHeight - 16 + this->y && y < gameHeight - this->y) {
            int xMin = 0;
            for (int i = 0; i < numLabels; i++) {
                int xMax = xMin + 8 * (strlen(labels[i]) + 1);
                if (x >= xMin + this->x && x < xMax + this->x) {
                    numHovered = i;
                    return;
                }
                xMin = xMax;
            }
        }
        numHovered = -1;
    }

    void click() {
        if (numSelected != -1) {
            lists[numSelected].click();
            if (!lists[numSelected].isAllowCloseList()) {
                return;
            }
        }
        numSelected = numHovered;
    }
};