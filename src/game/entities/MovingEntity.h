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

    virtual void moveX() {
        setXSub(getXSub() + velX);
    }

    virtual bool regionXCollide(IGameLevelRegion& region, CollisionBox& box, CollisionBox::collide_block_callback callback) {
        int bX = getX();
        int bY = getY();
        if (box.collideWithBlocksEntitiesX(bX, bY, velX, &region, this, nullptr)) {
            setX(bX);
            return true;
        }
        return false;
    }

    virtual bool regionXCollide(IGameLevelRegion& region, CollisionBox::collide_block_callback callback) {
        return regionXCollide(region, getCollisionBox(), callback);
    }

    virtual void moveY() {
        setYSub(getYSub() + velY);
    }

    virtual bool regionYCollide(IGameLevelRegion& region, CollisionBox& box, CollisionBox::collide_block_callback callback) {
        int bX = getX();
        int bY = getY();
        if (box.collideWithBlocksEntitiesY(bX, bY, velY, &region, this, nullptr)) {
            setY(bY);
            return true;
        }
        return false;
    }

    virtual bool regionYCollide(IGameLevelRegion& region, CollisionBox::collide_block_callback callback) {
        return regionYCollide(region, getCollisionBox(), callback);
    }
};