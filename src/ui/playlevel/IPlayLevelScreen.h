#pragma once

#include "game/GameLevel.h"
#include "screens/IScreen.h"

class IPlayLevelScreen: public IScreen {
    public:
    virtual void exitWindow() = 0;
    virtual void setLevel(GameLevel* level) = 0;
    virtual void exitToEditor() = 0;
};