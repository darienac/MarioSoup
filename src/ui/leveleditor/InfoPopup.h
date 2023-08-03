#pragma once

#include "ui/ui.h"
#include "ui/leveleditor/ILevelEditorScreen.h"

class InfoPopup: public PopupWindow {
    private:
    TextRegion textRegion = TextRegion("", 16);
    Button confirmInfoButton = Button("ok", 4, [](Button* button, UIButtonValue& value) {
        ILevelEditorScreen* editorScreen = (ILevelEditorScreen*) button->getPointer();
        editorScreen->setState(ILevelEditorScreen::EDITOR);
    });
    IUIElement* infoElements[2];

    public:
    InfoPopup(ILevelEditorScreen* screen) : PopupWindow("info", 18, 10, infoElements, 2) {
        textRegion.setPosition(8, 48);
        confirmInfoButton.setPosition(104, 8);
        confirmInfoButton.setPointer(screen);

        infoElements[0] = &textRegion;
        infoElements[1] = &confirmInfoButton;
    }

    void setMessage(const char* text) {
        textRegion.setText(text);
    }
};