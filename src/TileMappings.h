#pragma once

#include "render/Texture.h"

namespace Tiles {
    const std::string textEncoding = " 0123456789abcdefghijklmnopqrstuvwxyz-X!.C/\\()";
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
        MARIO_SMA4,
        TILESET,
        HUD,
        UI,
        SMA4_TILES,
        SMA4_NPCS_ITEMS,
        SMA4_BACKGROUNDS
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
    int xCenter = 0;
    int yCenter = 0;

    // Initialization based on selection coordinates (what paint.net uses)
    Tile(TextureName texName, int sx, int sy, int sw, int sh) {
        texture = Tiles::textures[texName];
        cx = sx;
        cw = sw;
        cy = texture->getHeight() - sh - sy;
        ch = sh;
    };

    int getWidth() const {
        return cw;
    }

    int getHeight() const {
        return ch;
    }

    Tile& setXCenter(int value) {
        xCenter = value;
        return *this;
    }
    
    Tile& setYCenter(int value) {
        yCenter = value;
        return *this;
    }

    int getXCenter() const {
        return xCenter;
    }
    
    int getYCenter() const {
        return yCenter;
    }

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

    void recenterTilesX(Tile* tiles, int numTiles, int xCenter) {
        for (int i = 0 ; i < numTiles; i++) {
            tiles->setXCenter(xCenter);
            tiles++;
        }
    }

    // void recenterTilesY(Tile* tiles, int numTiles, int yCenter) {
    //     for (int i = 0 ; i < numTiles; i++) {
    //         tiles->setXCenter(yCenter);
    //         tiles++;
    //     }
    // }
};

namespace Tiles {
    enum {
        AIR,
        D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
        LA, LB, LC, LD, LE, LF, LG, LH, LI, LJ, LK, LL, LM, LN, LO, LP, LQ, LR, LS, LT, LU, LV, LW, LX, LY, LZ,
        DA, XX, EX, DO, CO, FS, BS, PL, PR,

        TEXT_CURSOR,
        MENU_OFF, MENU_ON, MENULIST_OFF, MENULIST_ON, MENULIST_TOP, MENULIST_BOTTOM, RADIO_OFF, RADIO_ON,
        SELECT_BLOCK,
        BUTTON_LEFT, BUTTON_MID, BUTTON_RIGHT, BUTTON_ON_LEFT, BUTTON_ON_MID, BUTTON_ON_RIGHT,
        TEXTINPUT_LEFT, TEXTINPUT_MID, TEXTINPUT_RIGHT,
        UIBOX_BL, UIBOX_L, UIBOX_TL, UIBOX_T, UIBOX_TR, UIBOX_R, UIBOX_BR, UIBOX_B, UIBOX_C,
        UI_X,
        UIREGION_LIGHT, UIREGION_DARK, UIREGION_BLACK,
        UICOLLAPSE, UICOLLAPSE_ARROWUP, UICOLLAPSE_ARROWDOWN, UICOLLAPSE_END,
        UIHAND_OPEN, UIHAND_CLOSED, UICRATE_OPEN, UICRATE_CLOSED,
        LEVELBOUND_BL, LEVELBOUND_L, LEVELBOUND_TL, LEVELBOUND_T, LEVELBOUND_TR, LEVELBOUND_R, LEVELBOUND_BR, LEVELBOUND_B,
        LEVEL_PLUS, LEVEL_MINUS,

        COIN_ICO, TITLE,

        BACKGROUND_OVERWORLD1_SMA4,

        MARIO_STAND_SMA4, MARIO_RUN2_SMA4, MARIO_WALK_SMA4, MARIO_RUN1_SMA4, MARIO_JUMP_SMA4, MARIO_CARRY2_SMA4, MARIO_CARRY1_SMA4, MARIO_KICK_SMA4, MARIO_CLIMB1_SMA4,
        MARIO_FRONT_SMA4, MARIO_CLIMB2_SMA4, MARIO_SLIDE_SMA4, MARIO_DIE_SMA4, MARIO_SKID_SMA4,

        SMARIO_WALK2_SMA4, SMARIO_WALK1_SMA4, SMARIO_STAND_SMA4, SMARIO_FIRE1_SMA4, SMARIO_FIRE2_SMA4, SMARIO_CLIMB_SMA4, SMARIO_SLIDE_SMA4,
        SMARIO_CARRY2_SMA4, SMARIO_CARRY1_SMA4, SMARIO_CARRY_SMA4, SMARIO_KICK_SMA4, SMARIO_JUMP_SMA4,
        SMARIO_RUN2_SMA4, SMARIO_RUN1_SMA4, SMARIO_RUN_SMA4, SMARIO_SKID_SMA4,
        SMARIO_PULL1_SMA4, SMARIO_PULL2_SMA4, SMARIO_LIFT_SMA4, SMARIO_HOLD2_SMA4, SMARIO_HOLD1_SMA4, SMARIO_HOLD_SMA4, SMARIO_SQUAT_SMA4, SMARIO_GROW_SMA4,

        SMA4_WOODF_TL, SMA4_WOODF_T, SMA4_WOODF_TR, SMA4_WOODF_L, SMA4_WOODF_M, SMA4_WOODF_R,
        SMA4_WOODP, SMA4_WOODP_TL, SMA4_WOODP_T, SMA4_WOODP_TR, SMA4_WOODP_BL, SMA4_WOODP_B, SMA4_WOODP_BR,
        SMA4_WOODP_L, SMA4_WOODP_M, SMA4_WOODP_R, SMA4_WOODP_V,
        SMA4_QBLOCK_1, SMA4_QBLOCK_2, SMA4_QBLOCK_3, SMA4_QBLOCK_4, SMA4_QBLOCK_EMPTY,
        SMA4_BRICK_1, SMA4_BRICK_2, SMA4_BRICK_3, SMA4_BRICK_4,

        SMA4_BRICK_BREAK,

        SMA4_COIN_1, SMA4_COIN_2, SMA4_COIN_3, SMA4_COIN_4,
        SMA4_ITEMCOIN_1, SMA4_ITEMCOIN_2, SMA4_ITEMCOIN_3,
        SMA4_MUSHROOM, SMA4_1UP, SMA4_FLOWER, SMA4_STAR1, SMA4_STAR2, SMA4_STAR3,
        SMA4_FEATHER_FALL, SMA4_FEATHER, SMA4_COIN_ALT, SMA4_STAR, SMA4_CHEST_CLOSED, SMA4_CHEST_OPEN,
        SMA4_FROG, SMA4_TANOOKI, SMA4_HAMMER_SUIT, SMA4_NOTEBOX, SMA4_PWING, SMA4_CAPE_FEATHER
    };

    Tile& getTile(int tile) {
        return tiles[tile];
    };

    void initTiles() {
        textures[TextureName::MARIO_SMB3] = new Texture("smb3_mario.png");
        textures[TextureName::MARIO_SMA4] = new Texture("sma4_mario.png");
        textures[TextureName::TILESET] = new Texture("smb_misc_tileset.png");
        textures[TextureName::HUD] = new Texture("smb_hud.png");
        textures[TextureName::UI] = new Texture("ui_elements.png");
        textures[TextureName::SMA4_TILES] = new Texture("sma4_misc_tileset.png");
        textures[TextureName::SMA4_NPCS_ITEMS] = new Texture("sma4_npc_items.png");
        textures[TextureName::SMA4_BACKGROUNDS] = new Texture("sma4_backgrounds.png");

        tiles[AIR] = Tile(HUD, 264, 8, 0, 0);

        initTileGrid(tiles + D0, UI, 64, 0, 8, 8, 0, 0, 16, 45);

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
        tiles[UICRATE_OPEN] = Tile(UI, 80, 48, 16, 16);
        tiles[UICRATE_CLOSED] = Tile(UI, 64, 48, 16, 16);

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

        tiles[BACKGROUND_OVERWORLD1_SMA4] = Tile(SMA4_BACKGROUNDS, 2, 2, 512, 160);

        initTileGrid(tiles + MARIO_STAND_SMA4, MARIO_SMA4, 13, 773, 16, 16, 2, 2, 9, 14);

        initTileGrid(tiles + SMARIO_WALK2_SMA4, MARIO_SMA4, 38, 20, 16, 27, 2, 2, 7, 7);
        initTileGrid(tiles + SMARIO_CARRY2_SMA4, MARIO_SMA4, 38, 49, 16, 27, 2, 2, 3, 3);
        initTileGrid(tiles + SMARIO_RUN2_SMA4, MARIO_SMA4, 38, 77, 19, 28, 2, 2, 3, 3);
        tiles[SMARIO_SKID_SMA4] = Tile(MARIO_SMA4, 102, 77, 16, 28);
        tiles[SMARIO_KICK_SMA4] = Tile(MARIO_SMA4, 91, 49, 21, 27);
        tiles[SMARIO_JUMP_SMA4] = Tile(MARIO_SMA4, 114, 49, 16, 27);
        initTileGrid(tiles + SMARIO_PULL1_SMA4, MARIO_SMA4, 76, 106, 16, 32, 2, 2, 6, 6);
        initTileGrid(tiles + SMARIO_SQUAT_SMA4, MARIO_SMA4, 206, 106, 16, 32, 2, 2, 2, 2);

        recenterTilesX(tiles + MARIO_STAND_SMA4, SMARIO_GROW_SMA4 - MARIO_STAND_SMA4 + 1, 8);

        initTileGrid(tiles + SMA4_WOODF_TL, SMA4_TILES, 290, 110, 16, 16, 2, 2, 3, 6);
        tiles[SMA4_WOODP] = Tile(SMA4_TILES, 272, 128, 16, 16);
        initTileGrid(tiles + SMA4_WOODP_TL, SMA4_TILES, 434, 254, 16, 16, 2, 2, 3, 6);
        initTileGrid(tiles + SMA4_WOODP_L, SMA4_TILES, 488, 254, 16, 16, 2, 2, 2, 4);

        initTileGrid(tiles + SMA4_QBLOCK_1, SMA4_TILES, 182, 92, 16, 16, 2, 2, 4, 4);
        tiles[SMA4_QBLOCK_EMPTY] = Tile(SMA4_TILES, 254, 128, 16, 16);
        initTileGrid(tiles + SMA4_BRICK_1, SMA4_TILES, 182, 146, 16, 16, 2, 2, 4, 4);

        tiles[SMA4_BRICK_BREAK] = Tile(SMA4_NPCS_ITEMS, 484, 120, 8, 8).setXCenter(4).setYCenter(4);

        initTileGrid(tiles + SMA4_COIN_1, SMA4_TILES, 182, 110, 16, 16, 2, 2, 4, 4);
        initTileGrid(tiles + SMA4_ITEMCOIN_1, SMA4_NPCS_ITEMS, 300, 99, 16, 16, 2, 2, 3, 3);
        initTileGrid(tiles + SMA4_MUSHROOM, SMA4_NPCS_ITEMS, 300, 189, 16, 16, 2, 2, 6, 24);
    };
}