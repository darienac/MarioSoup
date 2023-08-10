#pragma once

#include "render/Texture.h"

namespace Tiles {
    const std::string textEncoding = " 0123456789abcdefghijklmnopqrstuvwxyz-X!.C/\\";
    Texture* textures[64];
};

namespace Palletes {
    unsigned char BLACK[16] = {
        0, 0, 0, 0,
        0, 0, 0, 255,
        0, 0, 0, 255,
        0, 0, 0, 255
    };

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
        MARIO_SMB3,
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

    void initTileGrid(Tile* tiles, TextureName texName, int x0, int y0, int w, int h, int paddingX, int paddingY, int columns, int amount) {
        while (true) {
            int x = x0;
            for (int i = 0; i < columns; i++) {
                *tiles = Tile(texName, x, y0, w, h);
                amount--;
                if (amount == 0) {
                    return;
                }
                tiles++;
                x += w + paddingX;
            }
            y0 += h + paddingY;
        }
    }
};

namespace Tiles {
    enum {
        AIR,
        D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
        LA, LB, LC, LD, LE, LF, LG, LH, LI, LJ, LK, LL, LM, LN, LO, LP, LQ, LR, LS, LT, LU, LV, LW, LX, LY, LZ,
        DA, XX, EX, DO, CO, FS, BS,

        TEXT_CURSOR,
        MENU_OFF, MENU_ON, MENULIST_OFF, MENULIST_ON, MENULIST_TOP, MENULIST_BOTTOM, RADIO_OFF, RADIO_ON,
        SELECT_BLOCK,
        BUTTON_LEFT, BUTTON_MID, BUTTON_RIGHT, BUTTON_ON_LEFT, BUTTON_ON_MID, BUTTON_ON_RIGHT,
        TEXTINPUT_LEFT, TEXTINPUT_MID, TEXTINPUT_RIGHT,
        UIBOX_BL, UIBOX_L, UIBOX_TL, UIBOX_T, UIBOX_TR, UIBOX_R, UIBOX_BR, UIBOX_B, UIBOX_C,
        UI_X,
        UIREGION_LIGHT, UIREGION_DARK, UIREGION_BLACK,
        UICOLLAPSE, UICOLLAPSE_ARROWUP, UICOLLAPSE_ARROWDOWN, UICOLLAPSE_END,
        UIHAND_OPEN, UIHAND_CLOSED,
        LEVELBOUND_BL, LEVELBOUND_L, LEVELBOUND_TL, LEVELBOUND_T, LEVELBOUND_TR, LEVELBOUND_R, LEVELBOUND_BR, LEVELBOUND_B,
        LEVEL_PLUS, LEVEL_MINUS,

        COIN_ICO, TITLE,

        MARIO_STAND_SMB3, MARIO_WALK_SMB3, MARIO_JUMP_SMB3,
        MARIO_RUN1_SMB3, MARIO_RUN2_SMB3, MARIO_VAULT_SMB3,
        MARIO_SKID_SMB3, MARIO_FRONT_SMB3, MARIO_GRAB1_SMB3, MARIO_GRAB2_SMB3, MARIO_KICK_SMB3, MARIO_SLIDE_SMB3,
        MARIO_SWIM1_SMB3, MARIO_SWIM2_SMB3, MARIO_SWIM3_SMB3, MARIO_SWIM4_SMB3,
        MARIO_CLIMB_SMB3, MARIO_DIE_SMB3,

        SMB_ROCK, SMB_BRICK, SMB_FENCE
    };

    Tile& getTile(int tile) {
        return tiles[tile];
    };

    void initTiles() {
        textures[TextureName::MARIO_SMB3] = new Texture("smb3_mario.png");
        textures[TextureName::TILESET] = new Texture("smb_misc_tileset.png");
        textures[TextureName::HUD] = new Texture("smb_hud.png");
        textures[TextureName::UI] = new Texture("ui_elements.png");

        tiles[AIR] = Tile(HUD, 264, 8, 0, 0);

        initTileGrid(tiles + D0, UI, 64, 0, 8, 8, 0, 0, 16, 43);

        tiles[TEXT_CURSOR] = Tile(UI, 8, 24, 8, 8);
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
        tiles[TEXTINPUT_LEFT] = Tile(UI, 16, 16, 8, 16);
        tiles[TEXTINPUT_MID] = Tile(UI, 20, 16, 8, 16);
        tiles[TEXTINPUT_RIGHT] = Tile(UI, 24, 16, 8, 16);
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
        tiles[UI_X] = Tile(UI, 32, 32, 8, 8);
        tiles[UIREGION_LIGHT] = Tile(UI, 0, 0, 8, 8);
        tiles[UIREGION_DARK] = Tile(UI, 8, 0, 8, 8);
        tiles[UIREGION_BLACK] = Tile(UI, 48, 16, 8, 8);
        tiles[UICOLLAPSE] = Tile(UI, 0, 32, 16, 8);
        tiles[UICOLLAPSE_ARROWUP] = Tile(UI, 0, 40, 16, 8);
        tiles[UICOLLAPSE_ARROWDOWN] = Tile(UI, 0, 48, 16, 8);
        tiles[UICOLLAPSE_END] = Tile(UI, 0, 56, 16, 8);
        tiles[UIHAND_OPEN] = Tile(UI, 48, 48, 16, 16);
        tiles[UIHAND_CLOSED] = Tile(UI, 48, 32, 16, 16);

        tiles[LEVELBOUND_BL] = Tile(UI, 16, 40, 8, 8);
        tiles[LEVELBOUND_L] = Tile(UI, 16, 36, 8, 8);
        tiles[LEVELBOUND_TL] = Tile(UI, 16, 32, 8, 8);
        tiles[LEVELBOUND_T] = Tile(UI, 20, 32, 8, 8);
        tiles[LEVELBOUND_TR] = Tile(UI, 24, 32, 8, 8);
        tiles[LEVELBOUND_R] = Tile(UI, 24, 36, 8, 8);
        tiles[LEVELBOUND_BR] = Tile(UI, 24, 40, 8, 8);
        tiles[LEVELBOUND_B] = Tile(UI, 20, 40, 8, 8);

        tiles[LEVEL_PLUS] = Tile(UI, 16, 48, 16, 16);
        tiles[LEVEL_MINUS] = Tile(UI, 32, 48, 16, 16);

        tiles[COIN_ICO] = Tile(HUD, 264, 76, 8, 8);
        tiles[TITLE] = Tile(HUD, 40, 144, 176, 88);

        initTileGrid(tiles + MARIO_STAND_SMB3, MARIO_SMB3, 1, 16, 16, 16, 2, 0, 18, 18);

        tiles[SMB_ROCK] = Tile(TILESET, 0, 16, 16, 16);
        tiles[SMB_BRICK] = Tile(TILESET, 17, 16, 16, 16);
        tiles[SMB_FENCE] = Tile(TILESET, 34, 67, 16, 16);
    };
}