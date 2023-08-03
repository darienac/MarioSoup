#pragma once

#include "game/GameLevel.h"

class ILevelEditorScreen {
    public:
    enum UIState {
        EDITOR, SAVE_DIALOG, OPEN_DIALOG, INFO_POPUP
    };

    virtual UIState getState() = 0;
    virtual void setState(UIState state) = 0;
    virtual GameLevel* getLevel() = 0;
    virtual void setLevel(GameLevel* level) = 0;

    virtual void setInfoMessage(const char* message) = 0;
    virtual char* getFileInputBuffer() = 0;
};