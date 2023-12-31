#pragma once

#include "game/objects/GameObject.h"
#include "game/objects/GameObjectCache.h"
#include "game/LevelSaver.h"
#include "game/LevelLoader.h"
#include "ui/ui.h"
#include "ui/leveleditor/ILevelEditorScreen.h"
#include "ui/leveleditor/ObjectPickerLoader.h"
#include "ui/leveleditor/EditorMenuBar.h"

#include <GLFW/glfw3.h>

using namespace GameObjectCache;

class LevelEditorUI: public UIBundle {
    public:
    enum HoverMode {
        NO_HOVER,
        TILE,
        MARIO
    };

    private:
    static const int NUM_BUNDLE_ELEMENTS = 3;

    static const int VIEW_WIDTH = 256;
    static const int VIEW_HEIGHT = 240;
    
    // Editor Bundle Elements
    
    EditorMenuBar menuBar = EditorMenuBar();

    TextInput* searchBar;
    char searchBarBuffer[32];

    ObjectPicker* picker;

    IUIElement* bundleElements[NUM_BUNDLE_ELEMENTS];

    HoverMode hoverMode = NO_HOVER;

    ILevelEditorScreen* screen;
    int width;
    int height;
    int* scrollX;
    int* scrollY;
    bool* keys;

    int hoverX = -1;
    int hoverY = -1;
    int tileHoverX = -1;
    int tileHoverY = -1;
    int lastTileHoverX = -1;
    int lastTileHoverY = -1;
    int lastLayer = -1;
    int eraseLayer = -1;
    GameObject* lastPlacedObject = nullptr;
    bool isMouseDown = false;
    bool isMouseRightDown = false;
    bool hoverInsertItem = false;

    bool marioMouseDown = false;
    int marioMouseOffsetX = 0;
    int marioMouseOffsetY = 0;

    bool changesSaved = true;
    bool autosave = true;

    void placeBlock(GameObject* object, int layer) {
        if (layer == -1) {
            return;
        }

        GameLevel* level = screen->getLevel();
        if (object == nullptr) {
            return;
        }
        if (object != lastPlacedObject || layer != lastLayer || tileHoverX != lastTileHoverX || tileHoverY != lastTileHoverY) {
            lastTileHoverX = tileHoverX;
            lastTileHoverY = tileHoverY;
            lastLayer = layer;
            lastPlacedObject = object;

            IGameLevelRegion* region = level->getCurrentZone()->getRegions()[layer];
            if (hoverInsertItem) {
                region->addGridData(tileHoverX, tileHoverY, {picker->getSelectedItem()});
            } else {
                region->setGridObject(object, tileHoverX, tileHoverY);
            }
            changesSaved = false;
        }
    }

    void placeBlock(GameObject* object) {
        if (object == nullptr) {
            return;
        }
        placeBlock(object, object->getPreferredLayer());
    }

    void eraseBlock() {
        if (eraseLayer == -1) {
            setEraseLayer();
        }
        if (eraseLayer == -1) {
            return;
        }
        screen->getLevel()->getCurrentZone()->getRegions()[eraseLayer]->removeGridData(tileHoverX, tileHoverY);
        placeBlock(GameObjectCache::objects["air"], eraseLayer);
    }

    void setEraseLayer() {
        screen->getLevel()->getCurrentZone()->getUpperGridObject(tileHoverX, tileHoverY, eraseLayer);
    }

    void setTileHover() {
        if (marioMouseDown || hoverX < *scrollX || hoverY < *scrollY) {
            tileHoverX = -1;
            tileHoverY = -1;
            return;
        }
        tileHoverX = (hoverX - *scrollX) / 16;
        tileHoverY = (hoverY - *scrollY) / 16;

        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        int upperLayer;
        hoverInsertItem = picker->getSelectedItem() && picker->getSelectedItem()->isFlag(GameObject::ITEM) && zone->getUpperGridObject(tileHoverX, tileHoverY, upperLayer)->isFlag(GameObject::CONTAINS_ITEM);
        if (isMouseDown) {
            placeBlock(picker->getSelectedItem());
        } else if (isMouseRightDown) {
            eraseBlock();
        }
    }

    void boundScroll() {
        GameLevel* level = screen->getLevel();
        int w = level->getCurrentZone()->getWidth();
        int h = level->getCurrentZone()->getHeight();

        if (*scrollX > 32) {
            *scrollX = 32;
        } else if (*scrollX < -w * 16 + 32) {
            *scrollX = -w * 16 + 32;
        }
        if (*scrollY > 32) {
            *scrollY = 32;
        } else if (*scrollY < -h * 16 + 32) {
            *scrollY = -h * 16 + 32;
        }
    }

    void dragMario() {
        if (!marioMouseDown || hoverMode == NO_HOVER) {
            return;
        }

        Mario* mario = &getCurrentZone()->getMario();

        int x = hoverX - *scrollX - marioMouseOffsetX;
        int y = hoverY - *scrollY - marioMouseOffsetY;
        if (!keys[GLFW_KEY_LEFT_SHIFT]) {
            x -= (x + 40) % 16 - 8;
            y -= (y + 40) % 16 - 8;
        }
        mario->setX(x);
        mario->setY(y);

        changesSaved = false;
        getCurrentZone()->boundMario();
    }

    void clickLevelBoundButtons() {
        GameLevel* level = screen->getLevel();
        int xOffset = 144;
        int yOffset = 0;

        auto inBoundButton = [this, &xOffset, &yOffset](int x, int y) {
            int mx = hoverX;
            int my = hoverY;
            return mx >= x && mx < x + 16 && my >= y && my < y + 16;
        };

        GameLevelZone* zone = level->getCurrentZone();

        int zoneW = zone->getWidth();
        int zoneH = zone->getHeight();

        int viewX;
        int viewY;

        viewX = getLevelBoundButtonX();
        viewY = *scrollY + zoneH * 16;

        if (inBoundButton(viewX - 16, viewY)) {
            // Shrink up
            zone->resizeGrid(zoneW, zoneH - 1, 0, 0);
            changesSaved = false;
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand up
            zone->resizeGrid(zoneW, zoneH + 1, 0, 0);
            changesSaved = false;
            return;
        }

        viewY = *scrollY - 16;

        if (inBoundButton(viewX - 16, viewY)) {
            // Shrink down
            zone->resizeGrid(zoneW, zoneH - 1, 0, -1);
            changesSaved = false;
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand down
            zone->resizeGrid(zoneW, zoneH + 1, 0, 1);
            changesSaved = false;
            return;
        }

        viewX = *scrollX + zoneW * 16;
        viewY = getLevelBoundButtonY();

        if (inBoundButton(viewX, viewY - 16)) {
            // Shrink right
            zone->resizeGrid(zoneW - 1, zoneH, 0, 0);
            changesSaved = false;
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand right
            zone->resizeGrid(zoneW + 1, zoneH, 0, 0);
            changesSaved = false;
            return;
        }

        viewX = *scrollX - 16;

        if (inBoundButton(viewX, viewY - 16)) {
            // Shrink left
            zone->resizeGrid(zoneW - 1, zoneH, -1, 0);
            changesSaved = false;
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand left
            zone->resizeGrid(zoneW + 1, zoneH, 1, 0);
            changesSaved = false;
            return;
        }
    }

    void mimickTile() {
        int layer;
        GameObject* pickedObject = getCurrentZone()->getUpperGridObject(tileHoverX, tileHoverY, layer);
        if (pickedObject == GameObjectCache::objects["air"]) {
            pickedObject = nullptr;
        }

        picker->setSelectedItem(pickedObject);
    }

    public:
    LevelEditorUI(ILevelEditorScreen* screen, int width, int height, int& scrollX, int& scrollY, bool* keys) : UIBundle(bundleElements, NUM_BUNDLE_ELEMENTS), screen(screen), width(width), height(height), scrollX(&scrollX), scrollY(&scrollY), keys(keys) {
        searchBar = new TextInput("search objects", 16, 30, searchBarBuffer, [](TextInput* input, char* value) {
            LevelEditorUI* editorUI = (LevelEditorUI*) input->getPointer();
            editorUI->getObjectPicker().updateSearchFilter(value);
        });

        // Editor Bundle Elements
        searchBar->setPointer(this);
        searchBar->setPosition(8, height - 28);

        menuBar.setPointer(screen);

        ObjectPickerLoader loader;
        picker = loader.loadObjectPicker();

        bundleElements[0] = &menuBar;
        bundleElements[1] = searchBar;
        bundleElements[2] = picker;
    }

    // Function should run 60 times per second when active
    void tick() {
        if (keys[GLFW_KEY_R]) {
            screen->runLevel();
            return;
        }

        if (hoverMode == NO_HOVER) {
            return;
        }

        int speed = keys[GLFW_KEY_LEFT_SHIFT] ? 8 : 2;

        if (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_A]) {
            (*scrollX) += speed;
        }
        if (keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_D]) {
            (*scrollX) -= speed;
        }
        if (keys[GLFW_KEY_UP] || keys[GLFW_KEY_W]) {
            (*scrollY) -= speed;
        }
        if (keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_S]) {
            (*scrollY) += speed;
        }
        boundScroll();
        dragMario();
        if (hoverMode == TILE) {
            setTileHover();
        }
    }

    MenuBar& getMenuBar() {
        return menuBar;
    }

    TextInput& getSearchBar() {
        return *searchBar;
    }

    ObjectPicker& getObjectPicker() {
        return *picker;
    }

    int getTileHoverX() {
        return tileHoverX;
    }
    
    int getTileHoverY() {
        return tileHoverY;
    }

    HoverMode getHoverMode() {
        return hoverMode;
    }

    bool isMarioMouseDown() {
        return marioMouseDown;
    }

    bool isHoverInsertItem() {
        return hoverInsertItem;
    }

    void setChangesSaved(bool value) {
        changesSaved = value;
    }

    bool getChangesSaved() {
        return changesSaved;
    }

    void setAutosave(bool value) {
        autosave = value;
    }

    void attemptAutosave() {
        if (!autosave || changesSaved || screen->getSavePath() == "") {
            return;
        }

        LevelSaver saver = LevelSaver();
        try {
            saver.saveLevel(*screen->getLevel(), screen->getSavePath().c_str());
            screen->setChangesSaved(true);
        } catch (const char* errorMsg) {
            std::printf("%s\n", errorMsg);
            screen->setInfoMessage(errorMsg);
            screen->setState(ILevelEditorScreen::INFO_POPUP);
        }
    }

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        if (UIBundle::hover(x, y, gameWidth, gameHeight) || x < 144 || x >= gameWidth || y < 0 || y >= gameHeight - 16) {
            hoverMode = NO_HOVER;
            tileHoverX = -1;
            tileHoverY = -1;
            return true;
        }
        
        // Hover behavior for level
        hoverX = x - 144;
        hoverY = y;

        Mario* mario = &getCurrentZone()->getMario();
        if (hoverX - *scrollX >= mario->getX() && hoverX - *scrollX < mario->getX() + 16 && hoverY - *scrollY >= mario->getY() && hoverY - *scrollY < mario->getY() + 16) {
            hoverMode = MARIO;
        } else {
            hoverMode = TILE;
            setTileHover();
        }

        if (hoverMode != TILE) {
            tileHoverX = -1;
            tileHoverY = -1;
        }

        dragMario();

        return true;
    }

    void mouseDown() override {
        UIBundle::mouseDown();

        if (hoverMode == TILE) {
            isMouseDown = true;
            placeBlock(picker->getSelectedItem());
        } else if (hoverMode == MARIO) {
            Mario* mario = &getCurrentZone()->getMario();
            marioMouseDown = true;
            marioMouseOffsetX = hoverX - *scrollX - mario->getX();
            marioMouseOffsetY = hoverY - *scrollY - mario->getY();
        }
    }

    void mouseRightDown() override {
        UIBundle::mouseRightDown();

        if (hoverMode == TILE) {
            isMouseRightDown = true;
            eraseBlock();
        }
    }

    void click() override {
        UIBundle::click();

        isMouseDown = false;
        marioMouseDown = false;

        if (hoverMode != TILE) {
            return;
        }

        clickLevelBoundButtons();
    }

    void clickRight() override {
        UIBundle::clickRight();

        isMouseRightDown = false;
        eraseLayer = -1;
    }

    void clickMiddle() override {
        UIBundle::clickMiddle();

        if (hoverMode != TILE) {
            return;
        }

        mimickTile();
    }

    void scroll(double xOff, double yOff) override {
        if (hoverMode != NO_HOVER) {
            int speed = keys[GLFW_KEY_LEFT_SHIFT] ? 32 : 8;
            if (keys[GLFW_KEY_LEFT_CONTROL]) {
                *scrollX += yOff * -speed;
                *scrollY += xOff * speed;
            } else {
                *scrollX += xOff * speed;
                *scrollY += yOff * -speed;
            }
            boundScroll();
            if (hoverMode == TILE) {
                setTileHover();
            } else if (hoverMode == MARIO) {
                dragMario();
            }
        } else if (hoverMode == NO_HOVER) {
            UIBundle::scroll(xOff, yOff);
        }
    }

    void safeExitWindow() {
        attemptAutosave();
        if (changesSaved) {
            screen->closeWindow();
            return;
        }
        
        screen->getConfirmPopup()->setMessage("exit editor? (changes will not be saved)");
        screen->getConfirmPopup()->setConfirmCallback([](Button* button, UIButtonValue& value) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
            editorScreen->closeWindow();
        });
        screen->getConfirmPopup()->setCancelCallback([](PopupWindow* popup) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) popup->getPointer();
            editorScreen->setState(ILevelEditorScreen::EDITOR);
        });
        screen->setState(ILevelEditorScreen::CONFIRM_DIALOG);
    }

    virtual void charInput(int codepoint) override {
        UIBundle::charInput(codepoint);

        switch (codepoint) {
            case GLFW_KEY_ESCAPE:
                safeExitWindow();
                break;
        }
    }

    int getLevelBoundButtonX() {
        GameLevel* level = screen->getLevel();
        int zoneW = level->getCurrentZone()->getWidth() * 16;

        if (*scrollX > VIEW_WIDTH / 2 - 16) {
            return *scrollX + 16;
        } else if (*scrollX + zoneW < VIEW_WIDTH / 2 + 16) {
            return *scrollX + zoneW - 16;
        }
        return VIEW_WIDTH / 2;
    }

    int getLevelBoundButtonY() {
        GameLevel* level = screen->getLevel();
        int zoneH = level->getCurrentZone()->getHeight() * 16;

        if (*scrollY > VIEW_HEIGHT / 2 - 16) {
            return *scrollY + 16;
        } else if (*scrollY + zoneH < VIEW_HEIGHT / 2 + 16) {
            return *scrollY + zoneH - 16;
        }
        return VIEW_HEIGHT / 2;
    }

    int getScrollX() {
        return *scrollX;
    }

    int getScrollY() {
        return *scrollY;
    }

    GameLevel* getLevel() {
        return screen->getLevel();
    }

    GameLevelZone* getCurrentZone() {
        return screen->getLevel()->getCurrentZone();
    }

    ~LevelEditorUI() {
        delete searchBar;
        delete picker;
    }
};