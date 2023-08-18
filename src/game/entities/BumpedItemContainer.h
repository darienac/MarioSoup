#pragma once

#include "game/entities/IEntity.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"

class BumpedItemContainer: public IEntity {
    private:
    int x;
    int y;
    int zoneLayer;
    GameObject* gameObject;
    int ticks = 0;

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
        // TODO: Implement this
    }

    virtual CollisionBox& getCollisionBox() override {
        return collision;
    }
    virtual void onCollideMario(Mario& mario) override {

    }
    virtual void onCollideEntity(IEntity& entity) override {

    }
};