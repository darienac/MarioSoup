#pragma once

#include "game/GameLevelRegion.h"
#include "game/Mario.h"
#include "game/IGameLevelZone.h"


// An actual sub area in a level, contains multiple regions for the layers, also zone specific info like its own player object
class GameLevelZone: public IGameLevelZone {
    private:
    Mario mario;
    GameLevelRegion* regions[GameObject::NUM_LAYERS];

    public:
    GameLevelZone(int width, int height) {
        regions[0] = new GameLevelRegion(width, height);
        regions[1] = new GameLevelRegion(width, height);
        regions[2] = new GameLevelRegion(width, height);
    }

    GameLevelZone(GameLevelZone& orig): mario(orig.getMario()) {
        std::printf("New GameLevelZone clone\n");
        GameLevelRegion** origRegions = orig.getRegions();
        regions[0] = new GameLevelRegion(*origRegions[0]);
        regions[1] = new GameLevelRegion(*origRegions[1]);
        regions[2] = new GameLevelRegion(*origRegions[2]);
    }

    Mario& getMario() {
        return mario;
    }

    virtual GameLevelRegion** getRegions() override {
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

        boundMario();
    }

    void boundMario() {
        int x = mario.getX();
        int y = mario.getY();
        int w = getWidth() * 16;
        int h = getHeight() * 16;

        if (x < -32) {
            x = -32;
        } else if (x > w + 16) {
            x = w + 16;
        }
        if (y < -32) {
            y = -32;
        } else if (y > h + 16) {
            y = h + 16;
        }

        mario.setX(x);
        mario.setY(y);
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