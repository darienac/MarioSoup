#pragma once

#include "render/ImageDrawer.h"
#include "game/GameLevel.h"
#include "TileMappings.h"

using namespace Tiles;

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
        int w = level.getWidth() * 16;
        int h = level.getHeight() * 16;

        drawer->drawTile(LEVELBOUND_BL, x, y);
        drawer->drawTile(LEVELBOUND_BR, x + w - 8, y);
        drawer->drawTile(LEVELBOUND_TL, x, y + h - 8);
        drawer->drawTile(LEVELBOUND_TR, x + w - 8, y + h - 8);

        drawer->drawTileStretched(LEVELBOUND_B, x + 8, y, w - 16, 8);
        drawer->drawTileStretched(LEVELBOUND_T, x + 8, y + h - 8, w - 16, 8);
        drawer->drawTileStretched(LEVELBOUND_L, x, y + 8, 8, h - 16);
        drawer->drawTileStretched(LEVELBOUND_R, x + w - 8, y + 8, 8, h - 16);
    }
};