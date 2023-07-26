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

    public:
    LevelEditorUI(int width, int height) : UIBundle(bundleElements, NUM_BUNDLE_ELEMENTS) {
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

    ~LevelEditorUI() {
        delete searchBar;
        delete picker;
    }
};