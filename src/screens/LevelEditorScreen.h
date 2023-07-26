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
    int scrollX = 0;
    int scrollY = 0;

    GlWindow* window;
    LevelEditorUI* editorUI;
    GameLevel level = GameLevel(8, 8);

    public:
    LevelEditorScreen(GlWindow& window, ScreenManager& manager) {
        this->window = &window;
        this->manager = &manager;

        editorUI = new LevelEditorUI(window.getWidth(), window.getHeight());

        level.setGridObject(objects["smb:brick"], 0, 0);
    }

    void enable() {
        window->resize(400, 256);
        window->stageDrawer->setOffset(144, 0);
        window->uiEventElement = editorUI;
    }

    void renderFrame() override {
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

        window->levelDrawer->drawLevelBoundary(level, 144 + scrollX, scrollY);
        window->levelDrawer->drawLevel(level, 144 + scrollX, scrollY);
    }

    ~LevelEditorScreen() {
        delete editorUI;
    }
};