#pragma once

#include "GlWindow.h"
#include "screens/ScreenManager.h"

class ScreenManager;

class IScreen {
    protected:
    GlWindow* window;
    ScreenManager* manager;

    public:
    virtual void enable() {}
    virtual void disable() {}
    virtual void renderFrame() {}
};