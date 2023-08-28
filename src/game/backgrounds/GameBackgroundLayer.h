#pragma once

#include "TileMappings.h"

class GameBackgroundLayer {
    private:
    Tile* tile;
    double scrollXMult = 1.0;
    double scrollYMult = 1.0;
    double xOff = 0.0;
    double yOff = 0.0;

    public:
    GameBackgroundLayer(int tile, double scrollXMult, double scrollYMult, double xOff, double yOff): tile(&Tiles::getTile(tile)), scrollXMult(scrollXMult), scrollYMult(scrollYMult), xOff(xOff), yOff(yOff) {}

    Tile* getTile() {
        return tile;
    }

    double getScrollXMult() {
        return scrollXMult;
    }

    double getScrollYMult() {
        return scrollYMult;
    }

    double getXOff() {
        return xOff;
    }

    double getYOff() {
        return yOff;
    }
};