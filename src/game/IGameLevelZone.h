#pragma once

#include "game/entities/IMario.h"

class IGameLevelRegion;

class IGameLevelZone {
    public:
    virtual IGameLevelRegion** getRegions() = 0;
    virtual IMario& getMario() = 0;

    virtual ~IGameLevelZone() {}
};