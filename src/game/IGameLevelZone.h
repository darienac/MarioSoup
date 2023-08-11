#pragma once

#include "game/GameLevelRegion.h"

class IGameLevelZone {
    public:
    virtual GameLevelRegion** getRegions() = 0;

    virtual ~IGameLevelZone() {}
};