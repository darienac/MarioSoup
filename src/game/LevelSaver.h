#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#include "game/GameLevel.h"

class LevelSaver {
    private:
    public:
    LevelSaver() {}

    void saveLevel(GameLevel& level, const char* filePath) {
        std::ofstream file;
        file.open(filePath, std::ios::in, std::ios::trunc);

        saveLevel(level, file);

        file.close();
    }

    void saveLevel(GameLevel& level, std::ostream& stream) {
        std::set<GameObject*> objectSet;
        level.mapUsedObjects(objectSet);
        std::vector<GameObject*> objectList(objectSet.begin(), objectSet.end());
        std::map<GameObject*, int> objectKey;
        for (int i = 0; i < objectList.size(); i++) {
            objectKey[objectList.at(i)] = i;
        }

        stream << objectList.size();
        for (GameObject* object : objectList) {
            stream << object->getId();
        }

        for (GameLevelRegion* region : *level.getRegions()) {
            saveLevelRegion(*region, objectKey, stream);
        }
    }

    void saveLevelRegion(GameLevelRegion& region, std::map<GameObject*, int> objectKey, std::ostream& stream) {
        int w = region.getWidth();
        int h = region.getHeight();
        stream << w;
        stream << h;

        GameObject** objects = region.getObjectGrid();
        for (int i = 0; i < w * h; i++) {
            stream << objectKey[objects[i]];
        }
    }
};