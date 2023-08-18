#pragma once

#include <algorithm>

#include "game/objects/GameObject.h"
#include "game/objects/GameObjectCache.h"
#include "game/IGameLevelZone.h"
#include "game/entities/IEntity.h"
#include "game/entities/CollisionBox.h"
#include "controls/IControls.h"
#include "audio/AudioManager.h"
#include "audio/AudioCache.h"

class Mario: public IEntity {
    private:
    static const int WALK_SPEED = 24;
    static const int RUN_SPEED = 56;

    CollisionBox collisionSmall = CollisionBox(3, 0, 10, 16);

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
            gameObject.setLevelTile(MARIO_SKID_SMA4);
            gameObject.setFlippedX(!gameObject.isFlippedX());
            velX += xDir;
        } else if (velX == 0 || (numTicks / ((velX > 9 || velX < -9 || velX == 0) ? 4 : 6)) % 2 == 1) {
            if (velX > RUN_SPEED - 4 || velX < -RUN_SPEED + 4) {
                gameObject.setLevelTile(MARIO_RUN1_SMA4);
            } else {
                gameObject.setLevelTile(MARIO_STAND_SMA4);
            }
        } else {
            if (std::abs(velX) > RUN_SPEED - 4) {
                gameObject.setLevelTile(MARIO_RUN2_SMA4);
            } else {
                gameObject.setLevelTile(MARIO_WALK_SMA4);
            }
        }
    }

    void setXShift(int& xShift, int tileX, int tileY) {
        tileX *= 256;
        tileY *= 256;

        int newShift;
        if (velX > 0) {
            newShift = tileX - (x + 208);
        } else {
            newShift = (tileX + 256) - (x + 48);
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

        int pTileX = div(x + 48, 256);
        int pTileX2 = div(x + 208, 256);
        int pTileY = div(y, 256);
        int pTileY2 = div(y + 256, 256);

        if (region.getGridObject(pTileX, pTileY)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX, pTileY);
        }

        if (mod(x + 48, 256) > 96 && region.getGridObject(pTileX2, pTileY)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX2, pTileY);
        }

        if (mod(y, 256) != 0 && region.getGridObject(pTileX, pTileY2)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX, pTileY2);
        }

        if (mod(x + 48, 256) > 96 && mod(y, 256) != 0 && region.getGridObject(pTileX2, pTileY2)->isFlag(s)) {
            stuck = true;
            setXShift(xShift, pTileX2, pTileY2);
        }

        if (stuck) {
            x += xShift;
            velX = 0;
        }

        // Y Movement
        stuck = false;
        y += velY;

        pTileX = div(x + 48, 256);
        pTileX2 = div(x + 208, 256);
        pTileY = div(y, 256);
        pTileY2 = div(y + 256, 256);

        if (region.getGridObject(pTileX, pTileY)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX, pTileY);
        }

        if (mod(x + 48, 256) > 96 && region.getGridObject(pTileX2, pTileY)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX2, pTileY);
        }

        if (mod(y, 256) != 0 && region.getGridObject(pTileX, pTileY2)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX, pTileY2);
            region.getGridObject(pTileX, pTileY2)->onHitUnder(pTileX, pTileY2, region);
        }

        if (pTileX != pTileX2 && mod(x + 48, 256) > 96 && mod(y, 256) != 0 && region.getGridObject(pTileX2, pTileY2)->isFlag(s)) {
            stuck = true;
            setYShift(yShift, pTileX2, pTileY2);
            region.getGridObject(pTileX2, pTileY2)->onHitUnder(pTileX2, pTileY2, region);
        }

        // make gaps harder to fall in if moving
        if (grounded && mod(y, 256) != 0 && velY < 0 && std::abs(velX) > WALK_SPEED) {
            pTileX = div(x, 256);
            pTileX2 = div(x, 256) + 1;

            if (region.getGridObject(pTileX, pTileY)->isFlag(s)) {
                stuck = true;
                setYShift(yShift, pTileX, pTileY);
            }

            if (region.getGridObject(pTileX2, pTileY)->isFlag(s)) {
                stuck = true;
                setYShift(yShift, pTileX2, pTileY);
            }
        }

        if (stuck) {
            y += yShift;
            velY = 0;
        }

        grounded = (yShift > 0);
    }

    int getXSub() {
        return x;
    }

    void setXSub(int value) {
        x = value;
    }

    int getYSub() {
        return y;
    }

    void setYSub(int value) {
        y = value;
    }

    public:
    // Mario gets own copy of player object to manipulate
    // Mario itself is also copied with the level when the level is played
    Mario(): gameObject(*GameObjectCache::objects["player"]) {}

    void resetGameObject() {
        gameObject = *GameObjectCache::objects["player"];
        numTicks = 0;
    }

    virtual GameObject& getGameObject() override {
        return gameObject;
    }

    virtual int getX() override {
        return x / 16;
    }

    virtual void setX(int value) override {
        x = value * 16;
    }

    virtual int getY() override {
        return y / 16;
    }

    virtual void setY(int value) override {
        y = value * 16;
    }

    virtual int getZoneLayer() override {
        return zoneLayer;
    }

    virtual void setZoneLayer(int value) override {
        zoneLayer = value;
    }

    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {

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
            gameObject.setLevelTile(MARIO_JUMP_SMA4);
        }
    }

    virtual CollisionBox& getCollisionBox() override {
        return collisionSmall;
    }
    virtual void onCollideMario(Mario& mario) override {
        // future multiplayer support?
    }
    virtual void onCollideEntity(IEntity& entity) override {}
};