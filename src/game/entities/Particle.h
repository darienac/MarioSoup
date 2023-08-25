#pragma once

#include <string>

#include "game/entities/MovingEntity.h"
#include "game/entities/CollisionBox.h"
#include "game/objects/GameObject.h"
#include "audio/AudioManager.h"
#include "TileMappings.h"

class Particle: public MovingEntity {
    private:
    int numTicks = 0;
    GameObject* object;
    std::string sound;

    CollisionBox box = CollisionBox(0, 0, 0, 0);

    public:
    Particle(int x, int y, int velX, int velY, int zoneLayer, GameObject* object, std::string sound): MovingEntity(x, y, velX, velY, zoneLayer), object(object), sound(sound) {}
    Particle(int x, int y, int velX, int velY, int zoneLayer, GameObject* object): Particle(x, y, velX, velY, zoneLayer, object, "") {}

    virtual int getLayerPriority() const override {
        return IEntity::PARTICLE;
    }
    virtual GameObject& getGameObject() override {
        return *object;
    }
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) override {
        if (numTicks == 0 && sound.size() != 0) {
            audio.playSound(AudioCache::audio[sound], getX(), getY());
        }
        setVelY(getVelY() - 6);
        // if (velY < -56) {
        //     velY = -56;
        // }

        moveX();
        moveY();

        numTicks++;
    }
    virtual bool shouldDelete() {
        return (getX() < -8) || (getY() < -8);
    }

    virtual CollisionBox& getCollisionBox() override {
        return box;
    }
};