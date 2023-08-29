#pragma once

#include "screens/IScreen.h"
#include "ui/leveleditor/ILevelEditorScreen.h"
#include "ui/playlevel/IPlayLevelScreen.h"

#include "GlWindow.h"
#include "game/objects/GameObject.h"
#include "game/objects/GameObjectCache.h"
#include "game/GameLevel.h"
#include "ui/ui.h"
#include "ui/leveleditor/LevelEditorUI.h"
#include "ui/leveleditor/FilePickerPopup.h"
#include "ui/leveleditor/InfoPopup.h"
#include "ui/leveleditor/ConfirmPopup.h"
#include "ui/leveleditor/ResizeDialog.h"
#include "ui/leveleditor/TextInputPopup.h"
#include "screens/ScreenManager.h"

class LevelEditorScreen: public ILevelEditorScreen {
    private:
    static const int WINDOW_WIDTH = 400;
    static const int WINDOW_HEIGHT = 256;
    static const int VIEW_WIDTH = 256;
    static const int VIEW_HEIGHT = 240;

    bool closeScreen = false;

    int scrollX = 32;
    int scrollY = 32;

    UIState uiState;

    IPlayLevelScreen* playLevelScreen;
    LevelEditorUI* editorUI;
    FilePickerPopup* filePicker;
    ResizeDialog* resizeDialog;
    InfoPopup infoPopup = InfoPopup(this);
    ConfirmPopup confirmPopup = ConfirmPopup(this);
    TextInputPopup inputPopup = TextInputPopup(this);
    GameLevel* level = nullptr;
    std::string savePath;

    public:
    LevelEditorScreen(GlWindow& window, ScreenManager& manager, IPlayLevelScreen& playLevelScreen) {
        this->window = &window;
        this->manager = &manager;
        this->playLevelScreen = &playLevelScreen;

        editorUI = new LevelEditorUI(this, WINDOW_WIDTH, WINDOW_HEIGHT, scrollX, scrollY, window.getKeys());
        initLevel();
        filePicker = new FilePickerPopup(this);
        resizeDialog = new ResizeDialog(this);

        inputPopup.setCancelCallback([](PopupWindow* popup) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) popup->getPointer();

            editorScreen->setState(ILevelEditorScreen::UIState::EDITOR);
        });
        inputPopup.setPointer(this);
    }

    virtual void initLevel() override {
        if (level != nullptr) {
            delete level;
        }
        level = new GameLevel();
        GameLevelZone* levelZone = level->addZone(8, 8);
        level->setCurrentZone(levelZone);
        savePath = "";
        editorUI->setChangesSaved(true);
    }

    void enable() {
        window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
        window->stageDrawer->setOffset(WINDOW_WIDTH - VIEW_WIDTH, 0);
        manager->getAudioManager()->cancelMusic();
        setState(EDITOR);
        closeScreen = false;
    }

    virtual void tick() override {
        editorUI->tick();
    }

    virtual void requestClose() override {
        editorUI->safeExitWindow();
    }

    virtual bool shouldClose() override {
        return closeScreen;
    }

    virtual void renderFrame() override {
        double mouseX;
        double mouseY;
        window->getCursorPosLetterboxed(mouseX, mouseY);
        // int windowWidth = window->getGameWidth();
        // int windowHeight = window->getGameHeight();

        GameLevelZone* zone = level->getCurrentZone();

        unsigned char* clearColor = zone->getBackground()->getBgColor();
        glClearColor((float) clearColor[0] / 0xFF, (float) clearColor[1] / 0xFF, (float) clearColor[2] / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->drawer->setZPos(ImageDrawer::ZPOS_UI);

        window->uiDrawer->drawMenuBar(editorUI->getMenuBar());

        window->uiDrawer->drawUIRegion(UIREGION_LIGHT, 0, 224, 18, 4);
        window->uiDrawer->drawTextInput(editorUI->getSearchBar());
        window->uiDrawer->drawObjectPicker(editorUI->getObjectPicker(), 0, 0);

        // window->stageDrawer->drawScoreboard(7654321, 54, 5, 4, -1, manager->getFPS());
		// window->stageDrawer->drawTitle(0, 7654321);

        window->levelDrawer->drawLevelZone(*zone, 144, 0, scrollX, scrollY, true);
        window->levelDrawer->drawLevelZoneBoundary(*zone, WINDOW_WIDTH - VIEW_WIDTH + scrollX, scrollY);
        window->levelDrawer->drawMario(*zone, 144 + scrollX, scrollY);

        window->levelDrawer->drawMarioGrabCursor(*editorUI, 144 + scrollX, scrollY);
        window->levelDrawer->drawCursor(*editorUI, 144 + scrollX, scrollY);
        
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
                break;
            case RESIZE_DIALOG:
                window->drawer->setZPos(ImageDrawer::ZPOS_UI_DIALOG);
                window->uiDrawer->drawPopupWindow(*resizeDialog, WINDOW_WIDTH, WINDOW_HEIGHT);
                break;
            case CONFIRM_DIALOG:
                window->drawer->setZPos(ImageDrawer::ZPOS_UI_DIALOG);
                window->uiDrawer->drawPopupWindow(confirmPopup, WINDOW_WIDTH, WINDOW_HEIGHT);
                break;
            case TEXT_INPUT_DIALOG:
                window->drawer->setZPos(ImageDrawer::ZPOS_UI_DIALOG);
                window->uiDrawer->drawPopupWindow(inputPopup, WINDOW_WIDTH, WINDOW_HEIGHT);
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

    virtual void setSavePath(std::string path) override {
        savePath = path;
    }

    virtual std::string& getSavePath() override {
        return savePath;
    }

    virtual void setChangesSaved(bool value) {
        editorUI->setChangesSaved(value);
    }

    virtual bool getChangesSaved() {
        return editorUI->getChangesSaved();
    }

    virtual void setAutosave(bool value) {
        editorUI->setAutosave(value);
    }

    virtual void attemptAutosave() {
        editorUI->attemptAutosave();
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
            case RESIZE_DIALOG:
                window->uiEventElement = resizeDialog;
                resizeDialog->enable();
                break;
            case CONFIRM_DIALOG:
                window->uiEventElement = &confirmPopup;
                break;
            case TEXT_INPUT_DIALOG:
                window->uiEventElement = &inputPopup;
                inputPopup.enable();
                break;
        }
    }

    virtual char* getFileInputBuffer() override {
        return filePicker->getFileInputBuffer();
    }

    virtual ConfirmPopup* getConfirmPopup() override {
        return &confirmPopup;
    }

    virtual TextInputPopup* getTextInputPopup() override {
        return &inputPopup;
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
        closeScreen = true;
    }

    ~LevelEditorScreen() {
        delete editorUI;
        delete filePicker;
        delete resizeDialog;
        delete level;
    }
};