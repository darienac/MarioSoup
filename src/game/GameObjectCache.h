#pragma once

#include <map>
#include <string>

#include "game/GameObject.h"
#include "TileMappings.h"

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
}

namespace GameObjectCache {
    void init() {
        addObject(new GameObject("air", "air", Tiles::AIR));
        addObject(new GameObject("player", "player", Tiles::MARIO_STAND_SMB3));

        addObject(new GameObject("smb:rock", "rock", Tiles::SMB_ROCK));
        addObject(new GameObject("smb:brick", "brick", Tiles::SMB_BRICK));
        addObject(new GameObject("smb:fence", "fence", Tiles::SMB_FENCE)).setPreferredLayer(GameObject::BACKGROUND);
    }
}