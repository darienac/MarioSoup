#pragma once

class GameObject {
    public:
    enum LevelLayer {
        BACKGROUND,
        MIDGROUND,
        FOREGROUND
    };
    enum Flag {
        SOLID
    };

    static const int NUM_LAYERS = 3;

    private:
    bool flags[64] = {};
    const char* id;
    const char* name;
    int tilePreview;

    int levelTile;
    bool flippedX = false;
    bool flippedY = false;
    LevelLayer preferredLayer = MIDGROUND;

    public:
    GameObject(const char* id, const char* name, int tilePreview): id(id), name(name), tilePreview(tilePreview) {
        setLevelTile(tilePreview);
        flags[SOLID] = true;
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

    virtual int getLevelTile(GameObject* left, GameObject* right, GameObject* up, GameObject* down) {
        return levelTile;
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

    GameObject& flag(Flag flag) {
        flags[flag] = false;
        return *this;
    }

    GameObject& unflag(Flag flag) {
        flags[flag] = false;
        return *this;
    }

    bool isFlag(Flag flag) {
        return flags[flag];
    }


    ~GameObject() {
        std::printf("Game Object %s destroyed\n", getId());
    }
};