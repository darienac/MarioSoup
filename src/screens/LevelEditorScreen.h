#pragma once

#include "screens/IScreen.h"
#include "screens/ScreenManager.h"
#include "GlWindow.h"
#include "game/GameObject.h"
#include "game/GameObjectCache.h"
#include "game/GameLevel.h"
#include "ui/ui.h"

using namespace GameObjectCache;

class LevelEditorScreen: public IScreen {
    private:
    static const int WINDOW_WIDTH = 400;
    static const int WINDOW_HEIGHT = 256;
    static const int VIEW_WIDTH = 256;
    static const int VIEW_HEIGHT = 240;

    int scrollX = 32;
    int scrollY = 32;

    GlWindow* window;
    LevelEditorUI* editorUI;
    GameLevel level = GameLevel();
    GameLevelRegion* levelRegion;

    public:
    LevelEditorScreen(GlWindow& window, ScreenManager& manager) {
        this->window = &window;
        this->manager = &manager;

        levelRegion = level.addRegion(8, 8);
        editorUI = new LevelEditorUI(*levelRegion, WINDOW_WIDTH, WINDOW_HEIGHT, scrollX, scrollY, window.getKeys());
    }

    void enable() {
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        window->stageDrawer->setOffset(WINDOW_WIDTH - VIEW_WIDTH, 0);
        window->uiEventElement = editorUI;
    }

    void renderFrame() override {
        editorUI->tick();

        double mouseX;
        double mouseY;
        window->getCursorPosLetterboxed(mouseX, mouseY);
        // int windowWidth = window->getGameWidth();
        // int windowHeight = window->getGameHeight();

        glClearColor((float) 0x94 / 0xFF, (float) 0x94 / 0xFF, (float) 0xFF / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->drawer->setZPos(ImageDrawer::ZPOS_UI);

        window->uiDrawer->drawMenuBar(editorUI->getMenuBar());

        window->uiDrawer->drawUIRegion(UIREGION_LIGHT, 0, 224, 18, 4);
        window->uiDrawer->drawTextInput(editorUI->getSearchBar());
        window->uiDrawer->drawObjectPicker(editorUI->getObjectPicker(), 0, 0);

        window->drawer->setZPos(ImageDrawer::ZPOS_GAME_TILES);

        // window->stageDrawer->drawScoreboard(7654321, 54, 5, 4, -1, manager->getFPS());
		// window->stageDrawer->drawTitle(0, 7654321);

        window->levelDrawer->drawLevelBoundary(*levelRegion, WINDOW_WIDTH - VIEW_WIDTH + scrollX, scrollY);
        window->levelDrawer->drawLevelBoundButtons(*editorUI);
        window->levelDrawer->drawLevel(*levelRegion, 144 + scrollX, scrollY);
        window->levelDrawer->drawCursor(*levelRegion, editorUI->getTileHoverX(), editorUI->getTileHoverY(), 144 + scrollX, scrollY);
    }

    ~LevelEditorScreen() {
        delete editorUI;
    }
};