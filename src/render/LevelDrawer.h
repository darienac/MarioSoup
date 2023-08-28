#pragma once

#include "render/ImageDrawer.h"
#include "game/GameLevel.h"
#include "game/entities/IEntity.h"
#include "game/backgrounds/GameBackground.h"
#include "TileMappings.h"
#include "ui/ui.h"
#include "ui/leveleditor/LevelEditorUI.h"
#include "ui/playlevel/IPlayLevelScreen.h"

using namespace Tiles;

class LevelDrawer {
    private:
    ImageDrawer* drawer;

    void drawLevelRegion(IGameLevelRegion* region, int xOff, int yOff, int scrollX, int scrollY, bool isEditor) {
        int x = scrollX + xOff;
        int y = scrollY + yOff;
        if (!isEditor) {
            for (IEntity* entity : region->getEntities()) {
                GameObject* object = &entity->getGameObject();
                Tile* tile = &Tiles::getTile(object->getLevelTile());
                int w = tile->cw;
                int h = tile->ch;
                if (object->isFlippedX()) {
                    w = -w;
                }
                if (object->isFlippedY()) {
                    h = -h;
                }
                drawer->drawTileStretched(*tile, entity->getX() + x, entity->getY() + y, w, h);
            }
        }
        
        int tileLeft = IEntity::div(-scrollX, 16);
        int tileBottom = IEntity::div(-scrollY, 16);
        int tilesWide = IEntity::div(IPlayLevelScreen::WINDOW_WIDTH, 16) + 1;
        int tilesTall = IEntity::div(IPlayLevelScreen::WINDOW_HEIGHT, 16) + 1;
        // TODO: Finish this area
        for (int i = std::max(tileLeft, 0); i < std::min(tileLeft + tilesWide, region->getWidth()); i++) {
            for (int j = std::max(tileBottom, 0); j < std::min(tileBottom + tilesTall, region->getHeight()); j++) {
                GameObject* left = region->getGridObject(i - 1, j);
                GameObject* right = region->getGridObject(i + 1, j);
                GameObject* up = region->getGridObject(i, j + 1);
                GameObject* down = region->getGridObject(i, j - 1);
                GameObject* obj = region->getGridObject(i, j);

                Tile* tile = &Tiles::getTile(obj->getLevelTile(left, right, up, down));
                int w = tile->cw;
                int h = tile->ch;
                if (obj->isFlippedX()) {
                    w = -w;
                }
                if (obj->isFlippedY()) {
                    h = -h;
                }

                drawer->drawTileStretched(*tile, x + i * 16, y + j * 16, w, h);
                if (isEditor && obj && obj->isFlag(GameObject::CONTAINS_ITEM) && region->getGridData(i, j).containerObject) {
                    drawer->drawTile(UICRATE_CLOSED, x + i * 16, y + j * 16);
                }
            }
        }
    }

    int mod(int v0, int v1) {
        if (v0 < 0) {
            v0 += -(v0 / v1) * v1 + v1;
        }
        return v0 % v1;
    }

    int div(int v0, int v1) {
        return (v0 - mod(v0, v1)) / v1;
    }

    public:
    LevelDrawer(ImageDrawer& drawer): drawer(&drawer) {}

    void drawZoneBackground(GameBackground& background, int xOff, int yOff, int scrollX, int scrollY) {
        for (GameBackgroundLayer* layer : background.getLayers()) {
            drawZoneBackgroundLayer(*layer, xOff, yOff, scrollX, scrollY);
        }
    }

    void drawZoneBackgroundLayer(GameBackgroundLayer& layer, int xOff, int yOff, int scrollX, int scrollY) {
        Tile* tile = layer.getTile();
        int w = tile->getWidth();

        int x0 = mod(scrollX * layer.getScrollXMult(), w) + layer.getXOff();
        int y0;
        if (scrollY >= 0) {
            y0 = layer.getYOff();
        } else {
            y0 = scrollY * layer.getScrollYMult() + layer.getYOff();
        }

        drawer->drawTile(*tile, x0 + xOff, y0 + yOff);
        if (x0 > 0) {
            drawer->drawTile(*tile, x0 - w + xOff, y0 + yOff);
        }
    }

    void drawLevelZone(GameLevelZone& zone, int xOff, int yOff, int scrollX, int scrollY, bool isEditor) {
        drawer->setZPos(ImageDrawer::ZPOS_BACKGROUND);
        drawZoneBackground(*zone.getBackground(), xOff, yOff, scrollX, scrollY);

        IGameLevelRegion** regions = zone.getRegions();
        for (int i = GameObject::NUM_LAYERS - 1; i >= 0; i--) {
            drawer->setZPos(ImageDrawer::ZPOS_GAME_TILES[i]);
            drawLevelRegion(regions[i], xOff, yOff, scrollX, scrollY, isEditor);
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

        GameObject* object = &mario->getGameObject();
        Tile* tile = &Tiles::getTile(object->getLevelTile());
        int w = tile->cw;
        int h = tile->ch;
        if (object->isFlippedX()) {
            w = -w;
        }
        if (object->isFlippedY()) {
            h = -h;
        }
        drawer->drawTileStretched(*tile, x + mario->getX(), y + mario->getY(), w, h);
    }

    void drawMarioGrabCursor(LevelEditorUI& levelEditor, int x, int y) {
        drawer->setZPos(ImageDrawer::ZPOS_TILE_UI);
        Mario* mario = &levelEditor.getCurrentZone()->getMario();

        if (levelEditor.isMarioMouseDown() || levelEditor.getHoverMode() == LevelEditorUI::MARIO) {
            int tile = (levelEditor.isMarioMouseDown()) ? UIHAND_CLOSED : UIHAND_OPEN;
            drawer->drawTile(tile, x + mario->getX(), y + mario->getY());
        }
    }

    void drawCursor(LevelEditorUI& levelEditor, int x, int y) {
        GameLevelZone* zone = levelEditor.getCurrentZone();
        int tileX = levelEditor.getTileHoverX();
        int tileY = levelEditor.getTileHoverY();
        drawer->setZPos(ImageDrawer::ZPOS_TILE_UI);

        int w = zone->getWidth();
        int h = zone->getHeight();

        if (tileX < 0 || tileX >= w || tileY < 0 || tileY >= h) {
            return;
        }

        if (levelEditor.isHoverInsertItem()) {
            drawer->drawTile(UICRATE_OPEN, x + tileX * 16, y + tileY * 16);
        } else {
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(GL_EQUIV);
            drawer->drawTile(SELECT_BLOCK, x + tileX * 16, y + tileY * 16);
            glLogicOp(GL_COPY);
            glDisable(GL_COLOR_LOGIC_OP);
        }
    }
};