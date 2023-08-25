#pragma once

#include <vector>
#include <string.h>

#include "ui/ui.h"
#include "game/objects/GameObject.h"

class ObjectPickerGroup: public IUIElement {
    private:
    static const int maxFilteredItems = 32;

    int width = 0;
    char* filter = nullptr;

    char label[256];
    ObjectPickerGroup** groups;
    int numGroups;
    GameObject** items;
    int numItems;
    GameObject* filteredItems[maxFilteredItems];
    int numFilteredItems = 0;


    bool open = false;
    bool isHover = false;
    int hoverX = 0;
    int hoverY = 0;
    GameObject* hoveredItem = nullptr;
    GameObject** selectedItemLoc = nullptr;

    public:
    ObjectPickerGroup(const char* label, ObjectPickerGroup** groups, int numGroups, GameObject** items, int numItems): groups(groups), numGroups(numGroups), items(items), numItems(numItems) {
        if (strlen(label) > 255) {
            throw "ObjectPickerGroup label name longer than 255 characters";
        }
        strcpy(this->label, label);
    }

    void updateSearchFilter(char* filter) {
        for (int i = 0; i < numGroups; i++) {
            groups[i]->updateSearchFilter(filter);
        }
        if (filter == nullptr || strlen(filter) == 0) {
            this->filter = nullptr;
            return;
        }
        this->filter = filter;

        numFilteredItems = 0;
        for (int i = 0; i < numItems && numFilteredItems < maxFilteredItems; i++) {
            if (strstr(items[i]->getName(), filter) != nullptr) {
                filteredItems[numFilteredItems] = items[i];
                numFilteredItems++;
            }
        }
    }
    
    void setSelectedItemLoc(GameObject** selectedItemLoc) {
        this->selectedItemLoc = selectedItemLoc;
        for (int i = 0; i < numGroups; i++) {
            groups[i]->setSelectedItemLoc(selectedItemLoc);
        }
    }
    
    int getHeight() {
        ObjectPickerGroup** groups = getGroups();
        if (open) {
            int height = 32 + ((getNumItems() + 7) / 8) * 16;
            for (int i = 0; i < getNumGroups(); i++) {
                height += groups[i]->getHeight();
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
        
        ObjectPickerGroup** groups = getGroups();
        for (int i = 0; i < getNumGroups(); i++) {
            groups[i]->setWidth(width);
        }
    }
    
    int getWidth() {
        return width;
    }

    ObjectPickerGroup** getGroups() {
        return groups;
    }

    int getNumGroups() {
        return numGroups;
    }

    GameObject** getItems() {
        if (filter == nullptr) {
            return items;
        }
        return filteredItems;
    }

    int getNumItems() {
        if (filter == nullptr) {
            return numItems;
        }
        return numFilteredItems;
    }

    void selectItem(GameObject* item) {
        *selectedItemLoc = item;
    }

    GameObject* getSelectedItem() {
        return *selectedItemLoc;
    }

    GameObject* getHoveredItem() {
        return hoveredItem;
    }

    int getHoverX() {
        return hoverX;
    }

    int getHoverY() {
        return hoverY;
    }

    // Hover relative to top left corner
    bool hover(int x, int y, int gameWidth, int gameHeight) {
        hoverX = x;
        hoverY = y;
        isHover = (x >= 0 && x < width * 8 && y >= -getHeight() && y < 0);
        hoveredItem = nullptr;
        GameObject** items = getItems();

        if (isHover) {
            ObjectPickerGroup** groups = getGroups();
            int yOffset = 8;
            for (int i = 0; i < getNumGroups(); i++) {
                int height = groups[i]->getHeight();
                groups[i]->hover(hoverX, hoverY + yOffset, gameWidth, gameHeight);
                yOffset += height;
            }
            yOffset += 8;
            if (hoverY + yOffset < 0 && getNumItems() > 0 && x >= 8 && x < width * 8 - 16) {
                int hoveredItemIdx = (-(hoverY + yOffset + 1) / 16) * 8 + ((x - 8) / 16);
                if (hoveredItemIdx >= getNumItems()) {
                    hoveredItem = nullptr;
                    return isHover;
                }
                hoveredItem = items[hoveredItemIdx];
            }
        }

        return isHover;
    }

    void click() {
        if (!isHover) {
            return;
        }
        if (hoverY >= -8 && hoverY < 0) {
            setOpen(!isOpen());
            return;
        }

        ObjectPickerGroup** groups = getGroups();
        for (int i = 0; i < getNumGroups(); i++) {
            groups[i]->click();
        }

        if (hoveredItem != nullptr) {
            selectItem(hoveredItem);
        }
    }

    ~ObjectPickerGroup() {
        for (int i = 0; i < numGroups; i++) {
            delete groups[i];
        }
        delete[] groups;
        delete[] items;
    }
};

class ObjectPicker: public IUIElement {
    private:
    ObjectPickerGroup** groups;
    int numGroups;
    int width;
    int height;

    int innerHeight = 1024;
    int scrollY = 0;
    bool isHover = false;
    bool isDragScrolling = false;
    int dragScrollOffset;
    int hoverX, hoverY;
    Texture* framebufferTexture;
    GlFramebuffer* framebuffer;
    GameObject* selectedItem = nullptr;

    void limitScroll() {
        if (scrollY < 0 || innerHeight <= height * 8) {
            scrollY = 0;
        } else if (scrollY > innerHeight - height * 8) {
            scrollY = (innerHeight - height * 8);
        }
    }

    public:
    // Intervals of 8 pixels for width and height
    ObjectPicker(ObjectPickerGroup** groups, int numGroups, int width, int height): groups(groups), numGroups(numGroups), width(width), height(height) {
        framebufferTexture = new Texture(width * 8 - 4, height * 8);
        framebuffer = new GlFramebuffer(1, framebufferTexture, false);

        for (int i = 0; i < numGroups; i++) {
            groups[i]->setWidth(width);
            groups[i]->setSelectedItemLoc(&selectedItem);
        }

        updateInnerHeight();
    }

    void updateInnerHeight() {
        innerHeight = 0;
        for (int i = 0; i < numGroups; i++) {
            innerHeight += groups[i]->getHeight();
        }

        limitScroll();
    }

    void updateSearchFilter(char* filter) {
        for (int i = 0; i < numGroups; i++) {
            groups[i]->updateSearchFilter(filter);
        }
    }

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
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
        ObjectPickerGroup** groups = getGroups();
        int yOffset = 0;
        for (int i = 0; i < getNumGroups(); i++) {
            int height = groups[i]->getHeight();
            groups[i]->hover(hoverX, -(pxlHeight - hoverY - yOffset) - scrollY, gameWidth, gameHeight);
            yOffset += height;
        }

        return isHover;
    }
    void click() {
        if (isDragScrolling) {
            isDragScrolling = false;
            return;
        }

        ObjectPickerGroup** groups = getGroups();
        for (int i = 0; i < getNumGroups(); i++) {
            groups[i]->click();
        }
        updateInnerHeight();
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

    ObjectPickerGroup** getGroups() {
        return groups;
    }

    int getNumGroups() {
        return numGroups;
    }

    GlFramebuffer* getFramebuffer() {
        return framebuffer;
    }

    void getScrollBar(int& scrollBarHeight, int& scrollYMax, int& scrollBarMax, int& scrollBarY) {
        int height = getHeight() * 8;
        int innerHeight = getInnerHeight();
        int scrollY = getScrollY();

        scrollBarHeight = height * height / innerHeight;
        scrollYMax = innerHeight - height;
        scrollBarMax = height - scrollBarHeight;
        scrollBarY = scrollY * scrollBarMax / scrollYMax;
    }

    GameObject* getSelectedItem() {
        return selectedItem;
    }

    void setSelectedItem(GameObject* item) {
        selectedItem = item;
    }

    ~ObjectPicker() {
        for (int i = 0; i < numGroups; i++) {
            delete groups[i];
        }
        delete[] groups;
        delete framebuffer;
        delete framebufferTexture;
    }
};