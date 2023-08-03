#pragma once

enum class UIButtonType {
    BUTTON,
    RADIO
};

enum class UITextInputType {
    TEXT,
    NUMBER
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
#include "ui/TextInput.h"
#include "ui/ObjectPicker.h"
#include "ui/UIBundle.h"
#include "ui/TextRegion.h"