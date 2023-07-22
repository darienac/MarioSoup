#pragma once

#include "ui/ui.h"
#include "game/GameObject.h"

class ObjectPickerGroup: public IUIElement {
    private:
    const char* label;
    int width = 0;
    ObjectPickerGroup* groups;
    int numGroups;
    GameObject* items;
    int numItems;
    bool open = false;
    bool isHover = false;
    int hoverX = 0;
    int hoverY = 0;
    int hoveredItem = -1;
    int selectedItem = -1;

    public:
    ObjectPickerGroup(const char* label, ObjectPickerGroup* groups, int numGroups, GameObject* items, int numItems): label(label), groups(groups), numGroups(numGroups), items(items), numItems(numItems) {}

    int getHeight() {
        ObjectPickerGroup* groups = getGroups();
        if (open) {
            int height = 32 + ((numItems + 7) / 8) * 16;
            for (int i = 0; i < getNumGroups(); i++) {
                height += groups[i].getHeight();
            }
            return height;
        } else {
            return 8;
        }
    }

    void setOpen(bool value) {
        open = value;
        std::printf("Open: %d\n", value);
    }

    bool isOpen() {
        return open;
    }

    const char* getLabel() {
        return label;
    }

    void setWidth(int width) {
        this->width = width;
        
        ObjectPickerGroup* groups = getGroups();
        for (int i = 0; i < getNumGroups(); i++) {
            groups[i].setWidth(width);
        }
    }
    
    int getWidth() {
        return width;
    }

    ObjectPickerGroup* getGroups() {
        return groups;
    }

    int getNumGroups() {
        return numGroups;
    }

    GameObject* getItems() {
        return items;
    }

    int getNumItems() {
        return numItems;
    }

    void selectItem(int itemNum) {
        selectedItem = itemNum;
        std::printf("Item selected: %d\n", itemNum);
    }

    int getSelectedItem() {
        return selectedItem;
    }

    // Hover relative to top left corner
    void hover(int x, int y, int gameWidth, int gameHeight) {
        hoverX = x;
        hoverY = y;
        isHover = (x >= 0 && x < width * 8 && y >= -getHeight() && y < 0);
        hoveredItem = -1;

        if (isHover) {
            ObjectPickerGroup* groups = getGroups();
            int yOffset = 8;
            for (int i = 0; i < getNumGroups(); i++) {
                int height = groups[i].getHeight();
                groups[i].hover(hoverX, hoverY + yOffset, gameWidth, gameHeight);
                yOffset += height;
            }
            yOffset += 8;
            if (hoverY + yOffset < 0 && getNumItems() > 0 && x >= 8 && x < width * 8 - 16) {
                hoveredItem = (-(hoverY + yOffset + 1) / 16) * 8 + ((x - 8) / 16);
                if (hoveredItem >= getNumItems()) {
                    hoveredItem = -1;
                }
            }
        }
    }

    void click() {
        if (!isHover) {
            return;
        }
        if (hoverY >= -8 && hoverY < 0) {
            setOpen(!isOpen());
            return;
        }

        ObjectPickerGroup* groups = getGroups();
        for (int i = 0; i < getNumGroups(); i++) {
            groups[i].click();
        }

        if (hoveredItem != -1) {
            selectItem(hoveredItem);
        }
    }
};

class ObjectPicker: public IUIElement {
    private:
    int width, height;
    ObjectPickerGroup* groups;
    int numGroups;

    int innerHeight = 1024;
    int scrollY = 0;
    bool isHover = false;
    bool isDragScrolling = false;
    int dragScrollOffset;
    int hoverX, hoverY;
    Texture* framebufferTexture;
    GlFramebuffer* framebuffer;

    void limitScroll() {
        if (scrollY < 0) {
            scrollY = 0;
        } else if (scrollY > innerHeight - height * 8) {
            scrollY = (innerHeight - height * 8);
        }
    }

    public:
    // Intervals of 8 pixels for width and height
    ObjectPicker(ObjectPickerGroup* groups, int numGroups, int width, int height): groups(groups), numGroups(numGroups), width(width), height(height) {
        framebufferTexture = new Texture(width * 8 - 4, height * 8);
        framebuffer = new GlFramebuffer(1, framebufferTexture, false);

        for (int i = 0; i < numGroups; i++) {
            groups[i].setWidth(width);
        }
    }

    void hover(int x, int y, int gameWidth, int gameHeight) {
        hoverX = x;
        hoverY = y;
        isHover = (hoverX >= 0 && hoverX < width * 8 && hoverY >= 0 && hoverY < height * 8);

        if (isDragScrolling) {
            int newScrollBarY = (height * 8 - hoverY) - dragScrollOffset;

            int scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY;
            getScrollBar(scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY);

            scrollY = newScrollBarY * scrollYMax / scrollBarMax;
            limitScroll();
        }

        int pxlHeight = height * 8;
        ObjectPickerGroup* groups = getGroups();
        int yOffset = 0;
        for (int i = 0; i < getNumGroups(); i++) {
            int height = groups[i].getHeight();
            groups[i].hover(hoverX, -(pxlHeight - hoverY - yOffset), gameWidth, gameHeight);
            yOffset += height;
        }
    }
    void click() {
        if (isDragScrolling) {
            isDragScrolling = false;
            return;
        }

        ObjectPickerGroup* groups = getGroups();
        for (int i = 0; i < getNumGroups(); i++) {
            groups[i].click();
        }
    }

    void scroll(double xOff, double yOff) {
        if (isHover) {
            scrollY += yOff * -8.0;
            limitScroll();
        }
    }

    void mouseDown() override {
        if (hoverX >= width * 8 - 4 && hoverX < width * 8 && hoverY >= 0 && hoverY < height * 8) {
            isDragScrolling = true;
            int scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY;
            getScrollBar(scrollBarHeight, scrollYMax, scrollBarMax, scrollBarY);
            if (hoverY >= height * 8 - scrollBarHeight - scrollBarY && hoverY < height * 8 - scrollBarY) {
                dragScrollOffset = (height * 8 - hoverY) - scrollBarY;
            } else {
                dragScrollOffset = 0;
            }
        }
    }

    void charInput(int codepoint) {}

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    int getInnerHeight() {
        return innerHeight;
    }

    int getScrollY() {
        return scrollY;
    }

    ObjectPickerGroup* getGroups() {
        return groups;
    }

    int getNumGroups() {
        return numGroups;
    }

    GlFramebuffer* getFramebuffer() {
        return framebuffer;
    }

    void getScrollBar(int& scrollBarHeight, int& scrollYMax, int& scrollBarMax, int& scrollBarY) {
        int width = getWidth() * 8;
        int height = getHeight() * 8;
        int innerHeight = getInnerHeight();
        int scrollY = getScrollY();

        scrollBarHeight = height * height / innerHeight;
        scrollYMax = innerHeight - height;
        scrollBarMax = height - scrollBarHeight;
        scrollBarY = scrollY * scrollBarMax / scrollYMax;
    }

    ~ObjectPicker() {
        delete framebuffer;
        delete framebufferTexture;
    }
};