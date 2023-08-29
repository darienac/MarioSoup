#pragma once

#include <vector>

#include "game/backgrounds/GameBackgroundLayer.h"

class GameBackground {
    private:
    std::vector<GameBackgroundLayer*> layers;
    std::string id;
    std::string preferredMusic = AudioCache::music["sma4:overworld"];
    unsigned char bgColor[3] = {0, 0, 0};

    public:
    GameBackground(std::string id): id(id) {}

    GameBackground& addLayer(GameBackgroundLayer* layer) {
        layers.push_back(layer);
        return *this;
    }

    std::vector<GameBackgroundLayer*>& getLayers() {
        return layers;
    }

    std::string& getId() {
        return id;
    }

    GameBackground& setPreferredMusic(std::string path) {
        preferredMusic = path;
        return *this;
    }

    std::string getPreferredMusic() {
        return preferredMusic;
    }

    GameBackground& setBgColor(unsigned char r, unsigned char g, unsigned char b) {
        bgColor[0] = r;
        bgColor[1] = g;
        bgColor[2] = b;
        return *this;
    }

    unsigned char* getBgColor() {
        return bgColor;
    }

    ~GameBackground() {
        for (GameBackgroundLayer* layer : layers) {
            delete layer;
        }
    }
};