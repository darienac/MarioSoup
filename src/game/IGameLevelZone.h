#pragma once

class IGameLevelRegion;
class IEntity;

class IGameLevelZone {
    public:
    virtual IGameLevelRegion** getRegions() = 0;
    virtual IEntity& getMario() = 0;

    virtual ~IGameLevelZone() {}
};