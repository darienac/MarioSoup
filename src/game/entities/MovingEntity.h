#pragma once

#include "game/entities/SubPosEntity.h"
#include "game/entities/CollisionBox.h"
#include "game/IGameLevelZone.h"

class MovingEntity: public SubPosEntity {
    private:
    int velX;
    int velY;
    
    public:
    MovingEntity(int x, int y, int velX, int velY, int zoneLayer): SubPosEntity(x, y, zoneLayer), velX(velX), velY(velY) {}
    MovingEntity(int x, int y, int zoneLayer): MovingEntity(x, y, 0, 0, zoneLayer) {}

    virtual int getVelX() {
        return velX;
    }

    virtual void setVelX(int value) {
        velX = value;
    }

    virtual int getVelY() {
        return velY;
    }

    virtual void setVelY(int value) {
        velY = value;
    }

    virtual bool moveZoneX(IGameLevelZone& zone, bool collideBlocksEntities) {
        IGameLevelRegion* region = zone.getRegions()[getZoneLayer()];

        setXSub(getXSub() + velX);
        if (!collideBlocksEntities) {
            return false;
        }

        int bX = getX();
        int bY = getY();
        if (getCollisionBox().collideWithBlocksEntitiesX(bX, bY, velX, region, this, nullptr)) {
            setX(bX);
            return true;
        }
        return false;
    }

    virtual bool moveZoneY(IGameLevelZone& zone, bool collideBlocksEntities) {
        IGameLevelRegion* region = zone.getRegions()[getZoneLayer()];

        setYSub(getYSub() + velY);
        if (!collideBlocksEntities) {
            return false;
        }

        int bX = getX();
        int bY = getY();
        if (getCollisionBox().collideWithBlocksEntitiesY(bX, bY, velY, region, this, nullptr)) {
            setY(bY);
            return true;
        }
        return false;
    }
};