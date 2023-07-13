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

	MenuBar menuBar = MenuBar(menuItems, 2);

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
		menuBar.hover(mouseX, mouseY, window.getGameHeight());
		if (window.pollMouseLeftClicked()) {
			menuBar.click();
		}

		// Render
		window.gameFramebuffer->bind();
		glClearColor((float) 0x94 / 0xFF, (float) 0x94 / 0xFF, (float) 0xFF / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.stageDrawer->drawScoreboard(7654321, 54, 5, 4, -1, fps);
		window.stageDrawer->drawTitle(0, 7654321);
        window.uiDrawer->drawMenuBar(menuBar);

		window.windowFramebuffer->bind();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.drawer->removePalleteSwap();
		window.drawer->drawLetterboxedImage(*window.gameTexture, (float) window.windowFramebuffer->getWidth() / (float) window.windowFramebuffer->getHeight());

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}