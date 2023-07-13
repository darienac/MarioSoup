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

    public:
    MenuBar(const char** labels, int numLabels, MenuList* lists) {
        this->labels = labels;
        this->numLabels = numLabels;
        this->lists = lists;
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

    void hover(int x, int y, int gameHeight) {
        if (numSelected != -1) {
            int xMax = 0;
            int xMin;
            for (int i = 0; i <= numSelected; i++) {
                xMin = xMax;
                xMax = xMin + 8 * (strlen(labels[i]) + 1);
            }
            lists[numSelected].hover(x - xMin, y - gameHeight + 16, gameHeight);
            std::printf("list hover x: %d, y: %d\n", x - xMin, y - gameHeight + 16);
        }
        if (y >= gameHeight - 16 && y < gameHeight) {
            int xMin = 0;
            for (int i = 0; i < numLabels; i++) {
                int xMax = xMin + 8 * (strlen(labels[i]) + 1);
                if (x >= xMin && x < xMax) {
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
        }
        numSelected = numHovered;
    }
};