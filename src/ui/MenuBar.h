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

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        bool isHovered = false;
        if (numSelected != -1) {
            int xMax = 0;
            int xMin;
            for (int i = 0; i <= numSelected; i++) {
                xMin = xMax;
                xMax = xMin + 8 * (strlen(labels[i]) + 1);
            }
            isHovered |= lists[numSelected].hover(x - xMin, y - gameHeight + 16, gameWidth, gameHeight);
        }
        if (y >= gameHeight - 16 && y < gameHeight) {
            isHovered = true;
            int xMin = 0;
            for (int i = 0; i < numLabels; i++) {
                int xMax = xMin + 8 * (strlen(labels[i]) + 1);
                if (x >= xMin && x < xMax) {
                    numHovered = i;
                    return isHovered;
                }
                xMin = xMax;
            }
        }
        numHovered = -1;

        return isHovered;
    }

    void click() {
        if (numSelected != -1) {
            lists[numSelected].click();
            if (!lists[numSelected].isAllowCloseList()) {
                return;
            }
        }
        if (numSelected == numHovered) {
            numSelected = -1;
        } else {
            numSelected = numHovered;
        }
    }
};