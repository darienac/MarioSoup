#pragma once

#include <iostream>

#include "ui/IUIElement.h"

class MenuBar : public IUIElement {
    private:
    const char** labels;
    int numLabels;
    int numHovered = -1;
    int numSelected = -1;

    public:
    MenuBar(const char** labels, int numLabels) {
        this->labels = labels;
        this->numLabels = numLabels;
    }

    const char** getLabels() {
        return labels;
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
        numSelected = numHovered;
    }
};