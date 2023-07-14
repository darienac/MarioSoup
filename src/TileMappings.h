#pragma once

#include "render/Texture.h"

namespace Tiles {
    const std::string textEncoding = " 0123456789abcdefghijklmnopqrstuvwxyz-X!.C";
    Texture* textures[64];
};

namespace Palletes {
    unsigned char MARIO[16] = {
        0, 0, 0, 0,
        181, 49, 33, 255,
        230, 156, 33, 255,
        107, 107, 0, 255
    };

    unsigned char SNOW[16] = {
        148, 148, 255, 255,
        255, 255, 255, 255,
        99, 99, 99, 255,
        173, 173, 173, 255
    };

    unsigned char OVERWORLD_0[16] = {
        148, 148, 255, 255,
        140, 214, 0, 255,
        16, 148, 0, 255,
        0, 0, 0, 255
    };

    unsigned char OVERWORLD_1[16] = {
        148, 148, 255, 255,
        255, 206, 197, 255,
        156, 74, 0, 255,
        0, 0, 0, 255
    };

    unsigned char OVERWORLD_2[16] = {
        148, 148, 255, 255,
        255, 255, 255, 255,
        99, 173, 255, 255,
        0, 0, 0, 255
    };
};

namespace {
    enum TextureName {
        MARIO,
        TILESET,
        HUD,
        UI
    };
};

class Tile {
    private:
    public:
    Texture* texture;
    int cx;
    int cy;
    int cw;
    int ch;

    // Initialization based on selection coordinates (what paint.net uses)
    Tile(TextureName texName, int sx, int sy, int sw, int sh) {
        texture = Tiles::textures[texName];
        cx = sx;
        cw = sw;
        cy = texture->getHeight() - sh - sy;
        ch = sh;
    };

    Tile() {}
};

namespace {
    Tile tiles[256];
};

namespace Tiles {
    enum {
        AIR,
        D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
        LA, LB, LC, LD, LE, LF, LG, LH, LI, LJ, LK, LL, LM, LN, LO, LP, LQ, LR, LS, LT, LU, LV, LW, LX, LY, LZ,
        DA, XX, EX, DO, CO,

        MENU_OFF, MENU_ON, MENULIST_OFF, MENULIST_ON, MENULIST_TOP, MENULIST_BOTTOM, RADIO_OFF, RADIO_ON, SELECT_BLOCK, BUTTON_LEFT, BUTTON_MID, BUTTON_RIGHT, BUTTON_ON_LEFT, BUTTON_ON_MID, BUTTON_ON_RIGHT, UIBOX_BL, UIBOX_L, UIBOX_TL, UIBOX_T, UIBOX_TR, UIBOX_R, UIBOX_BR, UIBOX_B, UIBOX_C,

        COIN_ICO, TITLE,

        ROCK,
        BRICK
    };

    Tile& getTile(int tile) {
        return tiles[tile];
    };

    void initTiles() {
        textures[TextureName::MARIO] = new Texture("smb_mario_and_luigi.png");
        textures[TextureName::TILESET] = new Texture("smb_misc_tileset.png");
        textures[TextureName::HUD] = new Texture("smb_hud.png");
        textures[TextureName::UI] = new Texture("ui_elements.png");

        tiles[AIR] = Tile(HUD, 264, 8, 0, 0);

        tiles[D0] = Tile(HUD, 264, 8, 8, 8);
        tiles[D1] = Tile(HUD, 273, 8, 8, 8);
        tiles[D2] = Tile(HUD, 282, 8, 8, 8);
        tiles[D3] = Tile(HUD, 291, 8, 8, 8);
        tiles[D4] = Tile(HUD, 300, 8, 8, 8);
        tiles[D5] = Tile(HUD, 309, 8, 8, 8);
        tiles[D6] = Tile(HUD, 318, 8, 8, 8);
        tiles[D7] = Tile(HUD, 327, 8, 8, 8);
        tiles[D8] = Tile(HUD, 336, 8, 8, 8);
        tiles[D9] = Tile(HUD, 345, 8, 8, 8);

        tiles[LA] = Tile(HUD, 354, 8, 8, 8);
        tiles[LB] = Tile(HUD, 363, 8, 8, 8);
        tiles[LC] = Tile(HUD, 372, 8, 8, 8);
        tiles[LD] = Tile(HUD, 381, 8, 8, 8);
        tiles[LE] = Tile(HUD, 390, 8, 8, 8);
        tiles[LF] = Tile(HUD, 399, 8, 8, 8);

        tiles[LG] = Tile(HUD, 264, 17, 8, 8);
        tiles[LH] = Tile(HUD, 273, 17, 8, 8);
        tiles[LI] = Tile(HUD, 282, 17, 8, 8);
        tiles[LJ] = Tile(HUD, 291, 17, 8, 8);
        tiles[LK] = Tile(HUD, 300, 17, 8, 8);
        tiles[LL] = Tile(HUD, 309, 17, 8, 8);
        tiles[LM] = Tile(HUD, 318, 17, 8, 8);
        tiles[LN] = Tile(HUD, 327, 17, 8, 8);
        tiles[LO] = Tile(HUD, 336, 17, 8, 8);
        tiles[LP] = Tile(HUD, 345, 17, 8, 8);
        tiles[LQ] = Tile(HUD, 354, 17, 8, 8);
        tiles[LR] = Tile(HUD, 363, 17, 8, 8);
        tiles[LS] = Tile(HUD, 372, 17, 8, 8);
        tiles[LT] = Tile(HUD, 381, 17, 8, 8);
        tiles[LU] = Tile(HUD, 390, 17, 8, 8);
        tiles[LV] = Tile(HUD, 399, 17, 8, 8);

        tiles[LW] = Tile(HUD, 264, 26, 8, 8);
        tiles[LX] = Tile(HUD, 273, 26, 8, 8);
        tiles[LY] = Tile(HUD, 282, 26, 8, 8);
        tiles[LZ] = Tile(HUD, 291, 26, 8, 8);
        tiles[DA] = Tile(HUD, 300, 26, 8, 8);
        tiles[XX] = Tile(HUD, 309, 26, 8, 8);
        tiles[EX] = Tile(HUD, 318, 26, 8, 8);
        tiles[DO] = Tile(HUD, 327, 26, 8, 8);
        tiles[CO] = Tile(HUD, 336, 26, 8, 8);

        tiles[MENU_OFF] = Tile(UI, 0, 0, 8, 16);
        tiles[MENU_ON] = Tile(UI, 8, 0, 8, 16);
        tiles[MENULIST_OFF] = Tile(UI, 0, 0, 8, 8);
        tiles[MENULIST_ON] = Tile(UI, 8, 0, 8, 8);
        tiles[MENULIST_TOP] = Tile(UI, 0, 24, 8, 4);
        tiles[MENULIST_BOTTOM] = Tile(UI, 0, 28, 8, 4);
        tiles[RADIO_OFF] = Tile(UI, 0, 16, 8, 8);
        tiles[RADIO_ON] = Tile(UI, 8, 16, 8, 8);
        tiles[SELECT_BLOCK] = Tile(UI, 16, 0, 16, 16);
        tiles[BUTTON_LEFT] = Tile(UI, 32, 0, 8, 16);
        tiles[BUTTON_MID] = Tile(UI, 36, 0, 8, 16);
        tiles[BUTTON_RIGHT] = Tile(UI, 40, 0, 8, 16);
        tiles[BUTTON_ON_LEFT] = Tile(UI, 32, 16, 8, 16);
        tiles[BUTTON_ON_MID] = Tile(UI, 36, 16, 8, 16);
        tiles[BUTTON_ON_RIGHT] = Tile(UI, 40, 16, 8, 16);
        tiles[UIBOX_BL] = Tile(UI, 48, 8, 8, 8);
        tiles[UIBOX_L] = Tile(UI, 48, 4, 8, 8);
        tiles[UIBOX_TL] = Tile(UI, 48, 0, 8, 8);
        tiles[UIBOX_T] = Tile(UI, 52, 0, 8, 8);
        tiles[UIBOX_TR] = Tile(UI, 56, 0, 8, 8);
        tiles[UIBOX_R] = Tile(UI, 56, 4, 8, 8);
        tiles[UIBOX_BR] = Tile(UI, 56, 8, 8, 8);
        tiles[UIBOX_B] = Tile(UI, 52, 8, 8, 8);
        tiles[UIBOX_BL] = Tile(UI, 48, 8, 8, 8);
        tiles[UIBOX_C] = Tile(UI, 52, 4, 8, 8);

        tiles[COIN_ICO] = Tile(HUD, 264, 76, 8, 8);
        tiles[TITLE] = Tile(HUD, 40, 144, 176, 88);

        tiles[ROCK] = Tile(TILESET, 0, 16, 16, 16);
        tiles[BRICK] = Tile(TILESET, 17, 16, 16, 16);
    };
}