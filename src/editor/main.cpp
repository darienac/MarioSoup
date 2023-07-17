#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

#include "TileMappings.h"
#include "GlWindow.h"
#include "ui/MenuBar.h"

const int WINDOW_WIDTH = 256;
const int WINDOW_HEIGHT = 240;

int main() {
	GlWindow window = GlWindow("Level Editor", WINDOW_WIDTH, WINDOW_HEIGHT);

	double lastTime = glfwGetTime();
	int fpsCounter = 0;
	int fps = 0;

    const char* menuItems[] = {
        "file",
        "edit"
    };

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

	window.onCharElement = &textInput;

	int windowWidth = 0;
	int windowHeight = 0;
	while (!glfwWindowShouldClose(window.window)) {
		double time = glfwGetTime();
		fpsCounter++;
		if (time - lastTime >= 1.0) {
			lastTime++;
			fps = fpsCounter;
			fpsCounter = 0;
		}

		int oldWidth = windowWidth;
		int oldHeight = windowHeight;
		glfwGetWindowSize(window.window, &windowWidth, &windowHeight);
		if (oldWidth != windowWidth || oldHeight != windowHeight) {
			// window.resize(windowWidth / 2, windowHeight / 2);
		}

		double mouseX;
		double mouseY;
		window.getCursorPosLetterboxed(mouseX, mouseY);
		// std::printf("Mouse pos x: %f, y: %f\n", mouseX, mouseY);
		menuBar.hover(mouseX, mouseY, window.getGameWidth(), window.getGameHeight());
		popup.hover(mouseX, mouseY, window.getGameWidth(), window.getGameHeight());
		if (window.pollMouseLeftClicked()) {
			menuBar.click();
			popup.click();
		}

		// Render
		window.gameFramebuffer->bind();
		glClearColor((float) 0x94 / 0xFF, (float) 0x94 / 0xFF, (float) 0xFF / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.stageDrawer->drawScoreboard(7654321, 54, 5, 4, -1, fps);
		window.stageDrawer->drawTitle(0, 7654321);
        window.uiDrawer->drawMenuBar(menuBar);
		window.uiDrawer->drawPopupWindow(popup, window.getGameWidth(), window.getGameHeight());

		window.windowFramebuffer->bind();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.drawer->removePalleteSwap();
		window.drawer->drawLetterboxedImage(*window.gameTexture, (float) window.windowFramebuffer->getWidth() / (float) window.windowFramebuffer->getHeight());

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}