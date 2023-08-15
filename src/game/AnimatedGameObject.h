#pragma once

#include <vector>

#include "game/GameObject.h"

class AnimatedGameObject: public GameObject {
    private:
    static int tick;

    int frameTime;
    std::vector<int> tiles;

    public:
    AnimatedGameObject(const char* id, const char* name, int tilePreview, int frameTime): GameObject(id, name, tilePreview), frameTime(frameTime) {}

    static void setTickNum(int value) {
        tick = value;
    }

    AnimatedGameObject& add(int tile) {
        tiles.push_back(tile);
        return *this;
    }

    AnimatedGameObject& add(int tileStart, int numTiles) {
        for (int i = 0; i < numTiles; i++) {
            add(tileStart + i);
        }
        return *this;
    }

    virtual int getLevelTile(GameObject* left, GameObject* right, GameObject* up, GameObject* down) override {
        return tiles.at((tick / frameTime) % tiles.size());
    }
};

int AnimatedGameObject::tick = 0;