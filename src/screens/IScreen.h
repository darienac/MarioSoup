#pragma once

class ScreenManager;
class GlWindow;

class IScreen {
    protected:
    GlWindow* window;
    ScreenManager* manager;

    public:
    virtual void enable() {}
    virtual void disable() {}
    virtual void renderFrame() {}
    virtual void tick() {}
};