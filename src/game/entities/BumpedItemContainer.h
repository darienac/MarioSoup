#pragma once

#include "game/entities/IEntity.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "game/IGameLevelZone.h"
#include "audio/AudioManager.h"

class BumpedItemContainer: public IEntity {
    private:
    int x;
    int y;
    int zoneLayer;
    int ticks = 0;
    GameObject* gameObject;
    int velY = 4;
    bool isDone = false;

    CollisionBox collision = CollisionBox(0, 0, 16, 16);

    public:
    BumpedItemContainer(int tileX, int tileY, int zoneLayer, GameObject* object): x(tileX * 16), y(tileY * 16), zoneLayer(zoneLayer), gameObject(object) {}

    virtual int getZoneLayer() override {
        return zoneLayer;
    }
    virtual void setZoneLayer(int value) override {
        zoneLayer = value;
    }
    virtual int getLayerPriority() const override {
        return IEntity::ITEM_CONTAINER;
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
        return *gameObject;
    }
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        if (ticks == 0) {
            audio.playSound(AudioCache::audio["smas:Bump"], getX(), getY());
        }
        y += velY;
        velY--;

        if (velY == -5) {
            zone.getRegions()[zoneLayer]->setGridObject(gameObject, div(x, 16), div(y, 16));
            isDone = true;
        }
        ticks++;
    }

    virtual CollisionBox& getCollisionBox() override {
        return collision;
    }
    virtual void onCollideMario(IMario& mario) override {

    }
    virtual void onCollideEntity(IEntity& entity) override {

    }

    virtual bool shouldDelete() {
        return isDone;
    }

    virtual bool isSolid() {
        return true;
    }
};