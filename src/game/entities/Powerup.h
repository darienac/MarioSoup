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
    int velX = 8;
    int velY = 0;

    CollisionBox collision = CollisionBox(0, 0, 16, 16);

    void movement(IGameLevelZone& zone) {
        IGameLevelRegion* region = zone.getRegions()[zoneLayer];
        velY -= 6;
        if (velY < -56) {
            velY = -56;
        }

        x += velX;
        int bX = getX();
        int bY = getY();
        if (collision.collideWithBlocksEntitiesX(bX, bY, velX, region, this)) {
            setX(bX);
            velX = -velX;
        }

        y += velY;
        bX = getX();
        bY = getY();
        if (collision.collideWithBlocksEntitiesY(bX, bY, velY, region, this)) {
            setY(bY);
            velY = 0;
        }
    }

    public:
    static GameObject* air;

    Powerup(int x, int y, int zoneLayer, GameObject* object, bool emerge): x(x * 16), y(y * 16), zoneLayer(zoneLayer), gameObject(object), emerge(emerge) {}

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
                setY(getY() + 1);
            }
        } else {
            movement(zone);
        }
        ticks++;
    }

    virtual bool shouldDelete() {
        return false;
    }

    virtual CollisionBox& getCollisionBox() override {
        return collision;
    }

    virtual void onPushed(IEntity& entity, int dx, int dy) {
        if (dy > 0) {
            velY = 64;
        }
    }

    virtual bool isPushable() {
        return !emerge;
    }
};