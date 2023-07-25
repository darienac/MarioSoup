#pragma once

#include "game/GameObject.h"
#include "game/GameObjectCache.h"

class GameLevel {
    private:
    int width;
    int height;
    GameObject** objectGrid;

    public:
    GameLevel(int width, int height): width(width), height(height) {
        objectGrid = new GameObject*[width * height];
        for (int i = 0; i < width * height; i++) {
            objectGrid[i] = GameObjectCache::objects["air"];
        }
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    GameObject* getGridObject(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return GameObjectCache::objects["air"];
        }
        return objectGrid[width * y + x];
    }

    void setGridObject(GameObject* object, int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        objectGrid[width * y + x] = object;
    }

    ~GameLevel() {
        delete objectGrid;
    }
};