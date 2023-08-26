#pragma once

#include "game/entities/MovingEntity.h"
#include "game/entities/IMario.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "game/IGameLevelZone.h"
#include "audio/AudioManager.h"

class Goomba: public MovingEntity {
    private:
    int zoneLayer;
    GameObject* gameObject;
    GameObject* squashedObject;
    bool isDone = false;
    bool squashed = false;
    int framesSquashed = 0;

    CollisionBox collision = CollisionBox(0, 0, 16, 16);

    public:
    Goomba(int x, int y, int zoneLayer, GameObject* object, GameObject* squashed): MovingEntity(x, y, -8, 0, zoneLayer), gameObject(object), squashedObject(squashed) {}

    virtual int getLayerPriority() const override {
        return IEntity::ENEMY;
    }
    virtual GameObject& getGameObject() override {
        if (squashed) {
            return *squashedObject;
        } else {
            return *gameObject;
        }
    }
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        setVelY(getVelY() - 6);
        if (getVelY() < -56) {
            setVelY(-56);
        }

        IGameLevelRegion* region = zone.getRegions()[getZoneLayer()];
        moveX();
        if (regionXCollide(*region, nullptr)) {
            setVelX(-getVelX());
        }

        moveY();
        if (regionYCollide(*region, nullptr)) {
            setVelY(0);
        }

        if (squashed) {
            if (framesSquashed == 0) {
                audio.playSound(AudioCache::audio["smas:Stomp"], getX(), getY());
            }
            framesSquashed++;
        }
    }
    virtual bool shouldDelete() {
        return framesSquashed > 16;
    }

    virtual CollisionBox& getCollisionBox() {
        return collision;
    }

    virtual void onCollideMario(IMario& mario) override {
        if (squashed) {
            return;
        }
        if (mario.getY() > (getY() + 8) && mario.getVelY() < 0) {
            squashed = true;
            mario.setVelY(64);
            setVelX(0);
        } else {
            mario.damage();
        }
    }
};