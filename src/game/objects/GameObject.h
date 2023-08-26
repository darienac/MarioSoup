#pragma once

#include "game/IGameLevelRegion.h"

class Entity;
class AudioManager;

class GameObject {
    public:
    typedef void (*interact_callback)(int tileX, int tileY, IEntity* entity, IGameLevelRegion& region);
    typedef void (*player_interact_callback)(int tileX, int tileY, IMario* mario, IGameLevelRegion& region, AudioManager& audio);
    typedef void (*gameobj_callback)(int tileX, int tileY, IGameLevelRegion& region);

    enum LevelLayer {
        BACKGROUND,
        MIDGROUND,
        FOREGROUND
    };
    enum Flag {
        SOLID,
        CONTAINS_ITEM,
        ITEM,
        ENTITY
    };

    static const int NUM_LAYERS = 3;

    private:
    bool flags[64] = {};
    const char* id;
    const char* name;
    int tilePreview;

    int levelTile;
    bool flippedX = false;
    bool flippedY = false;
    LevelLayer preferredLayer = MIDGROUND;

    interact_callback hitUnder = nullptr;
    interact_callback blockEmerge = nullptr;
    player_interact_callback playerCollide = nullptr;
    gameobj_callback entityReplace = nullptr;

    public:
    GameObject(const char* id, const char* name, int tilePreview): id(id), name(name), tilePreview(tilePreview) {
        setLevelTile(tilePreview);
        flags[SOLID] = true;
    }

    const char* getId() {
        return id;
    }

    const char* getName() {
        return name;
    }

    int getTilePreview() {
        return tilePreview;
    }

    virtual int getLevelTile(GameObject* left, GameObject* right, GameObject* up, GameObject* down) {
        return levelTile;
    }

    virtual int getLevelTile() {
        return levelTile;
    }

    GameObject& setLevelTile(int tile) {
        levelTile = tile;
        return *this;
    }

    virtual bool isFlippedX() {
        return flippedX;
    }

    GameObject& setFlippedX(bool value) {
        flippedX = value;
        return *this;
    }

    virtual bool isFlippedY() {
        return flippedY;
    }

    GameObject& setFlippedY(bool value) {
        flippedY = value;
        return *this;
    }

    LevelLayer getPreferredLayer() {
        return preferredLayer;
    }

    GameObject& setPreferredLayer(LevelLayer value) {
        preferredLayer = value;
        return *this;
    }

    GameObject& flag(Flag flag) {
        flags[flag] = true;
        return *this;
    }

    GameObject& unflag(Flag flag) {
        flags[flag] = false;
        return *this;
    }

    bool isFlag(Flag flag) {
        return flags[flag];
    }

    GameObject& setOnHitUnder(interact_callback callback) {
        hitUnder = callback;
        return *this;
    }

    void onHitUnder(int tileX, int tileY, IEntity* entity, IGameLevelRegion& region) {
        if (hitUnder != nullptr) {
            hitUnder(tileX, tileY, entity, region);
        }
    }

    GameObject& setOnBlockEmerge(interact_callback callback) {
        blockEmerge = callback;
        return *this;
    }

    void onBlockEmerge(int tileX, int tileY, IEntity* entity, IGameLevelRegion& region) {
        if (blockEmerge != nullptr) {
            blockEmerge(tileX, tileY, entity, region);
        }
    }

    GameObject& setOnPlayerCollide(player_interact_callback callback) {
        playerCollide = callback;
        return *this;
    }

    void onPlayerCollide(int tileX, int tileY, IMario* mario, IGameLevelRegion& region, AudioManager& audio) {
        if (playerCollide != nullptr) {
            playerCollide(tileX, tileY, mario, region, audio);
        }
    }

    GameObject& setOnEntityReplace(gameobj_callback callback) {
        flag(ENTITY);
        entityReplace = callback;
        return *this;
    }

    void onEntityReplace(int tileX, int tileY, IGameLevelRegion& region) {
        if (entityReplace != nullptr) {
            entityReplace(tileX, tileY, region);
        }
    }

    ~GameObject() {
        std::printf("Game Object %s destroyed\n", getId());
    }
};