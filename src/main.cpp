#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

#include "TileMappings.h"
#include "GlWindow.h"

const int WINDOW_WIDTH = 256;
const int WINDOW_HEIGHT = 240;

int main() {
	GlWindow window = GlWindow("Mario", WINDOW_WIDTH, WINDOW_HEIGHT);

	double lastTime = glfwGetTime();
	int fpsCounter = 0;
	int fps = 0;

	while (!glfwWindowShouldClose(window.window)) {
		double time = glfwGetTime();
		fpsCounter++;
		if (time - lastTime >= 1.0) {
			lastTime++;
			fps = fpsCounter;
			fpsCounter = 0;
		}

		window.gameFramebuffer->bind();
		glClearColor((float) 0x94 / 0xFF, (float) 0x94 / 0xFF, (float) 0xFF / 0xFF, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.stageDrawer->drawScoreboard(7654321, 54, 5, 4, -1, fps);
		window.stageDrawer->drawTitle(0, 7654321);

		window.windowFramebuffer->bind();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.drawer->removePalleteSwap();
		window.drawer->drawLetterboxedImage(*window.gameTexture, (float) window.windowFramebuffer->getWidth() / (float) window.windowFramebuffer->getHeight());

		glfwSwapBuffers(window.window);
		glfwPollEvents();
	}
}