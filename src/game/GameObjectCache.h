#pragma once

#include <map>
#include <string>

#include "game/GameObject.h"
#include "game/ConnectedGameObject.h"
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
}

namespace GameObjectCache {
    void init() {
        addObject(new GameObject("air", "air", AIR)).unflag(GameObject::SOLID);
        addObject(new GameObject("player", "player", MARIO_STAND_SMB3)).setFlippedX(true);

        addObject(new GameObject("smb:rock", "rock", SMB_ROCK));
        addObject(new GameObject("smb:brick", "brick", SMB_BRICK));
        addObject(new GameObject("smb:fence", "fence", SMB_FENCE)).setPreferredLayer(GameObject::BACKGROUND);

        addObject(new ConnectedGameObject("smb3:wood_floor", "wood floor", SMB3_WOODF_T))
            .setLevelTile3xTop(SMB3_WOODF_TL);
        addObject(new ConnectedGameObject("smb3:wood_platform", "wood platform", SMB3_WOODP_M))
            .setLevelTile3x2(SMB3_WOODP_TL)
            .setLevelTile3x1(SMB3_WOODP_L);
    }
}