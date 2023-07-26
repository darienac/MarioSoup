#pragma once

#include "render/ImageDrawer.h"
#include "game/GameLevel.h"
#include "TileMappings.h"
#include "ui/ui.h"

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

    void drawCursor(GameLevel& level, int tileX, int tileY, int x, int y) {
        int w = level.getWidth();
        int h = level.getHeight();

        if (tileX < 0 || tileX >= w || tileY < 0 || tileY >= h) {
            return;
        }

        glEnable(GL_COLOR_LOGIC_OP);
        glLogicOp(GL_EQUIV);
        drawer->drawTile(SELECT_BLOCK, x + tileX * 16, y + tileY * 16);
        glLogicOp(GL_COPY);
        glDisable(GL_COLOR_LOGIC_OP);
    }

    void drawLevelBoundButtons(LevelEditorUI& levelEditor) {
        int xOffset = 144;
        int yOffset = 0;

        int scrollX = levelEditor.getScrollX();
        int scrollY = levelEditor.getScrollY();

        GameLevel* level = levelEditor.getLevel();
        int levelW = level->getWidth() * 16;
        int levelH = level->getHeight() * 16;

        int viewX;
        int viewY;

        // Expand Up

        viewX = levelEditor.getLevelBoundButtonX();
        viewY = scrollY + levelH;

        drawer->drawTile(LEVEL_MINUS, xOffset + viewX - 16, yOffset + viewY);
        drawer->drawTile(LEVEL_PLUS, xOffset + viewX, yOffset + viewY);

        // Expand Down

        viewY = scrollY - 16;

        drawer->drawTile(LEVEL_MINUS, xOffset + viewX - 16, yOffset + viewY);
        drawer->drawTile(LEVEL_PLUS, xOffset + viewX, yOffset + viewY);

        // Expand Right

        viewX = scrollX + levelW;
        viewY = levelEditor.getLevelBoundButtonY();

        drawer->drawTile(LEVEL_MINUS, xOffset + viewX, yOffset + viewY - 16);
        drawer->drawTile(LEVEL_PLUS, xOffset + viewX, yOffset + viewY);

        // Expand Left

        viewX = scrollX - 16;

        drawer->drawTile(LEVEL_MINUS, xOffset + viewX, yOffset + viewY - 16);
        drawer->drawTile(LEVEL_PLUS, xOffset + viewX, yOffset + viewY);
    }
};