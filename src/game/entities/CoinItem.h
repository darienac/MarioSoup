#pragma once

#include "game/entities/IEntity.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "game/IGameLevelZone.h"
#include "audio/AudioManager.h"
#include "TileMappings.h"

class CoinItem: public IEntity {
    private:
    int x;
    int y;
    int yStart;
    int velY = 96;
    int zoneLayer;
    GameObject* object;
    int numTicks = 0;
    bool isDone = false;

    CollisionBox box = CollisionBox(0, 0, 0, 0);

    public:
    CoinItem(int x, int y, int zoneLayer, GameObject* object): x(x * 16), y(y * 16), zoneLayer(zoneLayer), object(object) {}

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
        return *object;
    }
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        if (numTicks == 0) {
            audio.playSound(AudioCache::audio["smas:Coin"], getX(), getY());
            yStart = y;
            zone.getMario().setNumCoins(zone.getMario().getNumCoins() + 1);
        }
        y += velY;
        velY -= 6;
        if (y == yStart) {
            isDone = true;
        }

        numTicks++;
    }
    virtual bool shouldDelete() {
        return isDone;
    }

    virtual CollisionBox& getCollisionBox() override {
        return box;
    }
};