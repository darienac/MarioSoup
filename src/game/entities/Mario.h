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
    public:
    enum PowerupState {
        SMALL,
        SUPER
    };
    
    private:
    static const int WALK_SPEED = 24;
    static const int RUN_SPEED = 56;

    CollisionBox collisionSmall = CollisionBox(3, 0, 10, 16);
    CollisionBox collisionStride = CollisionBox(-1, 0, 18, 1);

    GameObject gameObject;
    int x = 0;
    int y = 0;
    int zoneLayer = 1;

    bool grounded = true;
    int velX = 0;
    int velY = 0;
    unsigned int numTicks = 0;
    bool isJumpHeld = false;
    PowerupState powerupState = SUPER;

    int getRunTile(unsigned int numTicks) {
        static const int MARIO_WALK[] = {MARIO_STAND_SMA4, MARIO_WALK_SMA4};
        static const int MARIO_RUN[] = {MARIO_RUN1_SMA4, MARIO_RUN2_SMA4};
        static const int SMARIO_WALK[] = {SMARIO_STAND_SMA4, SMARIO_WALK1_SMA4, SMARIO_WALK2_SMA4, SMARIO_WALK1_SMA4};
        static const int SMARIO_RUN[] = {SMARIO_RUN_SMA4, SMARIO_RUN1_SMA4, SMARIO_RUN2_SMA4, SMARIO_RUN1_SMA4};

        int numWalkStages = (powerupState == SMALL) ? 2 : 4;
        int numRunStages = (powerupState == SMALL) ? 2 : 4;
        int frameTime = std::abs(velX) > 9 ? 4 : 6;
        if (std::abs(velX) > RUN_SPEED - 4) {
            int runStage = (numTicks / frameTime) % numRunStages;
            if (powerupState == SMALL) {
                return MARIO_RUN[runStage];
            } else {
                return SMARIO_RUN[runStage];
            }
        } else {
            int walkStage = (velX == 0) ? 0 : ((numTicks / frameTime) % numWalkStages);
            if (powerupState == SMALL) {
                return MARIO_WALK[walkStage];
            } else {
                return SMARIO_WALK[walkStage];
            }
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
            gameObject.setLevelTile(MARIO_SKID_SMA4);
            gameObject.setFlippedX(!gameObject.isFlippedX());
            velX += xDir;
        } else {
            gameObject.setLevelTile(getRunTile(numTicks));
        }
    }

    void setXShift(int& xShift, int tileX, int tileY) {
        tileX *= 16;
        tileY *= 16;

        int newShift;
        if (velX > 0) {
            newShift = tileX - (getX() + 13);
        } else {
            newShift = (tileX + 16) - (getX() + 3);
        }

        if (std::abs(newShift) > std::abs(xShift)) {
            xShift = newShift;
        }
    }

    void setYShift(int& yShift, int tileX, int tileY) {
        tileX *= 16;
        tileY *= 16;

        int newShift;
        if (velY > 0) {
            newShift = tileY - (getY() + 16);
        } else {
            newShift = (tileY + 16) - getY();
        }

        if (std::abs(newShift) > std::abs(yShift)) {
            yShift = newShift;
        }
    }

    void collisions(IGameLevelRegion& region, IControls& controls) {
        // X Movement
        x += velX;
        
        int bX = getX();
        int bY = getY();
        if (getCollisionBox().collideWithBlocksEntitiesX(bX, bY, velX, &region, this)) {
            setX(bX);
            velX = 0;
        }

        // Y Movement
        y += velY;

        bX = getX();
        bY = getY();

        CollisionBox* box;
        if (grounded && mod(y, 256) != 0 && velY < 0 && std::abs(velX) > WALK_SPEED) {
            box = &collisionStride;
            std::printf("running\n");
        } else {
            box = &getCollisionBox();
        }

        if (box->collideWithBlocksEntitiesY(bX, bY, velY, &region, this)) {
            setY(bY);
            grounded = velY < 0;
            if (velY > 0) {
                int tileX0 = div(getX() + getCollisionBox().getXOff(), 16);
                int tileX1 = div(getX() + getCollisionBox().getXOff() + getCollisionBox().getWidth(), 16);
                int tileY = div(getY() + getCollisionBox().getYOff() + getCollisionBox().getHeight(), 16);
                if (region.getGridObject(tileX0, tileY)->isFlag(GameObject::SOLID)) {
                    region.getGridObject(tileX0, tileY)->onHitUnder(tileX0, tileY, region);
                }
                if (tileX0 != tileX1 && region.getGridObject(tileX1, tileY)->isFlag(GameObject::SOLID)) {
                    region.getGridObject(tileX1, tileY)->onHitUnder(tileX1, tileY, region);
                }
            }
            velY = 0;
        } else {
            grounded = false;
        }

        if (x < 0) {
            x = 0;
            velX = 0;
        } else if (x > region.getWidth() * 256 - 256) {
            x = region.getWidth() * 256 - 256;
            velX = 0;
        }
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
        return div(x, 16);
    }

    virtual void setX(int value) override {
        x = value * 16;
    }

    virtual int getY() override {
        return div(y, 16);
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

    virtual int getLayerPriority() const override {
        return IEntity::MARIO;
    }

    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        numTicks++;
        IGameLevelRegion** regions = zone.getRegions();
        IGameLevelRegion* collideRegion = regions[zoneLayer];

        if (grounded) {
            groundMovement(numTicks, controls);
            if (controls.jump() && !isJumpHeld) {
                velY = 64 + (velX < 0 ? -velX : velX) / 4;
                grounded = false;
                if (velY > 112) {
                    velY = 112;
                }
                audio.playSound(*AudioCache::audio["smas:jump"], getX(), getY());
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

    virtual void onCollideMario(Mario& mario) override {}
    virtual void onCollideEntity(IEntity& entity) override {}

    virtual bool isPushable() {
        return true;
    }
};