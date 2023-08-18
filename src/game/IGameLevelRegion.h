#pragma once

class GameObject;
class IEntity;

class IGameLevelRegion {
    public:
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual GameObject** getObjectGrid() = 0;
    virtual GameObject* getGridObject(int x, int y) = 0;
    virtual void setGridObject(GameObject* object, int x, int y) = 0;
    virtual void resizeGrid(int newWidth, int newHeight, int xOff, int yOff) = 0;
    virtual int getZoneLayer() = 0;

    virtual void addEntity(IEntity* entity) = 0;
    virtual void removeEntity(IEntity* entity) = 0;
};