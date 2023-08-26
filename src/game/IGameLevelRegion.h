#pragma once

#include <set>
#include <map>

#include "game/entities/IEntity.h"

class GameObject;
class IEntity;

class IGameLevelRegion {
    public:
    union ObjectData {
        GameObject* containerObject;
    };
    struct entityCmp {
        bool operator() (IEntity* a, IEntity* b) const {
            if (a->getLayerPriority() == b->getLayerPriority()) {
                return a < b;
            } else {
                return a->getLayerPriority() < b->getLayerPriority();
            }
        }
    };

    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual GameObject** getObjectGrid() = 0;
    virtual GameObject* getGridObject(int x, int y) = 0;
    virtual void setGridObject(GameObject* object, int x, int y) = 0;
    virtual void resizeGrid(int newWidth, int newHeight, int xOff, int yOff) = 0;
    virtual int getZoneLayer() = 0;

    virtual IEntity* addEntity(IEntity* entity) = 0;
    virtual void removeEntity(IEntity* entity) = 0;
    virtual std::set<IEntity*, entityCmp>& getEntities() = 0;

    virtual void addGridData(int x, int y, ObjectData data) = 0;
    virtual void removeGridData(int x, int y) = 0;
    virtual ObjectData getGridData(int x, int y) = 0;
    virtual std::map<int, ObjectData>& getGridData() = 0;

    virtual void mapUsedObjects(std::set<GameObject*>& objects) = 0;

    virtual ~IGameLevelRegion() {}
};