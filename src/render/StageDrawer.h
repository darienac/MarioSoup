#pragma once

#include "render/ImageDrawer.h"
#include "TileMappings.h"

using namespace Tiles;
using namespace Palletes;

namespace {
    const int SCOREBOARD_WIDTH = 32;
    const int SCOREBOARD_HEIGHT = 4;
    const int SCOREBOARD_SIZE = SCOREBOARD_WIDTH * SCOREBOARD_HEIGHT;

    const int TITLE_WIDTH = 32;
    const int TITLE_HEIGHT = 10;
    const int TITLE_SIZE = TITLE_WIDTH * TITLE_HEIGHT;
}

class StageDrawer {
    private:
    int scoreboard[SCOREBOARD_SIZE];
    int title_screen[TITLE_SIZE];

    int xOff = 0;
    int yOff = 0;

    unsigned char* title_palletes1[TITLE_SIZE];
    unsigned char* title_palletes2[TITLE_SIZE];
    ImageDrawer* drawer;
    
    void drawGrid(int tiles[], int width, int height, int x, int y, int dx, int dy) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                drawer->drawTile(tiles[i * width + j], x + j * dx + xOff, y + i * dy + yOff);
            }
        }
    }

    void drawGridWithPalletes(int tiles[], unsigned char* palletes1[], unsigned char* palletes2[], int width, int height, int x, int y, int dx, int dy) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                drawer->setPalleteSwap(palletes1[i * width + j], palletes2[i * width + j], true);
                drawer->drawTile(tiles[i * width + j], x + j * dx + xOff, y + i * dy + yOff);
            }
        }
    }
    
    public:
    StageDrawer(ImageDrawer& drawer): drawer(&drawer) {
        fillGrid(scoreboard, SCOREBOARD_SIZE, AIR);
        addText(scoreboard, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT, "mario", 3, 1);
        addText(scoreboard, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT, "world", 18, 1);
        addText(scoreboard, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT, "time", 25, 1);
        addText(scoreboard, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT, "fps-", 0, 3);
        // addText(scoreboard, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT, "heat death of the universe", 3, 0);

        scoreboard[11] = COIN_ICO;
        scoreboard[12] = XX;
        scoreboard[20] = DA;

        fillGrid(title_screen, TITLE_SIZE, AIR);
        addText(title_screen, TITLE_WIDTH, TITLE_HEIGHT, "C1985 nintendo", 13, 8);
        addText(title_screen, TITLE_WIDTH, TITLE_HEIGHT, "1 player game", 11, 5);
        addText(title_screen, TITLE_WIDTH, TITLE_HEIGHT, "2 player game", 11, 3);
        addText(title_screen, TITLE_WIDTH, TITLE_HEIGHT, "top-", 12, 0);

        fillPalletes(title_palletes1, TITLE_SIZE, OVERWORLD_2);
        fillPalletes(title_palletes2, TITLE_SIZE, OVERWORLD_2);
        fillPalletes(title_palletes2, TITLE_WIDTH, 0, 8, TITLE_WIDTH, 1, OVERWORLD_1);

        title_screen[9 * TITLE_WIDTH + 5] = TITLE;
    }

    void setOffset(int x, int y) {
        xOff = x;
        yOff = y;
    }

    void fillGrid(int tiles[], int size, int tile) {
        for (int i = 0; i < size; i++) {
            tiles[i] = tile;
        }
    }

    void fillPalletes(unsigned char* palletes[], int size, unsigned char pallete[16]) {
        for (int i = 0; i < size; i++) {
            palletes[i] = pallete;
        }
    }

    void fillPalletes(unsigned char* palletes[], int width, int x, int y, int w, int h, unsigned char pallete[16]) {
        for (int i = y; i < h + y; i++) {
            for (int j = x; j < w + x; j++) {
                palletes[i * width + j] = pallete;
            }
        }
    }

    void addText(int tiles[], int width, int height, std::string text, int x, int y) {
        for (size_t i = 0; i < text.length(); i++) {
            int tile = AIR + textEncoding.find(text.at(i));
            tiles[y * width + x + i] = tile;
        }
    }

    void addScore(int tiles[], int width, int height, int score, int x, int y) {
        int digit;
        if (score < 1000000) {
            scoreboard[width * y + x] = AIR;
            x++;
            digit = 100000;
        } else {
            digit = 1000000;
        }
        for (; digit > 0; digit /= 10) {
            int placeValue = score / digit;
            score -= placeValue * digit;
            tiles[width * y + x] = D0 + placeValue;
            x++;
        }
    }

    void drawScoreboard(int score, int coins, char world, char level, int timer, int fps) {
        drawer->setPalleteSwap(OVERWORLD_2, true);
        // FPS Counter:
        scoreboard[SCOREBOARD_WIDTH * 3 + 4] = D0 + fps / 100;
        scoreboard[SCOREBOARD_WIDTH * 3 + 5] = D0 + (fps % 100) / 10;
        scoreboard[SCOREBOARD_WIDTH * 3 + 6] = D0 + (fps % 10);

        scoreboard[13] = D0 + (coins % 100) / 10;
        scoreboard[14] = D0 + (coins % 10);

        scoreboard[19] = D0 + world;
        scoreboard[21] = D0 + level;

        if (timer >= 0) {
            scoreboard[26] = D0 + (timer % 1000) / 100;
            scoreboard[27] = D0 + (timer % 100) / 10;
            scoreboard[28] = D0 + (timer % 10);
        } else {
            scoreboard[26] = AIR;
            scoreboard[27] = AIR;
            scoreboard[28] = AIR;
        }

        int x;
        int digit;
        if (score < 1000000) {
            x = 3;
            scoreboard[2] = AIR;
            digit = 100000;
        } else {
            x = 2;
            digit = 1000000;
        }
        for (; digit > 0; digit /= 10) {
            int placeValue = score / digit;
            score -= placeValue * digit;
            scoreboard[x] = D0 + placeValue;
            x++;
        }
        drawGrid(scoreboard, SCOREBOARD_WIDTH, SCOREBOARD_HEIGHT, 0, 208, 8, 8);
    }

    void drawTitle(int scrollX, int highScore) {
        addScore(title_screen, TITLE_WIDTH, TITLE_HEIGHT, highScore, 16, 0);
        drawGridWithPalletes(title_screen, title_palletes1, title_palletes2, TITLE_WIDTH, TITLE_HEIGHT, scrollX, 48, 8, 8);
    }

    void drawLevel(int tiles[], int width, int height, int scrollX) {
        drawGrid(tiles, width, height, scrollX, 0, 16, 16);
    }
};