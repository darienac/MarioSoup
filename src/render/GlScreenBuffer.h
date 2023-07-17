#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "render/GlFramebuffer.h"

class GlScreenBuffer: public GlFramebuffer {
    private:
    GLFWwindow* window;
    public:
    GlScreenBuffer(GLFWwindow* window) {
        this->window = window;
        glfwGetFramebufferSize(window, &width, &height);
        framebufferId = 0;
    }

    virtual void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        currentFramebuffer = this;
    }
};