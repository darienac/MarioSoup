#pragma once

#include <vector>

#include "game/objects/GameObject.h"

class AnimatedGameObject: public GameObject {
    private:
    struct Keyframe {
        int tile;
        bool flippedX;
        bool flippedY;
    };
    static int tick;

    int frameTime;
    std::vector<Keyframe> tiles;

    Keyframe& getCurrentKeyframe() {
        return tiles.at((tick / frameTime) % tiles.size());
    }

    public:
    AnimatedGameObject(const char* id, const char* name, int tilePreview, int frameTime): GameObject(id, name, tilePreview), frameTime(frameTime) {}

    static void setTickNum(int value) {
        tick = value;
    }

    AnimatedGameObject& add(int tile, bool flippedX, bool flippedY) {
        tiles.push_back({tile, flippedX, flippedY});
        return *this;
    }

    AnimatedGameObject& add(bool flippedX, bool flippedY) {
        return add(GameObject::getLevelTile(), flippedX, flippedY);
    }

    AnimatedGameObject& add(int tile) {
        return add(tile, false, false);
    }

    AnimatedGameObject& add(int tileStart, int numTiles) {
        for (int i = 0; i < numTiles; i++) {
            add(tileStart + i);
        }
        return *this;
    }

    virtual int getLevelTile(GameObject* left, GameObject* right, GameObject* up, GameObject* down) override {
        return getLevelTile();
    }

    virtual int getLevelTile() override {
        return getCurrentKeyframe().tile;
    }

    virtual bool isFlippedX() override {
        return getCurrentKeyframe().flippedX;
    }

    virtual bool isFlippedY() override {
        return getCurrentKeyframe().flippedY;
    }
};

int AnimatedGameObject::tick = 0;