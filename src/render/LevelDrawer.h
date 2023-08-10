#pragma once

#include "render/ImageDrawer.h"
#include "game/GameLevel.h"
#include "TileMappings.h"
#include "ui/ui.h"
#include "ui/leveleditor/LevelEditorUI.h"

using namespace Tiles;

class LevelDrawer {
    private:
    ImageDrawer* drawer;

    void drawLevelRegion(GameLevelRegion* region, int x, int y) {
        for (int i = 0; i < region->getWidth(); i++) {
            for (int j = 0; j < region->getHeight(); j++) {
                drawer->drawTile(region->getGridObject(i, j)->getTilePreview(), x + i * 16, y + j * 16);
            }
        }
    }

    public:
    LevelDrawer(ImageDrawer& drawer): drawer(&drawer) {}

    void drawLevelZone(GameLevelZone& zone, int x, int y) {
        GameLevelRegion** regions = zone.getRegions();
        for (int i = GameObject::NUM_LAYERS - 1; i >= 0; i--) {
            drawer->setZPos(ImageDrawer::ZPOS_GAME_TILES[i]);
            drawLevelRegion(regions[i], x, y);
        }
    }

    void drawLevelZoneBoundary(GameLevelZone& zone, int x, int y) {
        drawer->setZPos(ImageDrawer::ZPOS_BGTILE_UI);

        int w = zone.getWidth() * 16;
        int h = zone.getHeight() * 16;

        drawer->drawTile(LEVELBOUND_BL, x, y);
        drawer->drawTile(LEVELBOUND_BR, x + w - 8, y);
        drawer->drawTile(LEVELBOUND_TL, x, y + h - 8);
        drawer->drawTile(LEVELBOUND_TR, x + w - 8, y + h - 8);

        drawer->drawTileStretched(LEVELBOUND_B, x + 8, y, w - 16, 8);
        drawer->drawTileStretched(LEVELBOUND_T, x + 8, y + h - 8, w - 16, 8);
        drawer->drawTileStretched(LEVELBOUND_L, x, y + 8, 8, h - 16);
        drawer->drawTileStretched(LEVELBOUND_R, x + w - 8, y + 8, 8, h - 16);
    }

    void drawLevelBoundButtons(LevelEditorUI& levelEditor) {
        drawer->setZPos(ImageDrawer::ZPOS_TILE_UI);

        int xOffset = 144;
        int yOffset = 0;

        int scrollX = levelEditor.getScrollX();
        int scrollY = levelEditor.getScrollY();

        GameLevelZone* zone = levelEditor.getCurrentZone();
        int levelW = zone->getWidth() * 16;
        int levelH = zone->getHeight() * 16;

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

    void drawMario(GameLevelZone& zone, int x, int y) {
        Mario* mario = &zone.getMario();
        drawer->setZPos(ImageDrawer::ZPOS_GAME_TILES[mario->getZoneLayer()]);
        drawer->drawTile(mario->getGameObject().getLevelTile(), x + mario->getX(), y + mario->getY());
    }

    void drawMarioGrabCursor(LevelEditorUI& levelEditor, int x, int y) {
        drawer->setZPos(ImageDrawer::ZPOS_TILE_UI);
        Mario* mario = &levelEditor.getCurrentZone()->getMario();

        if (levelEditor.isMarioMouseDown() || levelEditor.getHoverMode() == LevelEditorUI::MARIO) {
            int tile = (levelEditor.isMarioMouseDown()) ? UIHAND_CLOSED : UIHAND_OPEN;
            drawer->drawTile(tile, x + mario->getX(), y + mario->getY());
        }
    }

    void drawCursor(GameLevelZone& zone, int tileX, int tileY, int x, int y) {
        drawer->setZPos(ImageDrawer::ZPOS_TILE_UI);

        int w = zone.getWidth();
        int h = zone.getHeight();

        if (tileX < 0 || tileX >= w || tileY < 0 || tileY >= h) {
            return;
        }

        glEnable(GL_COLOR_LOGIC_OP);
        glLogicOp(GL_EQUIV);
        drawer->drawTile(SELECT_BLOCK, x + tileX * 16, y + tileY * 16);
        glLogicOp(GL_COPY);
        glDisable(GL_COLOR_LOGIC_OP);
    }
};