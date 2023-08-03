#pragma once

#include "ui/ui.h"
#include "ui/leveleditor/ILevelEditorScreen.h"

class FilePickerPopup : public PopupWindow {
    private:
    TextInput* fileInput;
    char fileInputBuffer[256];
    Button confirmFileButton = Button("ok", 4, [](Button* button, UIButtonValue& value) {
        ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
        char* fileInputBuffer = editorScreen->getFileInputBuffer();

        switch (editorScreen->getState()) {
            case ILevelEditorScreen::EDITOR: {
                break;
            } case ILevelEditorScreen::SAVE_DIALOG: {
                LevelSaver saver = LevelSaver();
                try {
                    saver.saveLevel(*editorScreen->getLevel(), fileInputBuffer);
                } catch (const char* errorMsg) {
                    std::printf("%s\n", errorMsg);
                    editorScreen->setInfoMessage(errorMsg);
                    editorScreen->setState(ILevelEditorScreen::INFO_POPUP);
                    break;
                }

                editorScreen->setState(ILevelEditorScreen::EDITOR);
                break;
            } case ILevelEditorScreen::OPEN_DIALOG: {
                GameLevel* newLevel = new GameLevel();

                LevelLoader loader = LevelLoader();
                try {
                    loader.loadLevel(*newLevel, fileInputBuffer);
                    editorScreen->setLevel(newLevel);
                } catch (const char* errorMsg) {
                    std::printf("%s\n", errorMsg);
                    editorScreen->setInfoMessage(errorMsg);
                    editorScreen->setState(ILevelEditorScreen::INFO_POPUP);
                    delete newLevel;
                    break;
                }

                editorScreen->setState(ILevelEditorScreen::EDITOR);
                break;
            }
            default:
                break;
        }
    });
    IUIElement* filePickerElements[2];

    public:
    FilePickerPopup(ILevelEditorScreen* screen) : PopupWindow("enter filepath", 18, 10, filePickerElements, 2) {
        fileInput = new TextInput("file path", 11, 255, fileInputBuffer, nullptr);

        fileInput->setPosition(8, 8);
        confirmFileButton.setPosition(104, 8);
        confirmFileButton.setPointer(screen);

        filePickerElements[0] = fileInput;
        filePickerElements[1] = &confirmFileButton;
    }

    char* getFileInputBuffer() {
        return fileInputBuffer;
    }

    void enable() {
        fileInput->clear();
        fileInput->activate();
        confirmFileButton.setHovered(false);
    }

    ~FilePickerPopup() {
        delete fileInput;
    }
};