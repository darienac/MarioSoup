#pragma once

class GameObject {
    public:
    enum LevelLayer {
        BACKGROUND,
        MIDGROUND,
        FOREGROUND
    };
    static const int NUM_LAYERS = 3;

    private:
    const char* id;
    const char* name;
    int tilePreview;

    int levelTile;
    bool flippedX = false;
    bool flippedY = false;
    LevelLayer preferredLayer = MIDGROUND;

    public:
    GameObject(const char* id, const char* name, int tilePreview): id(id), name(name), tilePreview(tilePreview) {
        levelTile = tilePreview;
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

    int getLevelTile() {
        return levelTile;
    }

    GameObject& setLevelTile(int tile) {
        levelTile = tile;
        return *this;
    }

    bool isFlippedX() {
        return flippedX;
    }

    GameObject& setFlippedX(bool value) {
        flippedX = value;
        return *this;
    }

    bool isFlippedY() {
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


    ~GameObject() {
        std::printf("Game Object %s destroyed\n", getId());
    }
};