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
    unsigned int numTicks = 0;

    public:
    // Mario gets own copy of player object to manipulate
    // Mario itself is also copied with the level when the level is played
    Mario(): gameObject(*GameObjectCache::objects["player"]) {}

    void resetGameObject() {
        gameObject = *GameObjectCache::objects["player"];
        numTicks = 0;
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
        numTicks++;
        GameLevelRegion** regions = zone.getRegions();
        GameLevelRegion* collideRegion = regions[zoneLayer];

        if (grounded) {
            groundMovement(collideRegion, numTicks, controls);
            if (controls.jump()) {
                velY = 60 + (velX < 0 ? -velX : velX) / 4;
                grounded = false;
                if (velY > 112) {
                    velY = 112;
                }
                gameObject.setLevelTile(MARIO_JUMP_SMB3);
            }
        } else {
            airMovement(collideRegion, numTicks, controls);
        }

        x += velX;
        y += velY;

        // Do stuff like collision and checking for new state of player here
        if (y < 32 * 16) {
            y = 32 * 16;
            grounded = true;
        }
    }

    void airMovement(GameLevelRegion* region, unsigned int numTicks, IControls& controls) {
        if (velY > 0 && controls.jump()) {
            velY -= 2;
        } else {
            velY -= 6;
        }
        if (velY < -56) {
            velY = -56;
        }

        int xDir = 0;
        if (controls.left()) {
            xDir--;
        }
        if (controls.right()) {
            xDir++;
        }

        if (xDir * velX < 0) {
            velX += xDir * (controls.action() ? 2 : 1);
        }
    }

    void groundMovement(GameLevelRegion* region, unsigned int numTicks, IControls& controls) {
        int xDir = 0;
        if (controls.left()) {
            xDir--;
        }
        if (controls.right()) {
            xDir++;
        }
        velX += xDir;

        if (xDir == 0) {
            if (velX < 0 && (velX > -24 || numTicks % 2)) {
                velX++;
            } else if (velX > 0 && (velX < 24 || numTicks % 2)) {
                velX--;
            }
        }

        int maxSpeed = (controls.action() && xDir * velX > 0) ? 56 : 24;

        if (velX < -maxSpeed) {
            velX += 3;
        } else if (velX > maxSpeed) {
            velX -= 3;
        }

        if (velX < 0) {
            gameObject.setFlippedX(false);
        } else if (velX > 0) {
            gameObject.setFlippedX(true);
        }

        if (xDir * velX < 0) {
            gameObject.setLevelTile(MARIO_SKID_SMB3);
            gameObject.setFlippedX(!gameObject.isFlippedX());
            velX += xDir;
        } else if (velX == 0 || (numTicks / ((velX > 9 || velX < -9 || velX == 0) ? 4 : 6)) % 2 == 1) {
            gameObject.setLevelTile(MARIO_STAND_SMB3);
        } else {
            gameObject.setLevelTile(MARIO_WALK_SMB3);
        }
    }
};