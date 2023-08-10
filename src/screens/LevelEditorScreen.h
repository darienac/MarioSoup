#pragma once

#include "screens/IScreen.h"
#include "ui/leveleditor/ILevelEditorScreen.h"
#include "ui/playlevel/IPlayLevelScreen.h"

#include "GlWindow.h"
#include "game/GameObject.h"
#include "game/GameObjectCache.h"
#include "game/GameLevel.h"
#include "ui/ui.h"
#include "ui/leveleditor/LevelEditorUI.h"
#include "ui/leveleditor/FilePickerPopup.h"
#include "ui/leveleditor/InfoPopup.h"
#include "screens/ScreenManager.h"

class LevelEditorScreen: public ILevelEditorScreen {
    private:
    static const int WINDOW_WIDTH = 400;
    static const int WINDOW_HEIGHT = 256;
    static const int VIEW_WIDTH = 256;
    static const int VIEW_HEIGHT = 240;

    int scrollX = 32;
    int scrollY = 32;

    UIState uiState;

    IPlayLevelScreen* playLevelScreen;
    LevelEditorUI* editorUI;
    FilePickerPopup* filePicker;
    InfoPopup infoPopup = InfoPopup(this);
    GameLevel* level = new GameLevel();

    public:
    LevelEditorScreen(GlWindow& window, ScreenManager& manager, IPlayLevelScreen& playLevelScreen) {
        this->window = &window;
        this->manager = &manager;
        this->playLevelScreen = &playLevelScreen;

        GameLevelZone* levelZone = level->addZone(8, 8);
        level->setCurrentZone(levelZone);
        editorUI = new LevelEditorUI(this, WINDOW_WIDTH, WINDOW_HEIGHT, scrollX, scrollY, window.getKeys());
        filePicker = new FilePickerPopup(this);
    }

    void enable() {
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        window->stageDrawer->setOffset(WINDOW_WIDTH - VIEW_WIDTH, 0);
        setState(EDITOR);
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

        // window->stageDrawer->drawScoreboard(7654321, 54, 5, 4, -1, manager->getFPS());
		// window->stageDrawer->drawTitle(0, 7654321);

        GameLevelZone* zone = level->getCurrentZone();

        window->levelDrawer->drawLevelZoneBoundary(*zone, WINDOW_WIDTH - VIEW_WIDTH + scrollX, scrollY);
        window->levelDrawer->drawLevelZone(*zone, 144 + scrollX, scrollY);
        window->levelDrawer->drawMario(*zone, 144 + scrollX, scrollY);

        window->levelDrawer->drawMarioGrabCursor(*editorUI, 144 + scrollX, scrollY);
        window->levelDrawer->drawCursor(*zone, editorUI->getTileHoverX(), editorUI->getTileHoverY(), 144 + scrollX, scrollY);
        
        window->levelDrawer->drawLevelBoundButtons(*editorUI);

        switch (uiState) {
            case OPEN_DIALOG:
            case SAVE_DIALOG:
                window->drawer->setZPos(ImageDrawer::ZPOS_UI_DIALOG);
                window->uiDrawer->drawPopupWindow(*filePicker, WINDOW_WIDTH, WINDOW_HEIGHT);
                break;
            case INFO_POPUP:
                window->drawer->setZPos(ImageDrawer::ZPOS_UI_DIALOG);
                window->uiDrawer->drawPopupWindow(infoPopup, WINDOW_WIDTH, WINDOW_HEIGHT);
            default:
                break;
        };
    }

    virtual GameLevel* getLevel() override {
        return level;
    }

    virtual void setLevel(GameLevel* level) override {
        delete this->level;
        this->level = level;
    }

    virtual UIState getState() override {
        return uiState;
    }
    virtual void setState(UIState state) override {
        this->uiState = state;

        switch (uiState) {
            case EDITOR:
                window->uiEventElement = editorUI;
                break;
            case OPEN_DIALOG:
            case SAVE_DIALOG:
                window->uiEventElement = filePicker;
                filePicker->enable();
                break;
            case INFO_POPUP:
                window->uiEventElement = &infoPopup;
                break;
        }
    }

    virtual char* getFileInputBuffer() override {
        return filePicker->getFileInputBuffer();
    }

    virtual void setInfoMessage(const char* message) {
        infoPopup.setMessage(message);
    }

    virtual bool isFullscreen() override {
        return window->isFullscreen();
    }

    virtual void setFullscreen(bool value) override {
        if (value) {
            window->enableFullscreen();
        } else {
            window->disableFullscreen();
        }
    }

    virtual void runLevel() override {
        playLevelScreen->setLevel(level);
        manager->setScreen(playLevelScreen);
    }

    virtual void closeWindow() override {
        window->exitWindow();
    }

    ~LevelEditorScreen() {
        delete editorUI;
        delete filePicker;
        delete level;
    }
};