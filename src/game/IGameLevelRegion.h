#pragma once

class GameObject;

class IGameLevelRegion {
    public:
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual GameObject** getObjectGrid() = 0;
    virtual GameObject* getGridObject(int x, int y) = 0;
    virtual void setGridObject(GameObject* object, int x, int y) = 0;
    virtual void resizeGrid(int newWidth, int newHeight, int xOff, int yOff) = 0;
};