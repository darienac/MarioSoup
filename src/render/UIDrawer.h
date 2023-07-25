#pragma once

#include "render/ImageDrawer.h"
#include "TileMappings.h"
#include "ui/ui.h"
#include "render/GlFramebuffer.h"

using namespace Tiles;
using namespace Palletes;

class UIDrawer {
    private:
    ImageDrawer* drawer;
    int vOffset;
    int windowWidth;
    int windowHeight;

    void drawMenuItem(const char* label, int x, int y, bool isHover) {
        drawer->setPalleteSwap(OVERWORLD_2, true);

        int l = strlen(label);
        int tile;

        if (isHover) {
            tile = MENU_ON;
        } else {
            tile = MENU_OFF;
        }
        for (int i = x; i < l * 8 + x + 8; i += 8) {
            drawer->drawTile(tile, i, vOffset + y);
        }
        drawText(label, x + 4, 4 + vOffset);
    }
    
    public:
    UIDrawer(ImageDrawer& drawer, int windowWidth, int windowHeight): drawer(&drawer) {
        updateWindowWidth(windowWidth);
        updateWindowHeight(windowHeight);
    }

    void drawUIRegion(int tile, int x, int y, int w, int h) {
        drawer->drawTileStretched(tile, x, y, w * 8, h * 8);
    }

    void drawUIRegionPixels(int tile, int x, int y, int w, int h) {
        drawer->drawTileStretched(tile, x, y, w, h);
    }

    void updateWindowWidth(int width) {
        windowWidth = width;
    }

    void updateWindowHeight(int height) {
        vOffset = height - 16;
        windowHeight = height;
    }

    void drawMenuBar(MenuBar& menuBar) {
        const char** labels = menuBar.getLabels();
        MenuList* lists = menuBar.getLists();
        int numLabels = menuBar.getNumLabels();
        // drawer->setPalleteSwap(OVERWORLD_2, true);
        int x = 0;
        int barX;
        int barY;
        int tilesDrawn = 0;
        menuBar.getPosition(barX, barY);
        for (int i = 0; i < numLabels; i++) {
            bool isHover = (i == menuBar.getNumHovered());
            bool isSelected = (i == menuBar.getNumSelected());
            drawMenuItem(labels[i], x + barX, barY, isHover || isSelected);
            if (isSelected) {
                drawMenuList(lists[i], x + barX, vOffset - 8 + barY);
            }
            x += strlen(labels[i]) * 8 + 8;
            tilesDrawn += strlen(labels[i]) + 1;
        }
        for (int i = 0; i < (menuBar.getWidth() - tilesDrawn); i++) {
            drawer->drawTile(MENU_OFF, x + barX + i * 8, vOffset + barY);
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
        drawer->setPalleteSwap(OVERWORLD_2, true);
        
        int w = button.getWidth();
        bool hover = button.isHovered();
        if (hover) {
            drawer->drawTile(BUTTON_ON_LEFT, x, y);
            drawer->drawTile(BUTTON_ON_RIGHT, x + w * 8 - 8, y);
        } else {
            drawer->drawTile(BUTTON_LEFT, x, y);
            drawer->drawTile(BUTTON_RIGHT, x + w * 8 - 8, y);
        }
        for (int i = 1; i < w - 1; i++) {
            if (hover) {
                drawer->drawTile(BUTTON_ON_MID, x + i * 8, y);
            } else {
                drawer->drawTile(BUTTON_MID, x + i * 8, y);
            }
        }
        int xOff = (w - strlen(button.getLabel())) * 4;
        drawText(button.getLabel(), x + xOff, y + 4);
    }

    void drawTextInput(TextInput& textInput) {
        int ex;
        int ey;
        textInput.getPosition(ex, ey);
        drawTextInput(textInput, ex, ey);
    }

    void drawTextInput(TextInput& textInput, int x, int y) {
        drawer->setPalleteSwap(OVERWORLD_2, true);
        
        int w = textInput.getWidth();
        drawer->drawTile(TEXTINPUT_LEFT, x, y);
        drawer->drawTile(TEXTINPUT_RIGHT, x + w * 8 - 8, y);
        for (int i = 1; i < w - 1; i++) {
            drawer->drawTile(TEXTINPUT_MID, x + i * 8, y);
        }

        drawText(textInput.getLabel(), x, y + 16);

        drawer->setPalleteSwap(OVERWORLD_2, BLACK, true);
        GlFramebuffer* oldFB = GlFramebuffer::getCurrentFramebuffer();
        textInput.getFramebuffer()->bind();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawer->resizeToFramebuffer();
        drawCharBuffer(textInput.getTextBuffer(), textInput.getTextLength(), 3 + textInput.getScrollX(), 3);
        if (textInput.isSelected()) {
            drawer->drawTile(TEXT_CURSOR, 3 + 8 * textInput.getCursorPos() + textInput.getScrollX(), 2);
        }

        oldFB->bind();
        drawer->resizeToFramebuffer();

        drawer->drawImage(*textInput.getFramebuffer()->getTextures(), x + 1, y + 1);
    }

    void drawGameObjectPreview(GameObject& object, int x, int y, bool selected) {
        drawer->drawTile(object.getTilePreview(), x, y);
        if (selected) {
            glEnable(GL_COLOR_LOGIC_OP);
            glLogicOp(GL_EQUIV);
            drawer->drawTile(SELECT_BLOCK, x, y);
            glLogicOp(GL_COPY);
            glDisable(GL_COLOR_LOGIC_OP);
        }
    }

    // Draws relative to x and y of cursor
    void drawTextHint(const char* text, int x, int y) {
        float oldZPos = drawer->getZPos();
        drawer->setZPos(ImageDrawer::ZPOS_TEXTHINT);

        int length = strlen(text);
        int width = length * 8 + 4;
        int height = 12;
        y -= height;
        if (x < 0) {
            x = 0;
        } else if (x > windowWidth - width) {
            x = windowWidth - width;
        }
        if (y < 0) {
            y = 0;
        } else if (y >= windowHeight - height) {
            y = windowHeight - height;
        }
        drawer->drawTileStretched(UIREGION_BLACK, x, y, width, height);
        drawText(text, x + 2, y + 2);

        drawer->setZPos(oldZPos);
    }

    void drawObjectPickerGroup(ObjectPickerGroup& group, int x, int y) {
        int width = group.getWidth() * 8 - 4;
        drawer->drawTileStretched(UICOLLAPSE, x + 16, y - 8, width - 16, 8);
        if (group.isOpen()) {
            drawer->drawTile(UICOLLAPSE_ARROWUP, x, y - 8);
        } else {
            drawer->drawTile(UICOLLAPSE_ARROWDOWN, x, y - 8);
        }
        drawText(group.getLabel(), x + 16, y - 8);
        if (!group.isOpen()) {
            return;
        }

        int yOffset = 8;
        ObjectPickerGroup* groups = group.getGroups();
        for (int i = 0; i < group.getNumGroups(); i++) {
            drawObjectPickerGroup(groups[i], x, y - yOffset);
            yOffset += groups[i].getHeight();
        }

        GameObject** items = group.getItems();
        GameObject* selectedItem = group.getSelectedItem();
        GameObject* hoveredItem = group.getHoveredItem();
        for (int i = 0; i < group.getNumItems(); i++) {
            drawGameObjectPreview(*items[i], x + (i % 8) * 16 + 8, y - yOffset - (i / 8) * 16 - 24, items[i] == selectedItem);
        }
        if (hoveredItem != nullptr) {
            drawTextHint(hoveredItem->getName(), group.getHoverX() + x, group.getHoverY() + y);
        }
        yOffset += 16 * ((group.getNumItems() + 7) / 8);

        drawer->drawTileStretched(UICOLLAPSE_END, x, y - yOffset - 24, width, 8);
    }

    void drawObjectPicker(ObjectPicker& picker, int x, int y) {
        drawer->setPalleteSwap(OVERWORLD_2, true);

        int scrollY = picker.getScrollY();
        int pxlWidth = picker.getWidth() * 8;
        int pxlHeight = picker.getHeight() * 8;


        drawUIRegion(UIREGION_DARK, x, y, picker.getWidth(), picker.getHeight());

        GlFramebuffer* oldFB = GlFramebuffer::getCurrentFramebuffer();
        picker.getFramebuffer()->bind();
        drawer->resizeToFramebuffer();
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render inner section
        // drawer->drawTile(Tiles::ROCK, 0, pxlHeight + scrollY - 16);
        int yOffset = 0;
        ObjectPickerGroup* groups = picker.getGroups();
        for (int i = 0; i < picker.getNumGroups(); i++) {
            drawObjectPickerGroup(groups[i], x, y + pxlHeight + scrollY - yOffset);
            yOffset += groups[i].getHeight();
        }

        // Return to previous framebuffer
        oldFB->bind();
        drawer->resizeToFramebuffer();

        int scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY;
        picker.getScrollBar(scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY);

        drawer->drawImage(*picker.getFramebuffer()->getTextures(), x, y);
        drawUIRegionPixels(UIREGION_LIGHT, x + pxlWidth - 4, y + pxlHeight - scrollBarHeight - scrollBarY, 4, scrollBarHeight);
    }

    void drawCharBuffer(char* buffer, int length, int x, int y) {
        for (int i = 0; i < length; i++) {
            if (buffer[i] == 0) {
                break;
            }
            int tile = AIR + textEncoding.find(buffer[i]);
            drawer->drawTile(tile, x + i * 8, y);
        }
    }

    void drawText(std::string text, int x, int y) {
        for (size_t i = 0; i < text.length(); i++) {
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
            case UIElementType::TEXT_INPUT:
                drawTextInput(*((TextInput*) element), xOff + ex, yOff + ey);
                break;
            default:
                break;
        }
    }
};