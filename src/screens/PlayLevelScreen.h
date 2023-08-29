#pragma once

#include "ui/playlevel/IPlayLevelScreen.h"
#include "ui/playlevel/PlayLevelUI.h"
#include "screens/ScreenManager.h"
#include "game/LevelLoader.h"
#include "audio/AudioManager.h"
#include "audio/AudioCache.h"

class PlayLevelScreen: public IPlayLevelScreen {
    private:
    bool closeScreen = false;

    int scrollX = 0;
    int scrollY = 0;

    GameLevel* level = nullptr;
    PlayLevelUI* levelUI;
    ILevelEditorScreen* editorScreen = nullptr;

    public:
    PlayLevelScreen(GlWindow& window, ScreenManager& manager) {
        this->window = &window;
        this->manager = &manager;

        levelUI = new PlayLevelUI(this, manager.getAudioManager(), window.getKeys());
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

        levelUI->loadLevelZone();
    }

    void setEditorScreen(ILevelEditorScreen* screen) {
        editorScreen = screen;
    }

    virtual void tick() override {
        levelUI->tick();
    }

    virtual bool shouldClose() override {
        return closeScreen;
    }

    virtual void requestClose() override {
        closeScreen = true;
    }

    virtual void renderFrame() override {
        GameLevelZone* zone = level->getCurrentZone();

        unsigned char* clearColor = zone->getBackground()->getBgColor();
        glClearColor((float) clearColor[0] / 0xFF, (float) clearColor[1] / 0xFF, (float) clearColor[2] / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->levelDrawer->drawLevelZone(*zone, 0, 0, scrollX, scrollY, false);
        window->levelDrawer->drawMario(*zone, scrollX, scrollY);
    }

    virtual void enable() override {
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        window->uiEventElement = levelUI;
        closeScreen = false;
        if (level != nullptr && level->getCurrentZone()->getMusicPath() != "") {
            manager->getAudioManager()->playMusic(level->getCurrentZone()->getMusicPath());
        }
        
    }

    virtual void exitWindow() override {
        closeScreen = true;
    }

    virtual void setLevel(GameLevel* level) override {
        if (this->level != nullptr) {
            delete this->level;
        }
        this->level = new GameLevel(*level);

        levelUI->loadLevelZone();
    }

    virtual GameLevel* getLevel() override {
        return level;
    }

    virtual void exitToEditor() override {
        if (editorScreen == nullptr) {
            exitWindow();
            return;
        }
        
        manager->setScreen(editorScreen);
    }

    virtual int getScrollX() {
        return scrollX;
    }

    virtual void setScrollX(int value) {
        scrollX = value;
    }

    virtual int getScrollY() {
        return scrollY;
    }

    virtual void setScrollY(int value) {
        scrollY = value;
    }

    ~PlayLevelScreen() {
        delete levelUI;
        if (editorScreen == nullptr && level != nullptr) {
            delete level;
        }
    }
};