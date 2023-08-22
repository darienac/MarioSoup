#pragma once

#include "game/entities/IEntity.h"

class IMario: public IEntity {
    public:
    enum PlayState {
        PLAY,
        POWERUP
    };
    enum PowerupState {
        SMALL,
        SUPER
    };

    virtual PowerupState getPowerupState() = 0;
    virtual void triggerPowerupState(PowerupState state) = 0;
};