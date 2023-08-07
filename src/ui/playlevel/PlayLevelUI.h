#pragma once

#include "ui/IUIElement.h"
#include "ui/playlevel/IPlayLevelScreen.h"

class PlayLevelUI: public IUIElement {
    private:
    IPlayLevelScreen* screen;
    bool* keys;

    public:
    PlayLevelUI(IPlayLevelScreen* screen, bool* keys): screen(screen), keys(keys) {}

    void tick() {
        
    }

    virtual void charInput(int codepoint) override {
        if (codepoint == GLFW_KEY_ESCAPE) {
            screen->exitToEditor();
        }
    }
};