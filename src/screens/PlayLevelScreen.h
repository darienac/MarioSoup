#pragma once

#include "ui/playlevel/IPlayLevelScreen.h"
#include "ui/playlevel/PlayLevelUI.h"
#include "screens/ScreenManager.h"

class PlayLevelScreen: public IPlayLevelScreen {
    private:
    static const int WINDOW_WIDTH = 256;
    static const int WINDOW_HEIGHT = 240;

    int scrollX = 0;
    int scrollY = 0;

    GameLevel* level;
    PlayLevelUI* levelUI;
    ILevelEditorScreen* editorScreen = nullptr;

    public:
    PlayLevelScreen(GlWindow& window, ScreenManager& manager) {
        this->window = &window;
        this->manager = &manager;

        levelUI = new PlayLevelUI(this, window.getKeys());
    }

    void setEditorScreen(ILevelEditorScreen* screen) {
        editorScreen = screen;
    }

    void renderFrame() override {
        levelUI->tick();
        
        glClearColor((float) 0x94 / 0xFF, (float) 0x94 / 0xFF, (float) 0xFF / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GameLevelZone* zone = level->getCurrentZone();
        window->levelDrawer->drawLevelZone(*zone, scrollX, scrollY);
        window->levelDrawer->drawMario(*zone, scrollX, scrollY);
    }

    virtual void enable() override {
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        window->uiEventElement = levelUI;
    }

    virtual void exitWindow() override {
        window->exitWindow();
    }

    virtual void setLevel(GameLevel* level) override {
        this->level = level;
    }

    virtual GameLevel* getLevel() override {
        return level;
    }

    virtual void exitToEditor() override {
        if (editorScreen != nullptr) {
            manager->setScreen(editorScreen);
        }
    }

    ~PlayLevelScreen() {
        delete levelUI;
    }
};