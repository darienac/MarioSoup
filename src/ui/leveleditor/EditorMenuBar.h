#pragma once

#include "ui/MenuBar.h"
#include "ui/leveleditor/ConfirmPopup.h"
#include "ui/leveleditor/ILevelEditorScreen.h"

class EditorMenuBar: public MenuBar {
    private:
    const char* menuItems[4] = {
        "file",
        "edit",
        "view",
        "run"
    };

    MenuListButton buttonsFile[4] = {
        MenuListButton("new", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
            if (editorScreen->getChangesSaved()) {
                editorScreen->initLevel();
                editorScreen->setState(ILevelEditorScreen::EDITOR);
                return;
            }

            editorScreen->getConfirmPopup()->setMessage("overwrite current level? (changes will not be saved)");
            editorScreen->getConfirmPopup()->setConfirmCallback([](Button* button, UIButtonValue& value) {
                ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
                editorScreen->initLevel();
                editorScreen->setState(ILevelEditorScreen::EDITOR);
            });
            editorScreen->getConfirmPopup()->setCancelCallback([](PopupWindow* popup) {
                ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) popup->getPointer();
                editorScreen->setState(ILevelEditorScreen::EDITOR);
            });
            editorScreen->setState(ILevelEditorScreen::CONFIRM_DIALOG);
        }),
        MenuListButton("open", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            if (editorScreen->getChangesSaved()) {
                editorScreen->setState(ILevelEditorScreen::OPEN_DIALOG);
                return;
            }
            
            editorScreen->getConfirmPopup()->setMessage("overwrite current level? (changes will not be saved)");
            editorScreen->getConfirmPopup()->setConfirmCallback([](Button* button, UIButtonValue& value) {
                ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
                editorScreen->setState(ILevelEditorScreen::OPEN_DIALOG);
            });
            editorScreen->getConfirmPopup()->setCancelCallback([](PopupWindow* popup) {
                ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) popup->getPointer();
                editorScreen->setState(ILevelEditorScreen::EDITOR);
            });
            editorScreen->setState(ILevelEditorScreen::CONFIRM_DIALOG);
        }),
        MenuListButton("save", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            if (editorScreen->getSavePath() == "") {
                editorScreen->setState(ILevelEditorScreen::SAVE_DIALOG);
                return;
            }

            LevelSaver saver = LevelSaver();
            try {
                saver.saveLevel(*editorScreen->getLevel(), editorScreen->getSavePath().c_str());
                editorScreen->setChangesSaved(true);
            } catch (const char* errorMsg) {
                std::printf("%s\n", errorMsg);
                editorScreen->setInfoMessage(errorMsg);
                editorScreen->setState(ILevelEditorScreen::INFO_POPUP);
            }
        }),
        MenuListButton("save as", UIButtonType::BUTTON, [](MenuListButton* button) {
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
        MenuList(buttonsFile, 4, 12),
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