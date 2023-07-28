#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#include "game/GameLevel.h"

class LevelSaver {
    private:
    static const int VERSION_NUM = 1;

    void writeInt(std::ostream& stream, const int data) {
        stream.write((char*) &data, sizeof(int));
    }
    void writeByte(std::ostream& stream, const char data) {
        stream.write(&data, sizeof(char));
    }
    void writeString(std::ostream& stream, const char* data) {
        stream.write(data, strlen(data) + 1);
    }

    public:
    LevelSaver() {}

    void saveLevel(GameLevel& level, const char* filePath) {
        std::ofstream file;
        file.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

        saveLevel(level, file);

        file.close();
    }

    void saveLevel(GameLevel& level, std::ostream& stream) {
        std::set<GameObject*> objectSet;
        level.mapUsedObjects(objectSet);
        std::vector<GameObject*> objectList(objectSet.begin(), objectSet.end());
        std::map<GameObject*, int> objectKey;
        for (size_t i = 0; i < objectList.size(); i++) {
            objectKey[objectList.at(i)] = i;
        }

        writeInt(stream, VERSION_NUM);
        writeInt(stream, objectList.size());
        for (GameObject* object : objectList) {
            writeString(stream, object->getId());
        }

        for (GameLevelRegion* region : *level.getRegions()) {
            saveLevelRegion(*region, objectKey, stream);
        }
    }

    void saveLevelRegion(GameLevelRegion& region, std::map<GameObject*, int> objectKey, std::ostream& stream) {
        int w = region.getWidth();
        int h = region.getHeight();
        writeInt(stream, w);
        writeInt(stream, h);

        GameObject** objects = region.getObjectGrid();
        for (int i = 0; i < w * h; i++) {
            writeByte(stream, objectKey[objects[i]]);
        }
    }
};