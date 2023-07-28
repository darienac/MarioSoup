#pragma once

#include "game/LevelSaver.h"
#include "game/GameLevel.h"
#include "game/GameObjectCache.h"

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
        std::ifstream file;
        file.open(filePath, std::ios::in | std::ios::binary);

        loadLevel(level, file);
        level.setCurrentRegion(level.getRegions()->at(0));

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

        int numRegions = readInt(stream);
        for (int i = 0; i < numRegions; i++) {
            loadLevelRegion(level, objectList, stream);
        }

        delete [] objectList;
    }

    void loadLevelRegion(GameLevel& level, GameObject** objectList, std::istream& stream) {
        int w = readInt(stream);
        int h = readInt(stream);

        GameLevelRegion* region = level.addRegion(w, h);
        GameObject** objects = region->getObjectGrid();

        for (int i = 0; i < w * h; i++) {
            objects[i] = objectList[(int) readByte(stream)];
        }
    }
};