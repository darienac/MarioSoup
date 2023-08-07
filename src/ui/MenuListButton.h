#pragma once

#include "ui/ui.h"

// Doesn't implement IUIElement since bounding boxes are calculated by a parent MenuList

class MenuListButton {
    public:
    typedef void (*button_callback)(MenuListButton* button);

    private:
    const char* name;
    UIButtonType type;
    void* pointer;
    button_callback onClick;
    button_callback onListOpen = nullptr;

    UIButtonValue value = {};

    public:
    MenuListButton(const char* name, UIButtonType type, button_callback onClick): name(name), type(type), onClick(onClick) {}

    void setToggle(bool value) {
        this->value.toggle = value;
    }

    void setOnListOpen(button_callback callback) {
        onListOpen = callback;
    }

    UIButtonValue getValue() {
        return value;
    }

    const char* getLabel() {
        return name;
    }

    UIButtonType getType() {
        return type;
    }

    void setPointer(void* pointer) {
        this->pointer = pointer;
    }

    void* getPointer() {
        return pointer;
    }

    bool isAllowCloseList() {
        return type != UIButtonType::RADIO;
    }

    void click() {
        if (type == UIButtonType::RADIO) {
            value.toggle = !value.toggle;
        }
        if (onClick != nullptr) {
            onClick(this);
        }
    }

    void listOpen() {
        if (onListOpen != nullptr) {
            onListOpen(this);
        }
    }
};