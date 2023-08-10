#pragma once

#include "game/GameObject.h"
#include "game/GameObjectCache.h"

class Mario {
    private:
    GameObject gameObject;
    int x = 0;
    int y = 0;
    int zoneLayer = 1;

    public:
    // Mario gets own copy of player object to manipulate
    // Mario itself is also copied with the level when the level is played
    Mario(): gameObject(*GameObjectCache::objects["player"]) {}

    void resetGameObject() {
        gameObject = *GameObjectCache::objects["player"];
    }

    int getX() {
        return x;
    }

    void setX(int value) {
        x = value;
    }

    int getY() {
        return y;
    }

    void setY(int value) {
        y = value;
    }
};