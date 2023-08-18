#pragma once

class IGameLevelZone;
class AudioManager;
class IControls;
class CollisionBox;
class Mario;

class IEntity {
    public:
    enum Priority {
        ITEM,
        ITEM_CONTAINER,
        MARIO
    };

    virtual int getZoneLayer() = 0;
    virtual void setZoneLayer(int value) = 0;
    virtual int getLayerPriority() const = 0;
    virtual int getX() = 0;
    virtual void setX(int value) = 0;
    virtual int getY() = 0;
    virtual void setY(int value) = 0;
    virtual GameObject& getGameObject() = 0;
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) = 0;

    virtual CollisionBox& getCollisionBox() = 0;
    virtual void onCollideMario(Mario& mario) = 0;
    virtual void onCollideEntity(IEntity& entity) = 0;

    virtual bool isSolid() {return true;}

    bool operator<(const IEntity& other) const {
        // Define your custom sorting order based on the 'property'
        return getLayerPriority() < other.getLayerPriority();
    }

    virtual ~IEntity() {}
};