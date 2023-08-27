#pragma once

#include "ui/ui.h"

class ConfirmPopup: public PopupWindow {
    private:
    TextRegion textRegion = TextRegion("", 22);
    Button confirmInfoButton = Button("yes", 5, nullptr);
    Button cancelInfoButton = Button("no", 4, [](Button* button, UIButtonValue& value) {
        ConfirmPopup* popup = (ConfirmPopup*) button->getPointer();
        cancel_callback callback = popup->getCancelCallback();
        if (callback != nullptr) {
            callback(popup);
        }
    });

    IUIElement* confirmElements[3];
    public:
    ConfirmPopup(void* confirmPointer): PopupWindow("confirm", 24, 10, confirmElements, 3) {
        textRegion.setPosition(8, 48);
        confirmInfoButton.setPosition(8, 8);
        confirmInfoButton.setPointer(confirmPointer);
        cancelInfoButton.setPosition(56, 8);
        cancelInfoButton.setPointer(this);
        setPointer(confirmPointer);
        setCanCancel(true);

        confirmElements[0] = &textRegion;
        confirmElements[1] = &confirmInfoButton;
        confirmElements[2] = &cancelInfoButton;
    }

    void setMessage(const char* text) {
        textRegion.setText(text);
    }

    void setConfirmCallback(Button::button_callback callback) {
        confirmInfoButton.setCallback(callback);
    }
};