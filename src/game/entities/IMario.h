#pragma once

#include "game/entities/IEntity.h"

class IMario: virtual public IEntity {
    public:
    enum PlayState {
        PLAY,
        POWERUP
    };
    enum PowerupState {
        SMALL,
        SUPER
    };

    virtual bool isSkidding() = 0;
    virtual PowerupState getPowerupState() = 0;
    virtual PlayState getPlayState() = 0;
    virtual void triggerPowerupState(PowerupState state) = 0;
    virtual int getNumCoins() = 0;
    virtual void setNumCoins(int value) = 0;

    virtual int getVelX() = 0;
    virtual int getVelY() = 0;
    virtual void setVelX(int value) = 0;
    virtual void setVelY(int value) = 0;
};