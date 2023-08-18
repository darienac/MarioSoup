#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#include "game/GameLevel.h"
#include "ResourceReader.h"

class LevelSaver {
    private:
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
    static const int VERSION_NUM = 1;

    LevelSaver() {}

    void saveLevel(GameLevel& level, const char* filePath) {
        std::string fullPath = ResourceReader::getFullPath(ResourceReader::World, filePath);
        std::ofstream file;
        file.open(fullPath, std::ios::out | std::ios::trunc | std::ios::binary);

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

        writeInt(stream, level.getZones()->size());

        for (GameLevelZone* zone : *level.getZones()) {
            saveLevelZone(*zone, objectKey, stream);
        }
    }

    void saveLevelZone(GameLevelZone& zone, std::map<GameObject*, int> objectKey, std::ostream& stream) {
        writeInt(stream, zone.getWidth());
        writeInt(stream, zone.getHeight());
        
        saveMario(zone.getMario(), stream);

        IGameLevelRegion** regions = zone.getRegions();
        for (int i = 0; i < GameObject::NUM_LAYERS; i++) {
            saveLevelRegion(*regions[i], objectKey, stream);
        }
    }

    void saveMario(Mario& mario, std::ostream& stream) {
        writeInt(stream, mario.getX());
        writeInt(stream, mario.getY());
    }

    void saveLevelRegion(IGameLevelRegion& region, std::map<GameObject*, int> objectKey, std::ostream& stream) {
        GameObject** objects = region.getObjectGrid();
        for (int i = 0; i < region.getWidth() * region.getHeight(); i++) {
            writeByte(stream, objectKey[objects[i]]);
        }
        std::map<int, GameLevelRegion::ObjectData>* gridData = &region.getGridData();
        writeInt(stream, gridData->size());
        for (const auto& [key, value] : *gridData) {
            writeInt(stream, key);

            int x = key % GameLevelRegion::MAX_WIDTH;
            int y = key / GameLevelRegion::MAX_WIDTH;
            GameObject* object = region.getGridObject(x, y);
            if (object->isFlag(GameObject::CONTAINS_ITEM) && region.getGridData(x, y).containerObject != nullptr) {
                writeInt(stream, objectKey[region.getGridData(x, y).containerObject]);
            }
        }
    }
};