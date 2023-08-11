#pragma once

#include "game/GameLevel.h"
#include "screens/IScreen.h"

class IPlayLevelScreen: public IScreen {
    public:
    static const int WINDOW_WIDTH = 256;
    static const int WINDOW_HEIGHT = 240;
    
    virtual void exitWindow() = 0;
    virtual GameLevel* getLevel() = 0;
    virtual void setLevel(GameLevel* level) = 0;
    virtual void exitToEditor() = 0;

    virtual int getScrollX() = 0;
    virtual void setScrollX(int value) = 0;
    virtual int getScrollY() = 0;
    virtual void setScrollY(int value) = 0;
};