#pragma once

#include <functional>

#include "game/entities/IEntity.h"
#include "game/IGameLevelRegion.h"

class CollisionBox {
    private:
    int xOff;
    int yOff;
    int width;
    int height;
    
    public:
    // typedef void (*collide_block_callback)(int tileX, int tileY, GameObject* object);
    typedef std::function<void(int, int, GameObject*)> collide_block_callback;

    CollisionBox(int xOff, int yOff, int width, int height): xOff(xOff), yOff(yOff), width(width), height(height) {}

    int getXOff() {
        return xOff;
    }
    int getYOff() {
        return yOff;
    }
    int getWidth() {
        return width;
    }
    int getHeight() {
        return height;
    }

    bool collidesWith(CollisionBox& box, int relX, int relY) {
        int b1x1 = xOff;
        int b1x2 = b1x1 + width - 1;
        int b1y1 = yOff;
        int b1y2 = b1y1 + height - 1;
        
        int b2x1 = box.getXOff() + relX;
        int b2x2 = b2x1 + box.getWidth() - 1;
        int b2y1 = box.getYOff() + relY;
        int b2y2 = b2y1 + box.getHeight() - 1;

        bool xCollide = (b1x1 >= b2x1 && b1x1 <= b2x2) || (b1x2 >= b2x1 && b1x2 <= b2x2) || (b2x1 >= b1x1 && b2x1 <= b1x2) || (b2x2 >= b1x1 && b2x2 <= b1x2);
        bool yCollide = (b1y1 >= b2y1 && b1y1 <= b2y2) || (b1y2 >= b2y1 && b1y2 <= b2y2) || (b2y1 >= b1y1 && b2y1 <= b1y2) || (b2y2 >= b1y1 && b2y2 <= b1y2);
        return xCollide && yCollide;
    }

    bool pushBoxLeft(int x, int y, CollisionBox& box, int& boxX, int& boxY) {
        if (!collidesWith(box, boxX - x, boxY - y)) {
            return false;
        }
        boxX = x + xOff - box.getXOff() - box.getWidth();
        return true;
    }
    bool pushBoxRight(int x, int y, CollisionBox& box, int& boxX, int& boxY) {
        if (!collidesWith(box, boxX - x, boxY - y)) {
            return false;
        }
        boxX = x + xOff + width - box.getXOff();
        return true;
    }
    bool pushBoxDown(int x, int y, CollisionBox& box, int& boxX, int& boxY) {
        if (!collidesWith(box, boxX - x, boxY - y)) {
            return false;
        }
        boxY = y + yOff - box.getYOff() - box.getHeight();
        return true;
    }
    bool pushBoxUp(int x, int y, CollisionBox& box, int& boxX, int& boxY) {
        if (!collidesWith(box, boxX - x, boxY - y)) {
            return false;
        }
        boxY = y + yOff + height - box.getYOff();
        return true;
    }

    bool pushBoxAway(int x, int y, CollisionBox& box, int& boxX, int& boxY) {
        if (!collidesWith(box, boxX - x, boxY - y)) {
            return false;
        }
        int insetLeft = boxX + box.getXOff() + box.getWidth() - (x + xOff);
        int insetRight = x + xOff + width - (boxX + box.getXOff());
        int insetDown = boxY + box.getYOff() + box.getHeight() - (y + yOff);
        int insetUp = y + yOff + height - (boxY + box.getYOff());
        int minInset = insetLeft;
        if (insetRight > 0 && insetRight < minInset) {
            minInset = insetRight;
        }
        if (insetDown > 0 && insetDown < minInset) {
            minInset = insetDown;
        }
        if (insetUp > 0 && insetUp < minInset) {
            minInset = insetUp;
        }

        if (minInset == insetLeft) {
            return pushBoxLeft(x, y, box, boxX, boxY);
        }
        if (minInset == insetRight) {
            return pushBoxRight(x, y, box, boxX, boxY);
        }
        if (minInset == insetDown) {
            return pushBoxDown(x, y, box, boxX, boxY);
        }
        if (minInset == insetUp) {
            return pushBoxUp(x, y, box, boxX, boxY);
        }
        throw "Unreachable code reached in pushing box away";
        return false;
    }

    bool collideWithRegionX(int& x, int& y, int velX, IGameLevelRegion* region, collide_block_callback callback) {
        bool collide = false;
        CollisionBox regionBox = CollisionBox(0, 0, 16, 16);
        int xShift = 0;
        for (int i = IEntity::div(x + xOff, 16); i * 16 < x + xOff + width; i++) {
            for (int j = IEntity::div(y + yOff, 16); j * 16 < y + yOff + height; j++) {
                GameObject* object = region->getGridObject(i, j);
                if (callback == nullptr && !object->isFlag(GameObject::SOLID)) {
                    continue;
                }
                bool thisCollide = false;
                int bX = x;
                int bY = y;
                if (velX > 0) {
                    thisCollide = regionBox.pushBoxLeft(i * 16, j * 16, *this, bX, bY);
                } else {
                    thisCollide = regionBox.pushBoxRight(i * 16, j * 16, *this, bX, bY);
                }
                if (callback != nullptr && thisCollide) {
                    callback(i, j, object);
                }
                if (object->isFlag(GameObject::SOLID) && std::abs(bX - x) > std::abs(xShift)) {
                    collide |= thisCollide;
                    xShift = bX - x;
                }
            }
        }
        x += xShift;
        return collide;
    }

    bool collideWithRegionY(int& x, int& y, int velY, IGameLevelRegion* region, collide_block_callback callback) {
        bool collide = false;
        CollisionBox regionBox = CollisionBox(0, 0, 16, 16);
        int yShift = 0;
        for (int i = IEntity::div(x + xOff, 16); i * 16 < x + xOff + width; i++) {
            for (int j = IEntity::div(y + yOff, 16); j * 16 < y + yOff + height; j++) {
                GameObject* object = region->getGridObject(i, j);
                if (callback == nullptr && !object->isFlag(GameObject::SOLID)) {
                    continue;
                }
                bool thisCollide = false;
                int bX = x;
                int bY = y;
                if (velY > 0) {
                    thisCollide = regionBox.pushBoxDown(i * 16, j * 16, *this, bX, bY);
                } else {
                    thisCollide = regionBox.pushBoxUp(i * 16, j * 16, *this, bX, bY);
                }
                if (callback != nullptr && thisCollide) {
                    callback(i, j, object);
                }
                if (object->isFlag(GameObject::SOLID) && std::abs(bY - y) > std::abs(yShift)) {
                    collide |= thisCollide;
                    yShift = bY - y;
                }
            }
        }
        y += yShift;
        return collide;
    }

    bool collideWithEntitiesX(int& x, int& y, int velX, IGameLevelRegion* region, IEntity* ignore) {
        bool collide = false;
        int xShift = 0;
        for (IEntity* entity : region->getEntities()) {
            if (entity == ignore || !entity->isSolid()) {
                continue;
            }
            int bX = x;
            int bY = y;
            if (velX > 0) {
                collide |= entity->getCollisionBox().pushBoxLeft(entity->getX(), entity->getY(), *this, bX, bY);
            } else {
                collide |= entity->getCollisionBox().pushBoxRight(entity->getX(), entity->getY(), *this, bX, bY);
            }
            if (std::abs(bX - x) > std::abs(xShift)) {
                xShift = bX - x;
            }
        }
        x += xShift;
        return collide;
    }

    bool collideWithEntitiesY(int& x, int& y, int velY, IGameLevelRegion* region, IEntity* ignore) {
        bool collide = false;
        int yShift = 0;
        for (IEntity* entity : region->getEntities()) {
            if (entity == ignore || !entity->isSolid()) {
                continue;
            }
            int bX = x;
            int bY = y;
            if (velY > 0) {
                collide |= entity->getCollisionBox().pushBoxDown(entity->getX(), entity->getY(), *this, bX, bY);
            } else {
                collide |= entity->getCollisionBox().pushBoxUp(entity->getX(), entity->getY(), *this, bX, bY);
            }
            if (std::abs(bY - y) > std::abs(yShift)) {
                yShift = bY - y;
            }
        }
        y += yShift;
        return collide;
    }
    
    bool collideWithBlocksEntitiesX(int& x, int& y, int velX, IGameLevelRegion* region, IEntity* ignore, collide_block_callback callbackBlock) {
        int bX = x;
        int bY = y;
        int eX = x;
        int eY = y;
        bool collides = collideWithRegionX(bX, bY, velX, region, callbackBlock) || collideWithEntitiesX(eX, eY, velX, region, ignore);

        x = std::abs(bX - x) > std::abs(eX - x) ? bX : eX;
        return collides;
    }

    bool collideWithBlocksEntitiesY(int& x, int& y, int velY, IGameLevelRegion* region, IEntity* ignore, collide_block_callback callbackBlock) {
        int bX = x;
        int bY = y;
        int eX = x;
        int eY = y;
        bool collides = collideWithRegionY(bX, bY, velY, region, callbackBlock) || collideWithEntitiesY(eX, eY, velY, region, ignore);

        y = std::abs(bY - y) > std::abs(eY - y) ? bY : eY;
        return collides;
    }
};