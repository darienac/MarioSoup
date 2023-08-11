#pragma once

#include "game/GameObject.h"
#include "game/GameObjectCache.h"
#include "game/IGameLevelZone.h"
#include "controls/IControls.h"

class Mario {
    private:
    GameObject gameObject;
    int x = 0;
    int y = 0;
    int zoneLayer = 1;

    bool grounded = true;
    int velX = 0;
    int velY = 0;

    public:
    // Mario gets own copy of player object to manipulate
    // Mario itself is also copied with the level when the level is played
    Mario(): gameObject(*GameObjectCache::objects["player"]) {}

    void resetGameObject() {
        gameObject = *GameObjectCache::objects["player"];
    }

    GameObject& getGameObject() {
        return gameObject;
    }

    int getX() {
        return x / 16;
    }

    void setX(int value) {
        x = value * 16;
    }

    int getXSub() {
        return x;
    }

    void setXSub(int value) {
        x = value;
    }

    int getY() {
        return y / 16;
    }

    void setY(int value) {
        y = value * 16;
    }

    int getYSub() {
        return y;
    }

    void setYSub(int value) {
        y = value;
    }

    int getZoneLayer() {
        return zoneLayer;
    }

    void setZoneLayer(int value) {
        zoneLayer = value;
    }

    void tick(IGameLevelZone& zone, IControls& controls) {
        GameLevelRegion** regions = zone.getRegions();
        GameLevelRegion* collideRegion = regions[zoneLayer];

        if (grounded) {
            groundMovement(collideRegion, controls);
        }
    }

    void groundMovement(GameLevelRegion* region, IControls& controls) {
        if (controls.left()) {
            velX -= 2;
        }
        if (controls.right()) {
            velX += 2;
        }
        if (!controls.left() && !controls.right()) {
            if (velX < 0) {
                velX++;
            } else if (velX > 0) {
                velX--;
            }
        }

        if (velX < -20) {
            velX = -20;
        } else if (velX > 20) {
            velX = 20;
        }

        x += velX;
    }
};