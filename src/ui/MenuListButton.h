#pragma once

#include "ui/ui.h"

// Doesn't implement IUIElement since bounding boxes are calculated by a parent MenuList

class MenuListButton {
    private:
    const char* name;
    UIButtonType type;
    void (*callback)(MenuListButton* button, UIButtonValue& value);
    UIButtonValue value = {};

    public:
    MenuListButton(const char* name, UIButtonType type, void (*callback)(MenuListButton* button, UIButtonValue& value)): name(name), type(type), callback(callback) {}

    void setToggle(UIButtonValue value) {
        this->value = value;
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

    void click() {
        if (type == UIButtonType::RADIO) {
            value.toggle = !value.toggle;
        }
        if (callback != nullptr) {
            callback(this, value);
        }
    }
};