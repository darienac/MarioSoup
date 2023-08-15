#pragma once

#include "ui/IUIElement.h"
#include "ui/playlevel/IPlayLevelScreen.h"
#include "controls/KeyboardControls.h"
#include "audio/AudioManager.h"
#include "audio/AudioCache.h"

class PlayLevelUI: public IUIElement {
    private:
    IPlayLevelScreen* screen;
    AudioManager* audio;
    IControls* controls;
    bool* keys;

    void updateScroll() {
        int scrollX = screen->getScrollX();
        int scrollY = screen->getScrollY();
        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        Mario* mario = &zone->getMario();
        if (mario->getX() + scrollX > 128) {
            scrollX = 128 - mario->getX();
        } else if (mario->getX() + scrollX < 112) {
            scrollX = 112 - mario->getX();
        }

        if (scrollX < IPlayLevelScreen::WINDOW_WIDTH - zone->getWidth() * 16) {
            scrollX = IPlayLevelScreen::WINDOW_WIDTH - zone->getWidth() * 16;
        }
        if (scrollX > 0) {
            scrollX = 0;
        }

        if (scrollY < IPlayLevelScreen::WINDOW_HEIGHT - zone->getHeight() * 16) {
            scrollY = IPlayLevelScreen::WINDOW_HEIGHT - zone->getHeight() * 16;
        }
        if (scrollY > 0) {
            scrollY = 0;
        }

        screen->setScrollX(scrollX);
        screen->setScrollY(scrollY);
    }

    void updateAudioListener() {
        int scrollX = screen->getScrollX();
        int scrollY = screen->getScrollY();

        float pos[3] = {(float) -scrollX + IPlayLevelScreen::WINDOW_WIDTH / 2, (float) -scrollY + IPlayLevelScreen::WINDOW_HEIGHT / 2, 128};
        audio->setPos(pos);
    }

    public:
    PlayLevelUI(IPlayLevelScreen* screen, AudioManager* audio, bool* keys): screen(screen), audio(audio), keys(keys) {
        controls = new KeyboardControls(keys);
    }

    void tick() {
        GameLevelZone* zone = screen->getLevel()->getCurrentZone();
        zone->getMario().tick(*zone, *audio, *controls);

        updateScroll();
        updateAudioListener();
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