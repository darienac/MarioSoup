#pragma once

#include "game/LevelSaver.h"
#include "game/GameLevel.h"
#include "game/objects/GameObjectCache.h"
#include "ResourceReader.h"

class LevelLoader {
    private:
    static const int MAX_STRING_SIZE = 1024;

    char stringBuffer[MAX_STRING_SIZE];

    int readInt(std::istream& stream) {
        int out;
        stream.read((char*) &out, sizeof(int));
        return out;
    }
    char readByte(std::istream& stream) {
        char out;
        stream.read(&out, sizeof(char));
        return out;
    }
    void readString(std::istream& stream, char* buffer) {
        char* bufferPos = buffer;
        int charsRead = 0;
        do {
            if (stream.eof()) {
                throw "File end reached before end of string";
            }
            stream.read(bufferPos, sizeof(char));
            bufferPos++;
            charsRead++;
            if (charsRead == MAX_STRING_SIZE && bufferPos[-1] != '\0') {
                throw "Max string size reached in level load";
            }
        } while (bufferPos[-1] != '\0');
    }


    public:
    LevelLoader() {}

    void loadLevel(GameLevel& level, const char* filePath) {
        std::string fullPath = ResourceReader::getFullPath(ResourceReader::World, filePath);
        std::ifstream file;
        file.open(fullPath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            file.close();
            throw "Unable to open file";
        }

        loadLevel(level, file);
        level.setCurrentZone(level.getZones()->at(0));

        file.close();
    }

    void loadLevel(GameLevel& level, std::istream& stream) {
        if (readInt(stream) != LevelSaver::VERSION_NUM) {
            throw "Incompatible version number";
        }

        int objectListSize = readInt(stream);
        GameObject** objectList = new GameObject*[objectListSize];
        for (int i = 0; i < objectListSize; i++) {
            readString(stream, stringBuffer);
            objectList[i] = GameObjectCache::objects[stringBuffer];
        }

        int numZones = readInt(stream);
        for (int i = 0; i < numZones; i++) {
            loadLevelZone(level, objectList, stream);
        }

        delete [] objectList;
    }

    void loadLevelZone(GameLevel& level, GameObject** objectList, std::istream& stream) {
        int w = readInt(stream);
        int h = readInt(stream);

        GameLevelZone* zone = level.addZone(w, h);

        loadMario(zone->getMario(), objectList, stream);

        GameLevelRegion** regions = zone->getRegions();
        for (int i = 0; i < GameObject::NUM_LAYERS; i++) {
            loadLevelRegion(*regions[i], objectList, stream);
        }
    }

    void loadMario(Mario& mario, GameObject** objectList, std::istream& stream) {
        mario.setX(readInt(stream));
        mario.setY(readInt(stream));
    }

    void loadLevelRegion(GameLevelRegion& region, GameObject** objectList, std::istream& stream) {
        GameObject** objects = region.getObjectGrid();

        for (int i = 0; i < region.getWidth() * region.getHeight(); i++) {
            objects[i] = objectList[(int) readByte(stream)];
        }
    }
};