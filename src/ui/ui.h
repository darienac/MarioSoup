#pragma once

enum class UIButtonType {
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
#include "ui/PopupWindow.h"
#include "ui/Button.h"