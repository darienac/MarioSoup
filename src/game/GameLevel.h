#pragma once

#include <set>
#include <vector>

#include "game/objects/GameObject.h"
#include "game/objects/GameObjectCache.h"
#include "game/GameLevelZone.h"

class GameLevel {
    private:
    std::vector<GameLevelZone*> zones;
    GameLevelZone* currentZone = nullptr;

    public:
    GameLevel() {}

    GameLevel(GameLevel& orig) {
        std::printf("New GameLevel clone\n");
        for (GameLevelZone* zone : *orig.getZones()) {
            GameLevelZone* newZone = new GameLevelZone(*zone);
            zones.push_back(newZone);
            if (zone == orig.currentZone) {
                currentZone = newZone;
            }
        }
    }

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