#pragma once

#include "render/ImageDrawer.h"
#include "game/GameLevel.h"

class LevelDrawer {
    private:
    ImageDrawer* drawer;

    public:
    LevelDrawer(ImageDrawer& drawer): drawer(&drawer) {}

    void drawLevel(GameLevel& level, int x, int y) {
        for (int i = 0; i < level.getWidth(); i++) {
            for (int j = 0; j < level.getHeight(); j++) {
                drawer->drawTile(level.getGridObject(i, j)->getTilePreview(), x + i * 16, y + j * 16);
            }
        }
    }

    void drawLevelBoundary(GameLevel& level, int x, int y) {
        
    }
};