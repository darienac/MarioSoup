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

    virtual void triggerPowerupState(PowerupState state) = 0;
};