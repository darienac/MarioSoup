#pragma once

#include "game/entities/MovingEntity.h"
#include "game/entities/IMario.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "game/IGameLevelZone.h"
#include "audio/AudioManager.h"

class Powerup: public MovingEntity {
    private:
    GameObject* gameObject;
    bool emerge = false;
    int ticks = 0;
    bool isDone = false;

    CollisionBox collision = CollisionBox(0, 0, 16, 16);

    void movement(IGameLevelRegion& region) {
        setVelY(getVelY() - 6);
        if (getVelY() < -56) {
            setVelY(-56);
        }

        moveX();
        if (regionXCollide(region, nullptr)) {
            setVelX(-getVelX());
        }

        moveY();
        if (regionYCollide(region, nullptr)) {
            setVelY(0);
        }
    }

    public:
    static GameObject* air;

    Powerup(int x, int y, int zoneLayer, GameObject* object, bool emerge): MovingEntity(x, y, -8, 0, zoneLayer), gameObject(object), emerge(emerge) {}

    virtual int getLayerPriority() const override {
        return IEntity::ITEM;
    }
    virtual GameObject& getGameObject() override {
        if (emerge && ticks < 16) {
            return *IEntity::air;
        } else {
            return *gameObject;
        }
    }
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        if (emerge) {
            if (ticks == 64) {
                emerge = false;
            } else if (ticks >= 16 && ticks % 3 == 0) {
                setY(getY() + 1);
            } else if (ticks == 16) {
                audio.playSound(AudioCache::audio["smas:ItemSprout"], getX(), getY());
            }
        } else {
            movement(*zone.getRegions()[getZoneLayer()]);
        }
        ticks++;
    }

    virtual bool shouldDelete() {
        return isDone;
    }

    virtual CollisionBox& getCollisionBox() override {
        return collision;
    }

    virtual void onCollideMario(IMario& mario) override {
        if (emerge) {
            return;
        }
        mario.triggerPowerupState(IMario::SUPER);
        isDone = true;
    }

    virtual void onPushed(IEntity& entity, int dx, int dy) {
        if (dy > 0) {
            setVelY(64);
        }
    }

    virtual bool isPushable() {
        return !emerge;
    }
};