#pragma once

class IGameLevelZone;
class AudioManager;
class IControls;
class CollisionBox;
class Mario;

class IEntity {
    public:
    virtual int getZoneLayer() = 0;
    virtual void setZoneLayer(int value) = 0;
    virtual int getX() = 0;
    virtual void setX(int value) = 0;
    virtual int getY() = 0;
    virtual void setY(int value) = 0;
    virtual GameObject& getGameObject() = 0;
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) = 0;

    virtual CollisionBox& getCollisionBox() = 0;
    virtual void onCollideMario(Mario& mario) = 0;
    virtual void onCollideEntity(IEntity& entity) = 0;
};