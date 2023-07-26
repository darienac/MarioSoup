#pragma once

#include "game/GameObject.h"
#include "game/GameObjectCache.h"
#include "ui/ui.h"

using namespace GameObjectCache;

class LevelEditorUI: public UIBundle {
    private:
    static const int NUM_BUNDLE_ELEMENTS = 3;
    
    const char* menuItems[2] = {
        "file",
        "edit"
    };

    MenuListButton buttons1[2] = {
        MenuListButton("open", UIButtonType::BUTTON, nullptr),
        MenuListButton("save", UIButtonType::BUTTON, nullptr)
    };
    MenuListButton buttons2[2] = {
        MenuListButton("edit 1", UIButtonType::BUTTON, nullptr),
        MenuListButton("toggle", UIButtonType::RADIO, nullptr)
    };
    MenuList lists[2] = {
        MenuList(buttons1, 2, 8),
        MenuList(buttons2, 2, 8)
    };
    MenuBar menuBar = MenuBar(menuItems, 2, lists, 144, 0, 32);

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

    GameLevel* level;

    bool isHover = false;
    int* scrollX;
    int* scrollY;
    int tileHoverX = -1;
    int tileHoverY = -1;
    int lastTileHoverX = -1;
    int lastTileHoverY = -1;
    GameObject* lastPlacedObject = nullptr;
    bool isMouseDown = false;
    bool isMouseRightDown = false;

    void placeBlock(GameObject* object) {
        if (object == nullptr) {
            return;
        }
        if (object != lastPlacedObject || tileHoverX != lastTileHoverX || tileHoverY != lastTileHoverY) {
            lastTileHoverX = tileHoverX;
            lastTileHoverY = tileHoverY;

            level->setGridObject(object, tileHoverX, tileHoverY);
        }
    }

    public:
    LevelEditorUI(GameLevel& level, int width, int height, int& scrollX, int& scrollY) : UIBundle(bundleElements, NUM_BUNDLE_ELEMENTS), level(&level), scrollX(&scrollX), scrollY(&scrollY) {
        searchBar = new TextInput("search objects", 16, 30, searchBarBuffer, [](TextInput* input, char* value) {
            LevelEditorUI* editorUI = (LevelEditorUI*) input->getPointer();
            editorUI->getObjectPicker().updateSearchFilter(value);
        });

        searchBar->setPointer(this);
        searchBar->setPosition(8, height - 28);

        picker = new ObjectPicker(groups2, 2, 18, 28);
        picker->getGroups()[0].setOpen(true);

        bundleElements[0] = &menuBar;
        bundleElements[1] = searchBar;
        bundleElements[2] = picker;
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
            tileHoverX = -1;
            tileHoverY = -1;
            return true;
        }
        
        // Hover behavior for level
        x -= 144;
        tileHoverX = (x - *scrollX) / 16;
        tileHoverY = (y - *scrollY) / 16;
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
    }

    void clickRight() override {
        UIBundle::clickRight();

        isMouseRightDown = false;
    }

    ~LevelEditorUI() {
        delete searchBar;
        delete picker;
    }
};