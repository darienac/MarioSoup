#pragma once

#include <map>

#include "game/backgrounds/GameBackground.h"

namespace GameBackgroundCache {
    std::map<std::string, GameBackground*> backgrounds;
}

namespace {
    GameBackground& addBackground(GameBackground* background) {
        GameBackgroundCache::backgrounds[background->getId()] = background;
        return *background;
    }
}

namespace GameBackgroundCache {
    void init() {
        addBackground(new GameBackground("sma4:overworld_1")).setBgColor(0x80, 0xA8, 0xF8)
            .addLayer(new GameBackgroundLayer(Tiles::BACKGROUND_OVERWORLD1_SMA4, 0.5, 0.5, 0.0, 0.0))
            .addLayer(new GameBackgroundLayer(Tiles::BACKGROUND_CLOUDS1_SMA4, 0.75, 0.75, 0.0, 160.0));
    }
}