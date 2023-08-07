#pragma once

#include "screens/IScreen.h"
#include "screens/ScreenManager.h"

class PlayLevelScreen: public IScreen {
    private:
    static const int WINDOW_WIDTH = 256;
    static const int WINDOW_HEIGHT = 240;

    int scrollX = 0;
    int scrollY = 0;

    GameLevel* level;

    public:
    PlayLevelScreen(GlWindow& window, ScreenManager& manager) {
        this->window = &window;
        this->manager = &manager;
    }

    void setLevel(GameLevel* level) {
        this->level = level;
    }

    void renderFrame() override {
        glClearColor((float) 0x94 / 0xFF, (float) 0x94 / 0xFF, (float) 0xFF / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->drawer->setZPos(ImageDrawer::ZPOS_GAME_TILES);

        window->levelDrawer->drawLevelRegion(*level->getCurrentRegion(), scrollX, scrollY);
    }

    virtual void enable() override {
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
};