#pragma once

#include "GlWindow.h"
#include "screens/IScreen.h"

class ScreenManager {
    private:
    GlWindow* window;
    IScreen* screen = nullptr;

    public:
    ScreenManager(GlWindow& window): window(&window) {}

    void run() {
        double lastTime = glfwGetTime();
        int fpsCounter = 0;
        int fps = 0;

        while (!glfwWindowShouldClose(window->window)) {
            double time = glfwGetTime();
            fpsCounter++;
            if (time - lastTime >= 1.0) {
                lastTime++;
                fps = fpsCounter;
                fpsCounter = 0;
            }

            window->gameFramebuffer->bind();

            // Use the screen
            screen->renderFrame();

            window->windowFramebuffer->bind();
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            window->drawer->removePalleteSwap();
            window->drawer->drawLetterboxedImage(*window->gameTexture, (float) window->windowFramebuffer->getWidth() / (float) window->windowFramebuffer->getHeight());

            glfwSwapBuffers(window->window);
            glfwPollEvents();
        }
    }

    void setScreen(IScreen* screen) {
        if (this->screen != nullptr) {
            this->screen->disable();
        }
        this->screen = screen;
        screen->enable();
    }
};