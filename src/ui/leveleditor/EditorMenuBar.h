#pragma once

#include "ui/MenuBar.h"
#include "ui/leveleditor/ILevelEditorScreen.h"

class EditorMenuBar: public MenuBar {
    private:
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
    MenuListButton buttonsEdit[1] = {
        MenuListButton("resize", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            editorScreen->setState(ILevelEditorScreen::RESIZE_DIALOG);
        })
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
    MenuList menuLists[4] = {
        MenuList(buttonsFile, 2, 12),
        MenuList(buttonsEdit, 1, 12),
        MenuList(buttonsView, 1, 12),
        MenuList(buttonsRun, 1, 12)
    };


    public:
    EditorMenuBar(): MenuBar(menuItems, 4, menuLists, 144, 0, 32) {
        buttonsView[0].setOnListOpen([](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            button->setToggle(editorScreen->isFullscreen());
        });
    }
};