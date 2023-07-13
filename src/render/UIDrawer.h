#pragma once

#include "render/ImageDrawer.h"
#include "TileMappings.h"
#include "ui/ui.h"

using namespace Tiles;
using namespace Palletes;

class UIDrawer {
    private:
    ImageDrawer* drawer;
    int vOffset;

    void drawMenuItem(const char* label, int x, bool isHover) {
        int l = strlen(label);
        int tile;

        if (isHover) {
            tile = MENU_ON;
        } else {
            tile = MENU_OFF;
        }
        for (int i = x; i < l * 8 + x + 8; i += 8) {
            drawer->drawTile(tile, i, vOffset);
        }
        drawText(label, x + 4, 4 + vOffset);
    }
    
    public:
    UIDrawer(ImageDrawer& drawer, int windowHeight): drawer(&drawer) {
        updateWindowHeight(windowHeight);
    }

    void updateWindowHeight(int height) {
        vOffset = height - 16;
    }

    void drawMenuBar(MenuBar& menuBar) {
        const char** labels = menuBar.getLabels();
        int numLabels = menuBar.getNumLabels();
        drawer->setPalleteSwap(OVERWORLD_2, true);
        int x = 0;
        for (int i = 0; i < numLabels; i++) {
            bool isHover = (i == menuBar.getNumHovered());
            bool isSelected = (i == menuBar.getNumSelected());
            drawMenuItem(labels[i], x, isHover || isSelected);
            x += strlen(labels[i]) * 8 + 8;
        }
    }

    void drawText(std::string text, int x, int y) {
        drawer->setPalleteSwap(OVERWORLD_2, true);
        for (int i = 0; i < text.length(); i++) {
            int tile = AIR + textEncoding.find(text.at(i));
            drawer->drawTile(tile, x + i * 8, y);
        }
    }
};