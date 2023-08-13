#pragma once

#include "GlWindow.h"
#include "audio/AudioManager.h"
#include "screens/IScreen.h"

class ScreenManager {
    private:
    const double TICK_RATE = 60.0;
    
    AudioManager* audio;
    AudioSource* source;
    AudioBuffer* buffer;

    GlWindow* window;
    IScreen* screen = nullptr;
    int fps;

    public:
    ScreenManager(GlWindow& window): window(&window) {
        audio = new AudioManager();
        source = new AudioSource(false, true);
        buffer = new AudioBuffer("test.ogg");
        source->playBuffer(*buffer);
    }

    void run() {
        double lastTime = glfwGetTime();
        double lastTickTime = lastTime;
        int fpsCounter = 0;
        fps = 0;
        int frameNum = 0;

        while (!glfwWindowShouldClose(window->window)) {
            frameNum++;
            double time = glfwGetTime();
            fpsCounter++;
            if (time - lastTime >= 1.0) {
                lastTime++;
                fps = fpsCounter;
                fpsCounter = 0;
            }

            if (time - lastTickTime >= (1.0 / TICK_RATE)) {
                int ticks = (time - lastTickTime) * TICK_RATE;
                lastTickTime += ticks / TICK_RATE;
                for (int i = 0; i < ticks; i++) {
                    screen->tick();
                }
            }

            // Audio processing
            source->update();

            window->gameFramebuffer->bind();

            // Use the screen
            screen->renderFrame();

            window->windowFramebuffer->bind();
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            window->drawer->removePalleteSwap();
            window->drawer->drawLetterboxedImage(*window->gameTexture, (float) window->windowFramebuffer->getWidth() / (float) window->windowFramebuffer->getHeight());

            glfwSwapBuffers(window->window);
            glFinish();
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

    int getFPS() {
        return fps;
    }

    ~ScreenManager() {
        delete audio;
        delete source;
        delete buffer;
    }
};