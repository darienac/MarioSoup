#pragma once

class IGameLevelZone;
class AudioManager;
class IControls;
class CollisionBox;
class IMario;
class GameObject;

class IEntity {
    public:
    static GameObject* air;
    enum Priority {
        ITEM,
        ENEMY,
        ITEM_CONTAINER,
        MARIO,
        PARTICLE
    };

    static int mod(int v0, int v1) {
        return (v0 + 512) % v1;
    }

    static int div(int v0, int v1) {
        return (v0 - mod(v0, v1)) / v1;
    }

    virtual int getZoneLayer() = 0;
    virtual void setZoneLayer(int value) = 0;
    virtual int getLayerPriority() const = 0;
    virtual int getX() = 0;
    virtual void setX(int value) = 0;
    virtual int getY() = 0;
    virtual void setY(int value) = 0;
    virtual GameObject& getGameObject() = 0;
    virtual void tick(IGameLevelZone& zone, AudioManager& audio, IControls& controls) = 0;
    virtual bool shouldDelete() {return false;}

    virtual CollisionBox& getCollisionBox() = 0;

    virtual void onPushed(IEntity& entity, int dx, int dy) {}
    virtual void onCollideMario(IMario& mario) {}
    virtual void onCollideEntity(IEntity& entity) {}

    virtual bool isSolid() {return false;}
    virtual bool isPushable() {return false;}

    virtual ~IEntity() {}
};

GameObject* IEntity::air = nullptr;