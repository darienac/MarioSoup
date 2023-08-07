#pragma once

#include "game/GameObject.h"
#include "game/GameObjectCache.h"
#include "game/LevelSaver.h"
#include "game/LevelLoader.h"
#include "ui/leveleditor/ILevelEditorScreen.h"
#include "ui/ui.h"

#include <GLFW/glfw3.h>

using namespace GameObjectCache;

class LevelEditorUI: public UIBundle {
    private:
    static const int NUM_BUNDLE_ELEMENTS = 3;

    static const int VIEW_WIDTH = 256;
    static const int VIEW_HEIGHT = 240;
    
    // Editor Bundle Elements
    const char* menuItems[4] = {
        "file",
        "edit",
        "view",
        "run"
    };

    MenuListButton buttonsFile[2] = {
        MenuListButton("open", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            editorScreen->setState(ILevelEditorScreen::OPEN_DIALOG);
        }),
        MenuListButton("save", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            editorScreen->setState(ILevelEditorScreen::SAVE_DIALOG);
        })
    };
    MenuListButton buttonsEdit[2] = {
        MenuListButton("edit 1", UIButtonType::BUTTON, nullptr),
        MenuListButton("toggle", UIButtonType::RADIO, nullptr)
    };
    MenuListButton buttonsView[1] = {
        MenuListButton("fullscreen", UIButtonType::RADIO, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            editorScreen->setFullscreen(button->getValue().toggle);
        })
    };
    MenuListButton buttonsRun[1] = {
        MenuListButton("run level", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            editorScreen->runLevel();
        })
    };
    MenuList lists[4] = {
        MenuList(buttonsFile, 2, 12),
        MenuList(buttonsEdit, 2, 12),
        MenuList(buttonsView, 1, 12),
        MenuList(buttonsRun, 1, 12)
    };
    MenuBar menuBar = MenuBar(menuItems, 4, lists, 144, 0, 32);

    TextInput* searchBar;
    char searchBarBuffer[32];

    GameObject* objectList1[2] = {objects["smb:rock"], objects["smb:brick"]};
    GameObject* objectList2[2] = {objects["smb:brick"], objects["smb:rock"]};

    ObjectPickerGroup groups[2] = {
        ObjectPickerGroup("group 1", nullptr, 0, objectList1, 2),
        ObjectPickerGroup("group 2", nullptr, 0, nullptr, 0)
    };
    ObjectPickerGroup groups2[2] = {
        ObjectPickerGroup("group a", groups, 2, objectList2, 2),
        ObjectPickerGroup("group b", nullptr, 0, nullptr, 0)
    };
    ObjectPicker* picker;

    IUIElement* bundleElements[NUM_BUNDLE_ELEMENTS];

    bool isHover = false;

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
    GameObject* lastPlacedObject = nullptr;
    bool isMouseDown = false;
    bool isMouseRightDown = false;

    void placeBlock(GameObject* object) {
        GameLevel* level = screen->getLevel();
        if (object == nullptr) {
            return;
        }
        if (object != lastPlacedObject || tileHoverX != lastTileHoverX || tileHoverY != lastTileHoverY) {
            lastTileHoverX = tileHoverX;
            lastTileHoverY = tileHoverY;

            level->getCurrentRegion()->setGridObject(object, tileHoverX, tileHoverY);
        }
    }

    void setTileHover() {
        if (hoverX < *scrollX || hoverY < *scrollY) {
            tileHoverX = -1;
            tileHoverY = -1;
            return;
        }
        tileHoverX = (hoverX - *scrollX) / 16;
        tileHoverY = (hoverY - *scrollY) / 16;
    }

    void boundScroll() {
        GameLevel* level = screen->getLevel();
        int w = level->getCurrentRegion()->getWidth();
        int h = level->getCurrentRegion()->getHeight();

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

    void clickLevelBoundButtons() {
        GameLevel* level = screen->getLevel();
        int xOffset = 144;
        int yOffset = 0;

        auto inBoundButton = [this, &xOffset, &yOffset](int x, int y) {
            int mx = hoverX;
            int my = hoverY; 
            return mx >= x && mx < x + 16 && my >= y && my < y + 16;
        };

        GameLevelRegion* region = level->getCurrentRegion();

        int regionW = region->getWidth();
        int regionH = region->getHeight();

        int viewX;
        int viewY;

        viewX = getLevelBoundButtonX();
        viewY = *scrollY + regionH * 16;

        if (inBoundButton(viewX - 16, viewY)) {
            // Shrink up
            region->resizeGrid(regionW, regionH - 1, 0, 0);
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand up
            region->resizeGrid(regionW, regionH + 1, 0, 0);
            return;
        }

        viewY = *scrollY - 16;

        if (inBoundButton(viewX - 16, viewY)) {
            // Shrink down
            region->resizeGrid(regionW, regionH - 1, 0, -1);
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand down
            region->resizeGrid(regionW, regionH + 1, 0, 1);
            return;
        }

        viewX = *scrollX + regionW * 16;
        viewY = getLevelBoundButtonY();

        if (inBoundButton(viewX, viewY - 16)) {
            // Shrink right
            region->resizeGrid(regionW - 1, regionH, 0, 0);
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand right
            region->resizeGrid(regionW + 1, regionH, 0, 0);
            return;
        }

        viewX = *scrollX - 16;

        if (inBoundButton(viewX, viewY - 16)) {
            // Shrink left
            region->resizeGrid(regionW - 1, regionH, -1, 0);
            return;
        }
        if (inBoundButton(viewX, viewY)) {
            // Expand left
            region->resizeGrid(regionW + 1, regionH, 1, 0);
            return;
        }
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

        buttonsView[0].setOnListOpen([](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            button->setToggle(editorScreen->isFullscreen());
        });

        picker = new ObjectPicker(groups2, 2, 18, 28);
        picker->getGroups()[0].setOpen(true);

        bundleElements[0] = &menuBar;
        bundleElements[1] = searchBar;
        bundleElements[2] = picker;
    }

    // Function should run 60 times per second when active
    void tick() {
        if (!isHover) {
            return;
        }

        int speed = keys[GLFW_KEY_LEFT_SHIFT] ? 8 : 2;

        if (keys[GLFW_KEY_LEFT]) {
            (*scrollX) += speed;
        }
        if (keys[GLFW_KEY_RIGHT]) {
            (*scrollX) -= speed;
        }
        if (keys[GLFW_KEY_UP]) {
            (*scrollY) -= speed;
        }
        if (keys[GLFW_KEY_DOWN]) {
            (*scrollY) += speed;
        }
        boundScroll();
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

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        isHover = !(UIBundle::hover(x, y, gameWidth, gameHeight) || x < 144 || x >= gameWidth || y < 0 || y >= gameHeight - 16);

        if (!isHover) {
            hoverX = -1;
            hoverY = -1;
            tileHoverX = -1;
            tileHoverY = -1;
            return true;
        }
        
        // Hover behavior for level
        hoverX = x - 144;
        hoverY = y;
        setTileHover();

        if (isMouseDown) {
            placeBlock(picker->getSelectedItem());
        } else if (isMouseRightDown) {
            placeBlock(GameObjectCache::objects["air"]);
        }

        return true;
    }

    void mouseDown() override {
        UIBundle::mouseDown();

        if (isHover) {
            isMouseDown = true;
            placeBlock(picker->getSelectedItem());
        }
    }

    void mouseRightDown() override {
        UIBundle::mouseRightDown();

        if (isHover) {
            isMouseRightDown = true;
            placeBlock(GameObjectCache::objects["air"]);
        }
    }

    void click() override {
        UIBundle::click();

        isMouseDown = false;

        if (!isHover) {
            return;
        }

        clickLevelBoundButtons();
    }

    void clickRight() override {
        UIBundle::clickRight();

        isMouseRightDown = false;
    }

    void scroll(double xOff, double yOff) override {
        if (isHover) {
            int speed = keys[GLFW_KEY_LEFT_SHIFT] ? 32 : 8;
            if (keys[GLFW_KEY_LEFT_CONTROL]) {
                *scrollX += yOff * -speed;
                *scrollY += xOff * speed;
            } else {
                *scrollX += xOff * speed;
                *scrollY += yOff * -speed;
            }
            boundScroll();
            setTileHover();
            if (isMouseDown) {
                placeBlock(picker->getSelectedItem());
            } else if (isMouseRightDown) {
                placeBlock(GameObjectCache::objects["air"]);
            }
        } else {
            UIBundle::scroll(xOff, yOff);
        }
    }

    virtual void charInput(int codepoint) override {
        if (codepoint == GLFW_KEY_ESCAPE) {
            screen->closeWindow();
        }
    }

    int getLevelBoundButtonX() {
        GameLevel* level = screen->getLevel();
        int regionW = level->getCurrentRegion()->getWidth() * 16;

        if (*scrollX > VIEW_WIDTH / 2 - 16) {
            return *scrollX + 16;
        } else if (*scrollX + regionW < VIEW_WIDTH / 2 + 16) {
            return *scrollX + regionW - 16;
        }
        return VIEW_WIDTH / 2;
    }

    int getLevelBoundButtonY() {
        GameLevel* level = screen->getLevel();
        int regionH = level->getCurrentRegion()->getHeight() * 16;

        if (*scrollY > VIEW_HEIGHT / 2 - 16) {
            return *scrollY + 16;
        } else if (*scrollY + regionH < VIEW_HEIGHT / 2 + 16) {
            return *scrollY + regionH - 16;
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

    GameLevelRegion* getCurrentRegion() {
        return screen->getLevel()->getCurrentRegion();
    }

    ~LevelEditorUI() {
        delete searchBar;
        delete picker;
    }
};