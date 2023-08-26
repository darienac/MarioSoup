#pragma once

#include <algorithm>

#include "game/objects/GameObject.h"
#include "game/objects/GameObjectCache.h"
#include "game/IGameLevelZone.h"
#include "game/entities/IMario.h"
#include "game/entities/CollisionBox.h"
#include "controls/IControls.h"
#include "audio/AudioManager.h"
#include "audio/AudioCache.h"

class Mario: public MovingEntity, public IMario {
    private:
    static const int WALK_SPEED = 24;
    static const int RUN_SPEED = 56;

    CollisionBox collisionSmall = CollisionBox(3, 0, 10, 16);
    CollisionBox collisionSuper = CollisionBox(3, 0, 10, 28);
    CollisionBox collisionStride = CollisionBox(-1, 0, 18, 1);

    GameObject gameObject;

    bool grounded = true;
    bool crouching = false;
    bool skidding = false;
    unsigned int numTicks = 0;
    bool isJumpHeld = false;
    int powerupStart;
    PowerupState powerupState = SMALL;
    PowerupState powerupStatePrev;
    PlayState playState = PLAY;
    int coins = 0;

    void playTick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) {
        IGameLevelRegion** regions = zone.getRegions();
        IGameLevelRegion* collideRegion = regions[getZoneLayer()];

        if (grounded) {
            groundMovement(*collideRegion, numTicks, controls);
            int velX = getVelX();
            int velY = getVelY();
            if (controls.jump() && !isJumpHeld) {
                velY = 64 + (velX < 0 ? -velX : velX) / 4;
                grounded = false;
                if (velY > 112) {
                    velY = 112;
                }
                audio.playSound(AudioCache::audio["smas:jump"], getX(), getY());
            } else {
                velY = -6;
            }
            setVelX(velX);
            setVelY(velY);
        } else {
            airMovement(numTicks, controls);
        }
        isJumpHeld = controls.jump();

        // Do stuff like collision and checking for new state of player here
        collisions(*collideRegion, controls, audio);
        if (!grounded) {
            if (powerupState == SMALL) {
                gameObject.setLevelTile(MARIO_JUMP_SMA4);
            } else if (!crouching) {
                gameObject.setLevelTile(SMARIO_JUMP_SMA4);
            }
            
        }
    }

    void powerupTick(IGameLevelZone& zone, int numTicks, AudioManager& audio) {
        if (powerupStart == -1) {
            powerupStart = numTicks;
            if (powerupState == SMALL) {
                audio.playSound(AudioCache::audio["smas:PowerDown"], getX(), getY());
            } else {
                audio.playSound(AudioCache::audio["smas:Powerup"], getX(), getY());
            }
        }
        int ticks = numTicks - powerupStart;
        if ((powerupState == SUPER && powerupStatePrev == SMALL) || powerupState == SMALL) {
            int tileStart = powerupState == SUPER ? SMARIO_STAND_SMA4 : MARIO_STAND_SMA4;
            int tileEnd = powerupStatePrev == SUPER ? MARIO_STAND_SMA4 : SMARIO_STAND_SMA4;
            if (ticks >= 46) {
                playState = PLAY;
                int bX = getX();
                int bY = getY();
                if (collisionSuper.collideWithBlocksEntitiesX(bX, bY, getVelX(), zone.getRegions()[getZoneLayer()], this, nullptr)) {
                    crouching = true;
                    gameObject.setLevelTile(SMARIO_SQUAT_SMA4);
                }
                return;
            }
            if (ticks < 28) {
                gameObject.setLevelTile((ticks / 4) % 2 == 0 ? SMARIO_GROW_SMA4 : tileStart);
            } else {
                gameObject.setLevelTile((ticks / 4) % 2 == 0 ? tileEnd : SMARIO_GROW_SMA4);
            }
        } else {
            playState = PLAY;
        }
    }

    int getRunTile(unsigned int numTicks) {
        static const int MARIO_WALK[] = {MARIO_STAND_SMA4, MARIO_WALK_SMA4};
        static const int MARIO_RUN[] = {MARIO_RUN1_SMA4, MARIO_RUN2_SMA4};
        static const int SMARIO_WALK[] = {SMARIO_STAND_SMA4, SMARIO_WALK1_SMA4, SMARIO_WALK2_SMA4, SMARIO_WALK1_SMA4};
        static const int SMARIO_RUN[] = {SMARIO_RUN_SMA4, SMARIO_RUN1_SMA4, SMARIO_RUN2_SMA4, SMARIO_RUN1_SMA4};

        int numWalkStages = (powerupState == SMALL) ? 2 : 4;
        int numRunStages = (powerupState == SMALL) ? 2 : 4;
        int frameTime = std::abs(getVelX()) > 9 ? 4 : 6;
        if (std::abs(getVelX()) > RUN_SPEED - 4) {
            int runStage = (numTicks / frameTime) % numRunStages;
            if (powerupState == SMALL) {
                return MARIO_RUN[runStage];
            } else {
                return SMARIO_RUN[runStage];
            }
        } else {
            int walkStage = (getVelX() == 0) ? 0 : ((numTicks / frameTime) % numWalkStages);
            if (powerupState == SMALL) {
                return MARIO_WALK[walkStage];
            } else {
                return SMARIO_WALK[walkStage];
            }
        }
    }

    void airMovement(unsigned int numTicks, IControls& controls) {
        int velX = getVelX();
        int velY = getVelY();
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
        setVelX(velX);
        setVelY(velY);
    }

    void groundMovement(IGameLevelRegion& region, unsigned int numTicks, IControls& controls) {
        int velX = getVelX();
        int velY = getVelY();
        int xDir = 0;
        if (controls.left()) {
            xDir--;
        }
        if (controls.right()) {
            xDir++;
        }
        if (true) {
            velX += xDir;
            if (std::abs(velX) < 10) {
                velX += xDir;
            }
        }
        

        if (crouching || xDir == 0) {
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

        if (crouching && (xDir != 0 || !controls.down())) {
            int bX = getX();
            int bY = getY();
            if (!collisionSuper.collideWithBlocksEntitiesX(bX, bY, velX, &region, this, nullptr)) {
                crouching = false;
            }
        }

        skidding = !crouching && xDir * velX < 0;
        if (skidding) {
            if (powerupState == SMALL) {
                gameObject.setLevelTile(MARIO_SKID_SMA4);
            } else {
                gameObject.setLevelTile(SMARIO_SKID_SMA4);
            }
            gameObject.setFlippedX(!gameObject.isFlippedX());
            velX += xDir;
        } else if (powerupState != SMALL && xDir == 0 && controls.down()) {
            crouching = true;
            gameObject.setLevelTile(SMARIO_SQUAT_SMA4);
        } else if (!crouching) {
            gameObject.setLevelTile(getRunTile(numTicks));
        }
        setVelX(velX);
        setVelY(velY);
    }

    void collisions(IGameLevelRegion& region, IControls& controls, AudioManager& audio) {
        CollisionBox::collide_block_callback block_callback = [this, &region, &audio](int tileX, int tileY, GameObject* object) {
            object->onPlayerCollide(tileX, tileY, this, region, audio);
        };
        // X Movement
        moveX();
        if (regionXCollide(region, block_callback)) {
            setVelX(0);
        }

        // Y Movement
        moveY();

        CollisionBox* box;
        if (grounded && mod(getYSub(), 256) != 0 && getVelY() < 0 && std::abs(getVelX()) > WALK_SPEED) {
            box = &collisionStride;
        } else {
            box = &getCollisionBox();
        }

        if (regionYCollide(region, *box, block_callback)) {
            grounded = getVelY() < 0;
            if (getVelY() > 0) {
                int tileX0 = div(getX() + getCollisionBox().getXOff(), 16);
                int tileX1 = div(getX() + getCollisionBox().getXOff() + getCollisionBox().getWidth(), 16);
                int tileY = div(getY() + getCollisionBox().getYOff() + getCollisionBox().getHeight(), 16);
                if (region.getGridObject(tileX0, tileY)->isFlag(GameObject::SOLID)) {
                    region.getGridObject(tileX0, tileY)->onHitUnder(tileX0, tileY, this, region);
                }
                if (tileX0 != tileX1 && region.getGridObject(tileX1, tileY)->isFlag(GameObject::SOLID)) {
                    region.getGridObject(tileX1, tileY)->onHitUnder(tileX1, tileY, this, region);
                }
            }
            setVelY(0);
        } else {
            grounded = false;
        }

        if (getXSub() < 0) {
            setX(0);
            setVelX(0);
        } else if (getXSub() > region.getWidth() * 256 - 256) {
            setXSub(region.getWidth() * 256 - 256);
            setVelX(0);
        }
    }

    public:
    // Mario gets own copy of player object to manipulate
    // Mario itself is also copied with the level when the level is played
    Mario(): MovingEntity(0, 0, 1), gameObject(*GameObjectCache::objects["player"]) {}

    virtual bool isSkidding() override {
        return skidding;
    }

    virtual PowerupState getPowerupState() override {
        return powerupState;
    }

    virtual PlayState getPlayState() override {
        return playState;
    }

    virtual void triggerPowerupState(PowerupState state) override {
        powerupStatePrev = powerupState;
        powerupState = state;
        playState = POWERUP;
        powerupStart = -1;
    }

    virtual void damage() override {
        powerupStatePrev = powerupState;
        if (powerupState == SMALL) {
            // time to die
        } else {
            powerupState = SMALL;
            playState = POWERUP;
            powerupStart = -1;
        }
    }

    virtual int getNumCoins() override {
        return coins;
    }

    virtual void setNumCoins(int value) override {
        coins = value;
        std::printf("Coins: %d\n", value);
    }

    virtual int getVelX() override {
        return MovingEntity::getVelX();
    }

    virtual int getVelY() override {
        return MovingEntity::getVelY();
    }

    virtual void setVelX(int value) override {
        MovingEntity::setVelX(value);
    }

    virtual void setVelY(int value) override {
        MovingEntity::setVelY(value);
    }

    void resetGameObject() {
        gameObject = *GameObjectCache::objects["player"];
        numTicks = 0;
    }

    virtual GameObject& getGameObject() override {
        return gameObject;
    }

    virtual int getLayerPriority() const override {
        return IEntity::MARIO;
    }

    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        numTicks++;
        skidding = false;
        switch (playState) {
            case POWERUP:
                powerupTick(zone, numTicks, audio);
                if (playState == POWERUP) {
                    break;
                }
            case PLAY:
                playTick(zone, audio, controls);
                break;
        }
        
    }

    virtual CollisionBox& getCollisionBox() override {
        if (crouching || powerupState == SMALL) {
            return collisionSmall;
        } else {
            return collisionSuper;
        }   
    }

    virtual void onCollideMario(IMario& mario) override {}
    virtual void onCollideEntity(IEntity& entity) override {}

    virtual bool isPushable() {
        return true;
    }
};