#pragma once

enum UIButtonType {
    BUTTON,
    RADIO
};

union UIButtonValue {
    bool toggle;
    int numValue;
};

#include "ui/IUIElement.h"
#include "ui/MenuListButton.h"
#include "ui/MenuList.h"
#include "ui/MenuBar.h"