#pragma once

#include <string>

#include "game/entities/IEntity.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "audio/AudioManager.h"
#include "TileMappings.h"

class Particle: public IEntity {
    private:
    int x;
    int y;
    int velX;
    int velY;
    int zoneLayer;
    int numTicks = 0;
    GameObject object;
    std::string sound;

    CollisionBox box = CollisionBox(0, 0, 0, 0);

    public:
    Particle(int x, int y, int velX, int velY, int zoneLayer, GameObject* object, std::string sound): x(x*16), y(y*16), velX(velX), velY(velY), zoneLayer(zoneLayer), object(*object), sound(sound) {}
    Particle(int x, int y, int velX, int velY, int zoneLayer, GameObject* object): Particle(x, y, velX, velY, zoneLayer, object, "") {}

    virtual int getZoneLayer() override {
        return zoneLayer;
    }
    virtual void setZoneLayer(int value) override {
        zoneLayer = value;
    }
    virtual int getLayerPriority() const override {
        return IEntity::PARTICLE;
    }
    virtual int getX() override {
        return x / 16;
    }
    virtual void setX(int value) override {
        x = value * 16;
    }
    virtual int getY() override {
        return y / 16;
    }
    virtual void setY(int value) override {
        y = value * 16;
    }
    virtual GameObject& getGameObject() override {
        return object;
    }
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        if (numTicks == 0 && sound.size() != 0) {
            audio.playSound(AudioCache::audio[sound], getX(), getY());
        }
        velY -= 6;
        // if (velY < -56) {
        //     velY = -56;
        // }

        x += velX;
        y += velY;

        int tileState = (numTicks / 4) % 4;
        if (object.getLevelTile() == Tiles::SMA4_BRICK_BREAK) {
            switch (tileState) {
                case 0:
                    object.setFlippedX(false);
                    object.setFlippedY(false);
                    break;
                case 1:
                    object.setFlippedX(true);
                    object.setFlippedY(false);
                    break;
                case 2:
                    object.setFlippedX(false);
                    object.setFlippedY(true);
                    break;
                case 3:
                    object.setFlippedX(true);
                    object.setFlippedY(true);
                    break;
            }
        }
        numTicks++;
    }
    virtual bool shouldDelete() {
        return (getX() < -8) || (getY() < -8);
    }

    virtual CollisionBox& getCollisionBox() override {
        return box;
    }
};