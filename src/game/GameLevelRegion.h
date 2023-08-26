#pragma once

#include <set>
#include <map>
#include <vector>

#include "game/objects/GameObject.h"
#include "game/objects/GameObjectCache.h"
#include "game/IGameLevelRegion.h"
#include "game/entities/IEntity.h"

class GameLevelRegion: public IGameLevelRegion {
    public:
    static const int MAX_WIDTH = 65536;

    private:
    int width;
    int height;
    int zoneLayer;
    GameObject** objectGrid;
    std::map<int, ObjectData> gridData;
    std::set<IEntity*, IGameLevelRegion::entityCmp> entities;

    public:
    GameLevelRegion(int width, int height, int zoneLayer): width(width), height(height), zoneLayer(zoneLayer) {
        objectGrid = new GameObject*[width * height];
        for (int i = 0; i < width * height; i++) {
            objectGrid[i] = GameObjectCache::objects["air"];
        }
    }

    GameLevelRegion(IGameLevelRegion& orig) {
        std::printf("New GameLevelRegion clone\n");
        width = orig.getWidth();
        height = orig.getHeight();
        zoneLayer = orig.getZoneLayer();
        int size = width * height;
        objectGrid = new GameObject*[size];
        GameObject** grid = orig.getObjectGrid();
        for (int i = 0; i < size; i++) {
            objectGrid[i] = grid[i];
        }
        gridData = orig.getGridData();
        // Don't copy entities (are created at play level time only)
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

    virtual int getZoneLayer() {
        return zoneLayer;
    }

    virtual IEntity* addEntity(IEntity* entity) override {
        entities.insert(entity);
        return entity;
    }

    virtual void removeEntity(IEntity* entity) override {
        entities.erase(entity);
        delete entity;
    }

    virtual std::set<IEntity*, IGameLevelRegion::entityCmp>& getEntities() override {
        return entities;
    }

    virtual void addGridData(int x, int y, ObjectData data) override {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        gridData[MAX_WIDTH * y + x] = data;
    }

    virtual void removeGridData(int x, int y) override {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        gridData.erase(MAX_WIDTH * y + x);
    }

    virtual ObjectData getGridData(int x, int y) override {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return {nullptr};
        }

        if (gridData.find(MAX_WIDTH * y + x) == gridData.end()) {
            return {nullptr};
        } else {
            return gridData[MAX_WIDTH * y + x];
        }
    }

    virtual std::map<int, ObjectData>& getGridData() override {
        return gridData;
    }

    virtual void mapUsedObjects(std::set<GameObject*>& objects) override {
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
        for (IEntity* entity : entities) {
            delete entity;
        }
    }
};