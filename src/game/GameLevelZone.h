#pragma once

#include "game/GameLevelRegion.h"
#include "game/Mario.h"


// An actual sub area in a level, contains multiple regions for the layers, also zone specific info like its own player object
class GameLevelZone {
    private:
    Mario mario;
    GameLevelRegion* regions[GameObject::NUM_LAYERS];

    public:
    GameLevelZone(int width, int height) {
        regions[0] = new GameLevelRegion(width, height);
        regions[1] = new GameLevelRegion(width, height);
        regions[2] = new GameLevelRegion(width, height);
    }

    Mario& getMario() {
        return mario;
    }

    GameLevelRegion** getRegions() {
        return regions;
    }

    int getWidth() {
        return regions[0]->getWidth();
    }

    int getHeight() {
        return regions[0]->getHeight();
    }

    void resizeGrid(int newWidth, int newHeight, int xOff, int yOff) {
        if (newWidth <= 0 || newHeight <= 0) {
            return;
        }

        for (GameLevelRegion* region : regions) {
            region->resizeGrid(newWidth, newHeight, xOff, yOff);
        }
    }

    void mapUsedObjects(std::set<GameObject*>& objects) {
        for (GameLevelRegion* region : regions) {
            region->mapUsedObjects(objects);
        }
    }
    
    ~GameLevelZone() {
        delete regions[0];
        delete regions[1];
        delete regions[2];
    }
};