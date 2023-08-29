#pragma once

#include "ui/ui.h"

class TextInputPopup: public PopupWindow {
    private:
    char inputBuffer[256];
    TextRegion textRegion = TextRegion("", 22);
    TextInput* textInput;
    Button confirmTextButton = Button("ok", 4, nullptr);

    IUIElement* textInputElements[3];
    public:
    TextInputPopup(void* textInputPointer): PopupWindow("input", 24, 10, textInputElements, 3) {
        textInput = new TextInput("", 11, 255, inputBuffer, nullptr);
        textInput->setPosition(8, 8);
        textRegion.setPosition(8, 48);
        confirmTextButton.setPosition(104, 8);
        confirmTextButton.setPointer(this);
        setPointer(textInputPointer);
        setCanCancel(true);

        textInputElements[0] = &textRegion;
        textInputElements[1] = textInput;
        textInputElements[2] = &confirmTextButton;
    }

    void enable() {
        textInput->clear();
        textInput->activate();
        confirmTextButton.setHovered(false);
    }

    void setMessage(const char* text) {
        textRegion.setText(text);
    }

    std::string getTextInput() {
        return std::string(inputBuffer);
    }

    void setConfirmCallback(Button::button_callback callback) {
        confirmTextButton.setCallback(callback);
    }

    ~TextInputPopup() {
        delete textInput;
    }
};