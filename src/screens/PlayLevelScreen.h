#pragma once

#include "ui/playlevel/IPlayLevelScreen.h"
#include "ui/playlevel/PlayLevelUI.h"
#include "screens/ScreenManager.h"
#include "game/LevelLoader.h"

class PlayLevelScreen: public IPlayLevelScreen {
    private:
    static const int WINDOW_WIDTH = 256;
    static const int WINDOW_HEIGHT = 240;

    int scrollX = 0;
    int scrollY = 0;

    GameLevel* level = nullptr;
    PlayLevelUI* levelUI;
    ILevelEditorScreen* editorScreen = nullptr;

    public:
    PlayLevelScreen(GlWindow& window, ScreenManager& manager) {
        this->window = &window;
        this->manager = &manager;

        levelUI = new PlayLevelUI(this, window.getKeys());
    }

    void loadLevel(const char* filePath) {
        if (editorScreen != nullptr) {
            throw "Editor screen already in use";
        }
        if (level != nullptr) {
            delete level;
        }

        LevelLoader loader = LevelLoader();
        level = new GameLevel();
        loader.loadLevel(*level, filePath);
    }

    void setEditorScreen(ILevelEditorScreen* screen) {
        editorScreen = screen;
    }

    virtual void tick() override {
        levelUI->tick();
    }

    virtual void renderFrame() override {
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
        if (editorScreen == nullptr) {
            throw "No Editor Screen attached";
        }
        
        manager->setScreen(editorScreen);
    }

    ~PlayLevelScreen() {
        delete levelUI;
        if (editorScreen == nullptr && level != nullptr) {
            delete level;
        }
    }
};