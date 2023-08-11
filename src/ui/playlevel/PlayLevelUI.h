#pragma once

#include "ui/IUIElement.h"
#include "ui/playlevel/IPlayLevelScreen.h"
#include "controls/KeyboardControls.h"

class PlayLevelUI: public IUIElement {
    private:
    IPlayLevelScreen* screen;
    IControls* controls;
    bool* keys;

    public:
    PlayLevelUI(IPlayLevelScreen* screen, bool* keys): screen(screen), keys(keys) {
        controls = new KeyboardControls(keys);
    }

    void tick() {
        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        zone->getMario().tick(*zone, *controls);
    }

    virtual void charInput(int codepoint) override {
        if (codepoint == GLFW_KEY_ESCAPE) {
            screen->exitToEditor();
        }
    }

    ~PlayLevelUI() {
        delete controls;
    }
};