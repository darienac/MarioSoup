#pragma once

#include "game/GameLevel.h"
#include "screens/IScreen.h"

class ILevelEditorScreen: public IScreen {
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

    virtual bool isFullscreen() = 0;
    virtual void setFullscreen(bool value) = 0;

    virtual void runLevel() = 0;
    virtual void closeWindow() = 0;
};