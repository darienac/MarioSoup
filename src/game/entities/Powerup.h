#pragma once

#include "game/entities/IEntity.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "game/IGameLevelZone.h"

class Powerup: public IEntity {
    private:
    int x;
    int y;
    int zoneLayer;
    GameObject* gameObject;
    bool emerge = false;
    int ticks = 0;

    CollisionBox collision = CollisionBox(0, 0, 16, 16);

    public:
    static GameObject* air;

    Powerup(int x, int y, int zoneLayer, GameObject* object, bool emerge): x(x), y(y), zoneLayer(zoneLayer), gameObject(object), emerge(emerge) {}

    virtual int getZoneLayer() override {
        return zoneLayer;
    }
    virtual void setZoneLayer(int value) override {
        zoneLayer = value;
    }
    virtual int getLayerPriority() const override {
        return IEntity::ITEM;
    }
    virtual int getX() override {
        return x;
    }
    virtual void setX(int value) override {
        x = value;
    }
    virtual int getY() override {
        return y;
    }
    virtual void setY(int value) override {
        y = value;
    }
    virtual GameObject& getGameObject() override {
        if (ticks < 16) {
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
                y++;
            }
        } else {
            // Start moving and stuff
        }
        ticks++;
    }
    virtual bool shouldDelete() {
        return false;
    }

    virtual CollisionBox& getCollisionBox() override {
        return collision;
    }
};