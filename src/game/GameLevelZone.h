#pragma once

#include "game/GameLevelRegion.h"
#include "game/entities/Mario.h"
#include "game/IGameLevelZone.h"
#include "game/backgrounds/GameBackgroundCache.h"
#include "TileMappings.h"


// An actual sub area in a level, contains multiple regions for the layers, also zone specific info like its own player object
class GameLevelZone: public IGameLevelZone {
    private:
    GameBackground* background = GameBackgroundCache::backgrounds["sma4:overworld_1"];

    Mario mario;
    IGameLevelRegion* regions[GameObject::NUM_LAYERS];

    public:
    GameLevelZone(int width, int height) {
        regions[0] = new GameLevelRegion(width, height, 0);
        regions[1] = new GameLevelRegion(width, height, 1);
        regions[2] = new GameLevelRegion(width, height, 2);
    }

    GameLevelZone(GameLevelZone& orig): mario(orig.getMario()) {
        std::printf("New GameLevelZone clone\n");
        IGameLevelRegion** origRegions = orig.getRegions();
        regions[0] = new GameLevelRegion(*origRegions[0]);
        regions[1] = new GameLevelRegion(*origRegions[1]);
        regions[2] = new GameLevelRegion(*origRegions[2]);
    }

    GameObject* getUpperGridObject(int x, int y, int& layer) {
        GameObject* air = GameObjectCache::objects["air"];
        for (int i = GameObject::NUM_LAYERS - 1; i >= 0; i--) {
            GameObject* object = regions[i]->getGridObject(x, y);
            if (object != air) {
                layer = i;
                return object;
            }
        }
        layer = -1;
        return air;
    }

    GameBackground* getBackground() {
        return background;
    }

    virtual IGameLevelRegion** getRegions() override {
        return regions;
    }

    virtual Mario& getMario() override {
        return mario;
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

        for (IGameLevelRegion* region : regions) {
            region->resizeGrid(newWidth, newHeight, xOff, yOff);
        }

        mario.setX(mario.getX() + xOff * 16);
        mario.setY(mario.getY() + yOff * 16);

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
        for (IGameLevelRegion* region : regions) {
            region->mapUsedObjects(objects);
        }
    }
    
    ~GameLevelZone() {
        delete regions[0];
        delete regions[1];
        delete regions[2];
    }
};