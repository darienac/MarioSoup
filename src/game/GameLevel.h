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

    void resizeGrid(int newWidth, int newHeight, int xOff, int yOff) {
        if (newWidth <= 0 || newHeight <= 0) {
            return;
        }

        GameObject** newGrid = new GameObject*[newWidth * newHeight];

        for (int i = 0; i < newWidth * newHeight; i++) {
            int xOld = (i % newWidth) - xOff;
            int yOld = (i / newWidth) - yOff;
            newGrid[i] = getGridObject(xOld, yOld);
        }

        delete objectGrid;
        objectGrid = newGrid;

        width = newWidth;
        height = newHeight;
    }

    ~GameLevel() {
        delete objectGrid;
    }
};