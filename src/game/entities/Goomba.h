#pragma once

#include "game/entities/SubPosEntity.h"
#include "game/entities/IMario.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "game/IGameLevelZone.h"
#include "audio/AudioManager.h"

class Goomba: public SubPosEntity {
    private:
    int x;
    int y;
    int zoneLayer;
    GameObject* gameObject;
    GameObject* squashedObject;
    int velX = -8;
    int velY = 0;
    bool isDone = false;

    CollisionBox collision = CollisionBox(0, 0, 16, 16);

    public:
    Goomba(int x, int y, int zoneLayer, GameObject* object, GameObject* squashed): SubPosEntity(x, y, zoneLayer), gameObject(object), squashedObject(squashed) {}

    virtual int getLayerPriority() const override {
        return IEntity::ENEMY;
    }
    virtual GameObject& getGameObject() override {
        return *gameObject;
    }
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        
    }
    virtual bool shouldDelete() {
        return isDone;
    }

    virtual CollisionBox& getCollisionBox() = 0;
};