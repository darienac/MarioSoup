#pragma once

#include <map>
#include <string>

#include "game/objects/GameObject.h"
#include "game/objects/ConnectedGameObject.h"
#include "game/objects/AnimatedGameObject.h"
#include "game/IGameLevelRegion.h"
#include "TileMappings.h"

using namespace Tiles;

namespace GameObjectCache {
    struct strCompare {
        bool operator()(const char* a, const char* b) const {
            return strcmp(a, b) < 0;
        }
    };
    std::map<const char*, GameObject*, strCompare> objects;
};

namespace {
    GameObject& addObject(GameObject* object) {
        GameObjectCache::objects[object->getId()] = object;
        return *object;
    }
    ConnectedGameObject& addObject(ConnectedGameObject* object) {
        GameObjectCache::objects[object->getId()] = object;
        return *object;
    }
    AnimatedGameObject& addObject(AnimatedGameObject* object) {
        GameObjectCache::objects[object->getId()] = object;
        return *object;
    }
}

namespace GameObjectCache {
    void init() {
        AnimatedGameObject::setTickNum(0);

        addObject(new GameObject("air", "air", AIR)).unflag(GameObject::SOLID);
        addObject(new GameObject("player", "player", MARIO_STAND_SMA4)).setFlippedX(true);

        addObject(new ConnectedGameObject("sma4:wood_floor", "wood floor", SMA4_WOODF_T))
            .setLevelTile3xTop(SMA4_WOODF_TL);
        addObject(new ConnectedGameObject("sma4:wood_platform", "wood platform", SMA4_WOODP))
            .setLevelTile3x2(SMA4_WOODP_TL)
            .setLevelTile3x1(SMA4_WOODP_L, SMA4_WOODP_M, SMA4_WOODP_R);
        
        addObject(new AnimatedGameObject("sma4:qblock", "question block", SMA4_QBLOCK_1, 8)).add(SMA4_QBLOCK_1, 4).flag(GameObject::CONTAINS_ITEM)
            .setOnHitUnder([](int tileX, int tileY, IGameLevelRegion& region) {
                std::printf("Tile hit: %d, %d\n", tileX, tileY);
            });
        addObject(new GameObject("sma4:empty_block", "empty block", SMA4_QBLOCK_EMPTY));
        addObject(new AnimatedGameObject("sma4:brick", "brick block", SMA4_BRICK_1, 8)).add(SMA4_BRICK_1, 4).flag(GameObject::CONTAINS_ITEM);
        addObject(new AnimatedGameObject("sma4:coin", "coin", SMA4_COIN_1, 8)).add(SMA4_COIN_1, 4).unflag(GameObject::SOLID); // Only sort of an item, qblocks have coins by default

        addObject(new AnimatedGameObject("sma4:item_coin", "coin (item)", SMA4_ITEMCOIN_1, 4)).add(SMA4_ITEMCOIN_1, 3).add(SMA4_ITEMCOIN_2).flag(GameObject::ITEM);
        addObject(new GameObject("sma4:item_mushroom", "mushroom", SMA4_MUSHROOM)).flag(GameObject::ITEM);
    }
}