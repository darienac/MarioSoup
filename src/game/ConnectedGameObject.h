#pragma once

#include <algorithm>

#include "game/GameObject.h"
#include "TileMappings.h"

class ConnectedGameObject: public GameObject {
    private:
    int levelTiles[16] = {};

    int getIndex(bool left, bool right, bool up, bool down) {
        int idx = 0;
        if (left) {
            idx++;
        }
        if (right) {
            idx += 2;
        }
        if (up) {
            idx += 4;
        }
        if (down) {
            idx += 8;
        }

        return idx;
    }

    public:
    ConnectedGameObject(const char* id, const char* name, int tilePreview): GameObject(id, name, tilePreview) {
        std::fill_n(levelTiles, 16, Tiles::AIR);
    }

    GameObject& setLevelTile(bool left, bool right, bool up, bool down, int tile) {
        levelTiles[getIndex(left, right, up, down)] = tile;
        return *this;
    }

    GameObject& setLevelTile2x2(int tl, int tr, int bl, int br) {
        setLevelTile(false, true, false, true, tl);
        setLevelTile(true, false, false, true, tr);
        setLevelTile(false, true, true, false, bl);
        setLevelTile(true, false, true, false, br);

        return *this;
    }

    GameObject& setLevelTile3x3(int tl, int t, int tr, int l, int m, int r, int bl, int b, int br) {
        setLevelTile2x2(tl, tr, bl, br);

        setLevelTile(true, true, false, true, t);
        setLevelTile(false, true, true, true, l);
        setLevelTile(true, true, true, true, m);
        setLevelTile(true, false, true, true, r);
        setLevelTile(true, true, true, false, b);

        return *this;
    }

    GameObject& setLevelTile3xTop(int tl, int t, int tr, int l, int m, int r) {
        return setLevelTile3x3(tl, t, tr, l, m, r, l, m, r);
    }

    GameObject& setLevelTile3xTop(int t0) {
        return setLevelTile3xTop(t0, t0 + 1, t0 + 2, t0 + 3, t0 + 4, t0 + 5);
    }

    virtual int getLevelTile(GameObject* left, GameObject* right, GameObject* up, GameObject* down) override {
        int idx = getIndex(left == this, right == this, up == this, down == this);
        
        if (levelTiles[idx] != Tiles::AIR) {
            return levelTiles[idx];
        } else {
            return GameObject::getLevelTile();
        }
    }
};