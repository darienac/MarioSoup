#pragma once

#include <map>
#include <string>

#include "GameObject.h"
#include "TileMappings.h"

namespace GameObjectCache {
    std::map<std::string, GameObject*> objects;
};

namespace {
    void addObject(GameObject* object) {
        GameObjectCache::objects[object->getId()] = object;
    }
}

namespace GameObjectCache {
    void init() {
        addObject(new GameObject("air", "air", Tiles::AIR));
        addObject(new GameObject("smb:rock", "rock", Tiles::SMB_ROCK));
        addObject(new GameObject("smb:brick", "brick", Tiles::SMB_BRICK));
    }
}