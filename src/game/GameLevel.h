#pragma once

#include <set>
#include <vector>

#include "game/GameObject.h"
#include "game/GameObjectCache.h"
#include "game/GameLevelZone.h"

class GameLevel {
    private:
    std::vector<GameLevelZone*> zones;
    GameLevelZone* currentZone = nullptr;

    public:
    GameLevel() {}

    GameLevelZone* addZone(int width, int height) {
        GameLevelZone* zone = new GameLevelZone(width, height);
        zones.push_back(zone);
        return zone;
    }

    std::vector<GameLevelZone*>* getZones() {
        return &zones;
    }

    void setCurrentZone(GameLevelZone* zone) {
        currentZone = zone;
    }

    GameLevelZone* getCurrentZone() {
        return currentZone;
    }

    void mapUsedObjects(std::set<GameObject*>& objects) {
        for (GameLevelZone* zone : zones) {
            zone->mapUsedObjects(objects);
        }
    }

    ~GameLevel() {
        for (GameLevelZone* zone : zones) {
            delete zone;
        }
    }
};