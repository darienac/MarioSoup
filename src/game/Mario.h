#pragma once

#include <algorithm>

#include "game/GameObject.h"
#include "game/GameObjectCache.h"
#include "game/IGameLevelZone.h"
#include "controls/IControls.h"
#include "audio/AudioManager.h"
#include "audio/AudioCache.h"

class Mario {
    private:
    static const int WALK_SPEED = 24;
    static const int RUN_SPEED = 56;

    GameObject gameObject;
    int x = 0;
    int y = 0;
    int zoneLayer = 1;

    bool grounded = true;
    int velX = 0;
    int velY = 0;
    unsigned int numTicks = 0;
    bool isJumpHeld = false;

    int mod(int v0, int v1) {
        return (v0 + 512) % v1;
    }

    int div(int v0, int v1) {
        return (v0 - mod(v0, v1)) / v1;
    }

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

    void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) {

        numTicks++;
        GameLevelRegion** regions = zone.getRegions();
        GameLevelRegion* collideRegion = regions[zoneLayer];

        if (grounded) {
            groundMovement(numTicks, controls);
            if (controls.jump() && !isJumpHeld) {
                velY = 64 + (velX < 0 ? -velX : velX) / 4;
                grounded = false;
                if (velY > 112) {
                    velY = 112;
                }
                float pos[3] = {(float) getX(), (float) getY(), 0.0};
                audio.playSound(*AudioCache::audio["smb3:jump"], pos);
            } else {
                velY = -6;
            }
        } else {
            airMovement(numTicks, controls);
        }
        isJumpHeld = controls.jump();

        // Do stuff like collision and checking for new state of player here
        collisions(*collideRegion, controls);
        if (!grounded) {
            gameObject.setLevelTile(MARIO_JUMP_SMB3);
        }
    }

    void airMovement(unsigned int numTicks, IControls& controls) {
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

        if ((xDir < 0 && velX > -16) || (xDir > 0 && velX < 16)) {
            velX += xDir * (controls.action() ? 2 : 1) * (xDir * velX > 0 ? 2 : 1);
        }
    }

    void groundMovement(unsigned int numTicks, IControls& controls) {
        int xDir = 0;
        if (controls.left()) {
            xDir--;
        }
        if (controls.right()) {
            xDir++;
        }
        velX += xDir;
        if (std::abs(velX) < 10) {
            velX += xDir;
        }

        if (xDir == 0) {
            if (velX < 0 && (velX > -WALK_SPEED || numTicks % 2)) {
                velX++;
            } else if (velX > 0 && (velX < WALK_SPEED || numTicks % 2)) {
                velX--;
            }
        }

        int maxSpeed = (controls.action() && xDir * velX > 0) ? RUN_SPEED : WALK_SPEED;

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
            if (velX > RUN_SPEED - 4 || velX < -RUN_SPEED + 4) {
                gameObject.setLevelTile(MARIO_RUN1_SMB3);
            } else {
                gameObject.setLevelTile(MARIO_STAND_SMB3);
            }
        } else {
            if (std::abs(velX) > RUN_SPEED - 4) {
                gameObject.setLevelTile(MARIO_RUN2_SMB3);
            } else {
                gameObject.setLevelTile(MARIO_WALK_SMB3);
            }
        }
    }

    void setXShift(int& xShift, int tileX, int tileY) {
        tileX *= 256;
        tileY *= 256;

        int newShift;
        if (velX > 0) {
            newShift = tileX - (x + 256);
        } else {
            newShift = (tileX + 256) - x;
        }

        if (std::abs(newShift) > std::abs(xShift)) {
            xShift = newShift;
        }
    }

    void setYShift(int& yShift, int tileX, int tileY) {
        tileX *= 256;
        tileY *= 256;

        int newShift;
        if (velY > 0) {
            newShift = tileY - (y + 256);
        } else {
            newShift = (tileY + 256) - y;
        }

        if (std::abs(newShift) > std::abs(yShift)) {
            yShift = newShift;
        }
    }

    void collisions(GameLevelRegion& region, IControls& controls) {
        if (x < 0) {
            x = 0;
            velX = 0;
        } else if (x > region.getWidth() * 256 - 256) {
            x = region.getWidth() * 256 - 256;
            velX = 0;
        }

        GameObject::Flag s = GameObject::SOLID;
        bool stuck = false;

        // X Movement
        x += velX;
        int xShift = 0;
        int yShift = 0;

        int pTileX = div(x, 256);
        int pTileY = div(y, 256);

        if (region.getGridObject(pTileX, pTileY)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX, pTileY);
        }

        if (mod(x, 256) != 0 && region.getGridObject(pTileX + 1, pTileY)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX + 1, pTileY);
        }

        if (mod(y, 256) != 0 && region.getGridObject(pTileX, pTileY + 1)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX, pTileY + 1);
        }

        if (mod(x, 256) != 0 && mod(y, 256) != 0 && region.getGridObject(pTileX + 1, pTileY + 1)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX + 1, pTileY + 1);
        }

        if (stuck) {
            x += xShift;
            velX = 0;
        }

        // Y Movement
        stuck = false;
        y += velY;

        pTileX = div(x, 256);
        pTileY = div(y, 256);

        if (region.getGridObject(pTileX, pTileY)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX, pTileY);
        }

        if (mod(x, 256) != 0 && region.getGridObject(pTileX + 1, pTileY)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX + 1, pTileY);
        }

        if (mod(y, 256) != 0 && region.getGridObject(pTileX, pTileY + 1)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX, pTileY + 1);
        }

        if (mod(x, 256) != 0 && mod(y, 256) != 0 && region.getGridObject(pTileX + 1, pTileY + 1)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX + 1, pTileY + 1);
        }

        if (stuck) {
            y += yShift;
            velY = 0;
        }

        grounded = (yShift > 0);
    }
};