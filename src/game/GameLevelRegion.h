#pragma once

#include <set>
#include <map>
#include <vector>

#include "game/objects/GameObject.h"
#include "game/objects/GameObjectCache.h"
#include "game/IGameLevelRegion.h"

class GameLevelRegion: public IGameLevelRegion {
    public:
    static const int MAX_WIDTH = 65536;
    union ObjectData {
        GameObject* containerObject;
    };

    private:
    int width;
    int height;
    GameObject** objectGrid;
    std::map<int, ObjectData> gridData;

    public:
    GameLevelRegion(int width, int height): width(width), height(height) {
        objectGrid = new GameObject*[width * height];
        for (int i = 0; i < width * height; i++) {
            objectGrid[i] = GameObjectCache::objects["air"];
        }
    }

    GameLevelRegion(GameLevelRegion& orig) {
        std::printf("New GameLevelRegion clone\n");
        width = orig.getWidth();
        height = orig.getHeight();
        int size = width * height;
        objectGrid = new GameObject*[size];
        GameObject** grid = orig.getObjectGrid();
        for (int i = 0; i < size; i++) {
            objectGrid[i] = grid[i];
        }
    }

    virtual int getWidth() override {
        return width;
    }

    virtual int getHeight() override {
        return height;
    }

    virtual GameObject** getObjectGrid() override {
        return objectGrid;
    }

    virtual GameObject* getGridObject(int x, int y) override {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return GameObjectCache::objects["air"];
        }
        return objectGrid[width * y + x];
    }

    virtual void setGridObject(GameObject* object, int x, int y) override {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        objectGrid[width * y + x] = object;
    }

    virtual void resizeGrid(int newWidth, int newHeight, int xOff, int yOff) override {
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

        std::vector<int> badKeys;
        for (const auto& [key, value] : gridData) {
            int x = key % MAX_WIDTH;
            int y = key / MAX_WIDTH;
            if (x < 0 || x >= width || y < 0 || y >= height) {
                badKeys.push_back(key);
            }
        }
        for (int key : badKeys) {
            gridData.erase(key);
        }
    }

    void addGridData(int x, int y, ObjectData data) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        gridData[MAX_WIDTH * y + x] = data;
    }

    void removeGridData(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        gridData.erase(MAX_WIDTH * y + x);
    }

    ObjectData getGridData(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return {nullptr};
        }

        if (gridData.find(MAX_WIDTH * y + x) == gridData.end()) {
            return {nullptr};
        } else {
            return gridData[MAX_WIDTH * y + x];
        }
    }

    std::map<int, ObjectData>& getGridData() {
        return gridData;
    }

    void mapUsedObjects(std::set<GameObject*>& objects) {
        for (int i = 0; i < width * height; i++) {
            objects.insert(objectGrid[i]);
        }
        for (const auto& [key, value] : gridData) {
            int x = key % MAX_WIDTH;
            int y = key / MAX_WIDTH;
            GameObject* object = getGridObject(x, y);
            if (object->isFlag(GameObject::CONTAINS_ITEM) && getGridData(x, y).containerObject != nullptr) {
                objects.insert(getGridData(x, y).containerObject);
            }
        }
    }

    virtual ~GameLevelRegion() {
        delete [] objectGrid;
    }
};