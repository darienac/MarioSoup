#pragma once

#include "render/ImageDrawer.h"
#include "TileMappings.h"
#include "ui/ui.h"

using namespace Tiles;
using namespace Palletes;

class UIDrawer {
    private:
    ImageDrawer* drawer;
    int vOffset;

    void drawMenuItem(const char* label, int x, bool isHover) {
        int l = strlen(label);
        int tile;

        if (isHover) {
            tile = MENU_ON;
        } else {
            tile = MENU_OFF;
        }
        for (int i = x; i < l * 8 + x + 8; i += 8) {
            drawer->drawTile(tile, i, vOffset);
        }
        drawText(label, x + 4, 4 + vOffset);
    }
    
    public:
    UIDrawer(ImageDrawer& drawer, int windowHeight): drawer(&drawer) {
        updateWindowHeight(windowHeight);
    }

    void updateWindowHeight(int height) {
        vOffset = height - 16;
    }

    void drawMenuBar(MenuBar& menuBar) {
        const char** labels = menuBar.getLabels();
        MenuList* lists = menuBar.getLists();
        int numLabels = menuBar.getNumLabels();
        // drawer->setPalleteSwap(OVERWORLD_2, true);
        int x = 0;
        for (int i = 0; i < numLabels; i++) {
            bool isHover = (i == menuBar.getNumHovered());
            bool isSelected = (i == menuBar.getNumSelected());
            drawMenuItem(labels[i], x, isHover || isSelected);
            if (isSelected) {
                drawMenuList(lists[i], x, vOffset - 8);
            }
            x += strlen(labels[i]) * 8 + 8;
        }
    }

    void drawMenuList(MenuList& menuList, int x, int y) {
        MenuListButton* buttons = menuList.getButtons();
        int numButtons = menuList.getNumButtons();
        int width = menuList.getWidth();

        for (int i = 0; i < width; i++) {
            drawer->drawTile(MENULIST_TOP, x + i * 8, y + 4);
            drawer->drawTile(MENULIST_BOTTOM, x + i * 8, y - numButtons * 8);
        }
        for (int i = 0; i < numButtons; i++) {
            bool isHover = (i == menuList.getNumHovered());
            drawMenuListButton(buttons[i], x, y - 4 - i * 8, width, isHover);
        }
    }

    void drawMenuListButton(MenuListButton& button, int x, int y, int width, bool isHover) {
        drawer->setPalleteSwap(OVERWORLD_2, true);

        int tile;
        if (isHover) {
            tile = MENULIST_ON;
        } else {
            tile = MENULIST_OFF;
        }
        for (int i = 0; i < width; i++) {
            drawer->drawTile(tile, x + i * 8, y);
        }
        drawText(button.getLabel(), x + 12, y);
        if (button.getType() == UIButtonType::RADIO) {
            int tile;
            if (button.getValue().toggle) {
                tile = RADIO_ON;
            } else {
                tile = RADIO_OFF;
            }
            drawer->drawTile(tile, x + 4, y);
        }
    }

    void drawPopupWindow(PopupWindow& popup, int gameWidth, int gameHeight) {
        drawer->setPalleteSwap(OVERWORLD_2, true);

        int w = popup.getWidth();
        int h = popup.getHeight();
        int x0 = (gameWidth - w * 8) / 2;
        int y0 = (gameHeight - h * 8) / 2;
        int x1 = x0 + w * 8 - 8;
        int y1 = y0 + h * 8 - 8;

        // Draw Box
        drawer->drawTile(UIBOX_BL, x0, y0);
        drawer->drawTile(UIBOX_BR, x1, y0);
        drawer->drawTile(UIBOX_TL, x0, y1);
        drawer->drawTile(UIBOX_TR, x1, y1);
        for (int i = 1; i < w - 1; i++) {
            drawer->drawTile(UIBOX_B, x0 + i * 8, y0);
            drawer->drawTile(UIBOX_T, x0 + i * 8, y1);
        }
        for (int i = 1; i < h - 1; i++) {
            drawer->drawTile(UIBOX_L, x0, y0 + i * 8);
            drawer->drawTile(UIBOX_R, x1, y0 + i * 8);
        }
        for (int i = 1; i < w - 1; i++) {
            for (int j = 1; j < h - 1; j++) {
                drawer->drawTile(UIBOX_C, x0 + i * 8, y0 + j * 8);
            }
        }

        // Draw Label
        int t0 = (w - strlen(popup.getLabel())) / 2;
        drawText(popup.getLabel(), x0 + t0 * 8, y1 - 8);

        // Draw Elements
        IUIElement** elements = popup.getElements();
        for (int i = 0; i < popup.getNumElements(); i++) {
            drawElement(elements[i], x0, y0);
        }
    }

    void drawButton(Button& button, int x, int y) {
        int w = button.getWidth();
        drawer->drawTile(BUTTON_LEFT, x, y);
        drawer->drawTile(BUTTON_RIGHT, x + w * 8 - 8, y);
        for (int i = 1; i < w - 1; i++) {
            drawer->drawTile(BUTTON_MID, x + i * 8, y);
        }
        int xOff = (w - strlen(button.getLabel())) * 4;
        drawText(button.getLabel(), x + xOff, y + 4);
    }

    void drawText(std::string text, int x, int y) {
        drawer->setPalleteSwap(OVERWORLD_2, true);
        for (int i = 0; i < text.length(); i++) {
            int tile = AIR + textEncoding.find(text.at(i));
            drawer->drawTile(tile, x + i * 8, y);
        }
    }

    void drawElement(IUIElement* element, int xOff, int yOff) {
        int ex;
        int ey;
        element->getPosition(ex, ey);
        switch(element->getElementType()) {
            case UIElementType::BUTTON:
                drawButton(*((Button*) element), xOff + ex, yOff + ey);
                break;
        }
    }
};