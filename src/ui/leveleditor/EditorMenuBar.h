#pragma once

#include "ui/MenuBar.h"
#include "ui/leveleditor/ConfirmPopup.h"
#include "ui/leveleditor/TextInputPopup.h"
#include "ui/leveleditor/ILevelEditorScreen.h"

class EditorMenuBar: public MenuBar {
    private:
    const char* menuItems[4] = {
        "file",
        "edit",
        "view",
        "run"
    };

    MenuListButton buttonsFile[5] = {
        MenuListButton("new", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
            editorScreen->attemptAutosave();
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
            editorScreen->attemptAutosave();

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
        }),
        MenuListButton("autosave", UIButtonType::RADIO, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
            editorScreen->setAutosave(button->getValue().toggle);
        })
    };
    MenuListButton buttonsEdit[2] = {
        MenuListButton("resize", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            editorScreen->setState(ILevelEditorScreen::RESIZE_DIALOG);
        }),
        MenuListButton("set music", UIButtonType::BUTTON, [](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            editorScreen->getTextInputPopup()->setMessage("enter music path, blank for none (in audio folder)");
            editorScreen->getTextInputPopup()->setConfirmCallback([](Button* button, UIButtonValue& value) {
                TextInputPopup* popupWindow = (TextInputPopup*) button->getPointer();
                ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) popupWindow->getPointer();
                editorScreen->getLevel()->getCurrentZone()->setMusicPath(popupWindow->getTextInput());
                editorScreen->setState(ILevelEditorScreen::EDITOR);
                editorScreen->setChangesSaved(false);
            });
            editorScreen->setState(ILevelEditorScreen::TEXT_INPUT_DIALOG);
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
        MenuList(buttonsFile, 5, 12),
        MenuList(buttonsEdit, 2, 12),
        MenuList(buttonsView, 1, 12),
        MenuList(buttonsRun, 1, 12)
    };


    public:
    EditorMenuBar(): MenuBar(menuItems, 4, menuLists, 144, 0, 32) {
        buttonsView[0].setOnListOpen([](MenuListButton* button) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();

            button->setToggle(editorScreen->isFullscreen());
        });
        buttonsFile[4].setToggle(true);
    }
};