#pragma once

#include "ui/ui.h"
#include "ui/leveleditor/ILevelEditorScreen.h"

class ResizeDialog: public PopupWindow {
    private:
    ILevelEditorScreen* screen;
    TextInput* widthInput;
    TextInput* heightInput;
    char widthInputBuffer[16];
    char heightInputBuffer[16];

    Button confirmButton = Button("ok", 4, [](Button* button, UIButtonValue& value) {
        ResizeDialog* resizeDialog = (ResizeDialog*) button->getPointer();
        ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) resizeDialog->getPointer();

        int width = atoi(resizeDialog->getWidthInputBuffer());
        int height = atoi(resizeDialog->getHeightInputBuffer());

        if (width <= 0 || height <= 0) {
            editorScreen->setInfoMessage("Invalid bounds for zone");
            editorScreen->setState(ILevelEditorScreen::INFO_POPUP);
            return;
        }

        editorScreen->getLevel()->getCurrentZone()->resizeGrid(width, height, 0, 0);
        editorScreen->setState(ILevelEditorScreen::EDITOR);
    });

    IUIElement* dialogElements[3];

    public:
    ResizeDialog(ILevelEditorScreen* screen): PopupWindow("enter dimensions", 20, 10, dialogElements, 3), screen(screen) {
        widthInput = new TextInput("width", 6, 15, widthInputBuffer, nullptr);
        heightInput = new TextInput("height", 6, 15, heightInputBuffer, nullptr);

        widthInput->setPosition(8, 8);
        heightInput->setPosition(64, 8);
        confirmButton.setPosition(120, 8);
        confirmButton.setPointer(this);

        dialogElements[0] = widthInput;
        dialogElements[1] = heightInput;
        dialogElements[2] = &confirmButton;

        setCanCancel(true);
        setPointer(screen);
        setCancelCallback([](PopupWindow* popup) {
            ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) popup->getPointer();

            editorScreen->setState(ILevelEditorScreen::UIState::EDITOR);
        });
    }

    void enable() {
        widthInput->clear(std::to_string(screen->getLevel()->getCurrentZone()->getWidth()));
        heightInput->clear(std::to_string(screen->getLevel()->getCurrentZone()->getHeight()));
        confirmButton.setHovered(false);
        widthInput->activate();
    }

    char* getWidthInputBuffer() {
        return widthInputBuffer;
    }

    char* getHeightInputBuffer() {
        return heightInputBuffer;
    }

    ~ResizeDialog() {
        delete widthInput;
        delete heightInput;
    }
};