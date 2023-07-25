#pragma once

#include "GlWindow.h"
#include "screens/IScreen.h"
#include "screens/ScreenManager.h"
#include "ui/ui.h"
#include "game/GameObject.h"
#include "game/GameObjectCache.h"

using namespace GameObjectCache;

namespace {
    const char* menuItems[2] = {
        "file",
        "edit"
    };
}

class LevelEditorScreen: public IScreen {
    private:
    static const int NUM_BUNDLE_ELEMENTS = 3;

    MenuListButton buttons1[2] = {
        MenuListButton("open", UIButtonType::BUTTON, nullptr),
        MenuListButton("save", UIButtonType::BUTTON, nullptr)
    };
    MenuListButton buttons2[2] = {
        MenuListButton("edit 1", UIButtonType::BUTTON, nullptr),
        MenuListButton("toggle", UIButtonType::RADIO, nullptr)
    };
    MenuList lists[2] = {
        MenuList(buttons1, 2, 8),
        MenuList(buttons2, 2, 8)
    };
    MenuBar menuBar = MenuBar(menuItems, 2, lists, 144, 0, 32);

    TextInput* searchBar;
    char searchBarBuffer[32];

    GameObject* objectList1[2] = {objects["smb:rock"], objects["smb:brick"]};
    GameObject* objectList2[2] = {objects["smb:brick"], objects["smb:rock"]};

    ObjectPickerGroup groups[2] = {
        ObjectPickerGroup("group 1", nullptr, 0, objectList1, 2),
        ObjectPickerGroup("group 2", nullptr, 0, nullptr, 0)
    };
    ObjectPickerGroup groups2[2] = {
        ObjectPickerGroup("group a", groups, 2, objectList2, 2),
        ObjectPickerGroup("group b", nullptr, 0, nullptr, 0)
    };
    ObjectPicker* picker;
    

    IUIElement* bundleElements[NUM_BUNDLE_ELEMENTS];

    UIBundle* uiBundle;

    public:
    LevelEditorScreen(GlWindow& window, ScreenManager& manager) {
        this->window = &window;
        this->manager = &manager;

        searchBar = new TextInput("search objects", 16, 30, searchBarBuffer, [](TextInput* input, char* value){
            LevelEditorScreen* screen = (LevelEditorScreen*) input->getPointer();
            screen->getObjectPicker()->updateSearchFilter(value);
        });
        searchBar->setPointer(this);
        searchBar->setPosition(8, window.getHeight() - 28);

        // Setup Tile Picker
        picker = new ObjectPicker(groups2, 2, 18, 28);
        picker->getGroups()[0].setOpen(true);

        bundleElements[0] = &menuBar;
        bundleElements[1] = searchBar;
        bundleElements[2] = picker;
        uiBundle = new UIBundle(bundleElements, NUM_BUNDLE_ELEMENTS);
    }

    void enable() {
        window->resize(400, 256);
        window->stageDrawer->setOffset(144, 0);
        window->uiEventElement = uiBundle;
    }

    void renderFrame() override {
        double mouseX;
        double mouseY;
        window->getCursorPosLetterboxed(mouseX, mouseY);
        // int windowWidth = window->getGameWidth();
        // int windowHeight = window->getGameHeight();

        glClearColor((float) 0x94 / 0xFF, (float) 0x94 / 0xFF, (float) 0xFF / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->drawer->setZPos(ImageDrawer::ZPOS_UI);

        window->uiDrawer->drawMenuBar(menuBar);

        window->uiDrawer->drawUIRegion(UIREGION_LIGHT, 0, 224, 18, 4);
        window->uiDrawer->drawTextInput(*searchBar);
        window->uiDrawer->drawObjectPicker(*picker, 0, 0);

        window->drawer->setZPos(ImageDrawer::ZPOS_GAME_TILES);

        window->stageDrawer->drawScoreboard(7654321, 54, 5, 4, -1, manager->getFPS());
		window->stageDrawer->drawTitle(0, 7654321);
    }

    ObjectPicker* getObjectPicker() {
        return picker;
    }

    ~LevelEditorScreen() {
        delete searchBar;
        delete picker;

        delete uiBundle;
    }
};

/*
Demo UI Code:

window.getCursorPosLetterboxed(mouseX, mouseY);
// std::printf("Mouse pos x: %f, y: %f\n", mouseX, mouseY);
menuBar.hover(mouseX, mouseY, window.getGameWidth(), window.getGameHeight());
popup.hover(mouseX, mouseY, window.getGameWidth(), window.getGameHeight());
if (window.pollMouseLeftClicked()) {
    menuBar.click();
    popup.click();
}

MenuListButton buttons1[] = {
    MenuListButton("open", UIButtonType::BUTTON, nullptr),
    MenuListButton("save", UIButtonType::BUTTON, nullptr)
};
MenuListButton buttons2[] = {
    MenuListButton("edit 1", UIButtonType::BUTTON, nullptr),
    MenuListButton("toggle", UIButtonType::RADIO, nullptr)
};
MenuList lists[] = {
    MenuList(buttons1, 2, 8),
    MenuList(buttons2, 2, 8)
};
MenuBar menuBar = MenuBar(menuItems, 2, lists);

Button button1 = Button("yes", 8, [](Button* button, UIButtonValue& value) {
    std::cout << "Button 1 pressed\n";
});
button1.setPosition(8, 8);
Button button2 = Button("no", 8, [](Button* button, UIButtonValue& value) {
    std::cout << "Button 2 pressed\n";
});
button2.setPosition(80, 8);
char textInputBuffer[32];
textInputBuffer[0] = 0;
TextInput textInput = TextInput("test123", 8, 8, textInputBuffer, [](TextInput* input, char* value) {
    std::cout << "Text Input pressed\n";
});
textInput.setPosition(8, 32);
IUIElement* elements[] = {&button1, &button2, &textInput};
PopupWindow popup = PopupWindow("rival rush. accept", 20, 12, elements, 3);

// Resize window
glfwGetWindowSize(window.window, &windowWidth, &windowHeight);
if (oldWidth != windowWidth || oldHeight != windowHeight) {
    // window.resize(windowWidth / 2, windowHeight / 2);
}

*/