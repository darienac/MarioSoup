#pragma once

class GlWindow;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "render/shader/ShaderProgram.h"
#include "render/shader/ImageShader.h"
#include "render/ImageDrawer.h"
#include "render/GlFramebuffer.h"
#include "render/GlScreenBuffer.h"
#include "render/StageDrawer.h"
#include "render/LevelDrawer.h"
#include "render/UIDrawer.h"

class GlWindow {
    private:
    bool mouseLeftClicked = false;
    bool fullscreen = false;
    int oldWindowX;
    int oldWindowY;
    int oldWindowW;
    int oldWindowH;
    bool keys[348] = {false};

    void setCallbacks() {
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            GlWindow* glWindow = (GlWindow*) glfwGetWindowUserPointer(window);
            glWindow->setKey(key, action != GLFW_RELEASE);

            if ((key == GLFW_KEY_BACKSPACE || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT || key == GLFW_KEY_DELETE || key == GLFW_KEY_ESCAPE) && action != GLFW_RELEASE) {
                GlWindow* glWindow = ((GlWindow*) glfwGetWindowUserPointer(window));
                if (glWindow->uiEventElement != nullptr) {
                    glWindow->uiEventElement->charInput(key);
                }
            } else if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
                if (glWindow->isFullscreen()) {
                    glWindow->disableFullscreen();
                } else {
                    glWindow->enableFullscreen();
                }
            }
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
            GlWindow* glWindow = (GlWindow*) glfwGetWindowUserPointer(window);
            if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_REPEAT) {
                glWindow->mouseLeftPressed = (action == GLFW_PRESS);
                if (action == GLFW_PRESS) {
                    glWindow->uiEventElement->mouseDown();
                } else {
                    glWindow->uiEventElement->click();
                    glWindow->mouseLeftDrag = false;
                }
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action != GLFW_REPEAT) {
                if (action == GLFW_PRESS) {
                    glWindow->uiEventElement->mouseRightDown();
                } else {
                    glWindow->uiEventElement->clickRight();
                }
            } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action != GLFW_REPEAT) {
                if (action == GLFW_PRESS) {
                    glWindow->uiEventElement->mouseMiddleDown();
                } else {
                    glWindow->uiEventElement->clickMiddle();
                }
            }
        });
        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOff, double yOff) {
            GlWindow* glWindow = (GlWindow*) glfwGetWindowUserPointer(window);
            glWindow->uiEventElement->scroll(xOff, yOff);
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
            GlWindow* glWindow = (GlWindow*) glfwGetWindowUserPointer(window);
            glWindow->mouseX = xpos;
            glWindow->mouseY = ypos;

            double mouseX;
            double mouseY;
            glWindow->getCursorPosLetterboxed(mouseX, mouseY);
            glWindow->uiEventElement->hover(mouseX - glWindow->uiEventElement->getX(), mouseY - glWindow->uiEventElement->getY(), glWindow->getGameWidth(), glWindow->getGameHeight());
            if (glWindow->mouseLeftPressed) {
                glWindow->mouseLeftDrag = true;
            }
        });
        glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
            GlWindow* glWindow = ((GlWindow*) glfwGetWindowUserPointer(window));
            if (glWindow->uiEventElement != nullptr) {
                glWindow->uiEventElement->charInput(codepoint);
            }
        });
    }

    public:
    GLFWwindow* window;
    ImageDrawer* drawer;
    StageDrawer* stageDrawer;
    LevelDrawer* levelDrawer;
    UIDrawer* uiDrawer;
    ImageShader* imageShader;

    Texture* gameTexture;
    GlFramebuffer* gameFramebuffer;
    GlScreenBuffer* windowFramebuffer;

    IUIElement* uiEventElement = nullptr;

    double mouseX = 0.0;
    double mouseY = 0.0;
    bool mouseLeftPressed = false;
    bool mouseLeftDrag = false;

    GlWindow(std::string windowName, int windowWidth, int windowHeight) {
        // Init GLFW
        if (!glfwInit()) {
            std::cout << "GLFW Not Initialized";
        }

        glfwSetErrorCallback([](int error, const char* description) {
            std::cerr << "Error: " << description << std::endl;
        });

        // Set window hints for new window
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Make invisible until window is properly configured
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        // glfwWindowHint(GLFW_SAMPLES, 4);
        // glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window
        window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), NULL, NULL);
        // Having problems with 1 pixel errors with this
        // glfwSetWindowAspectRatio(window, 256, 224);

        // Set window callbacks
        glfwSetWindowUserPointer(window, this);
        setCallbacks();

        // Center the window
        const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window, (videoMode->width - windowWidth) / 2, (videoMode->height - windowHeight) / 2);

        // Finish window setup
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glfwShowWindow(window);

        // GLEW Stuff
        glewExperimental = true;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            std::cout << "Error: %s\n" << glewGetErrorString(err);
        }
        std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << "\n";
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        if (!glewIsSupported("GL_ARB_shader_objects") || !glewIsSupported("GL_ARB_vertex_shader") || !glewIsSupported("GL_ARB_fragment_shader")) {
            std::cerr << "Missing necessary OpenGL extensions";
        }

        // Setup OpenGL
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_MULTISAMPLE);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // VAO Stuff, maybe optional
        GLuint vertexArrayId;
        glGenVertexArrays(1, &vertexArrayId);
        glBindVertexArray(vertexArrayId);

        // Setup game stuff
        imageShader = new ImageShader("vImage.glsl", "fImage.glsl");
        imageShader->link();
        drawer = new ImageDrawer(*imageShader, windowWidth, windowHeight);

        windowFramebuffer = new GlScreenBuffer(window);

        gameTexture = new Texture(windowWidth, windowHeight);
        gameFramebuffer = new GlFramebuffer(1, gameTexture, true);

        drawer->bind();

        stageDrawer = new StageDrawer(*drawer);
        levelDrawer = new LevelDrawer(*drawer);
        uiDrawer = new UIDrawer(*drawer, windowWidth, windowHeight);
    }

    void exitWindow() {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void setKey(int key, bool value) {
        if (key < 348) {
            keys[key] = value;
        }
    }

    bool* getKeys() {
        return keys;
    }

    void getCursorPos(double& x, double& y) {
        glfwGetCursorPos(window, &x, &y);
    }

    void getCursorPosLetterboxed(double& x, double& y) {
        double xBase;
        double yBase;
        getCursorPos(xBase, yBase);
        xBase = -1.0 + 2.0 * (xBase / getWidth());
        yBase = 1.0 - 2.0 * (yBase / getHeight());

        glm::mat4 projection = drawer->getLetterboxedProjection(*gameTexture, (float) windowFramebuffer->getWidth() / (float) windowFramebuffer->getHeight());
        glm::mat4 projInverse = glm::inverse(projection);
        glm::vec4 mouseBase = glm::vec4(xBase, yBase, 0.0, 1.0);
        glm::vec4 mouseNew = projInverse * mouseBase;

        x = mouseNew.x * gameTexture->getWidth();
        y = mouseNew.y * gameTexture->getHeight();
    }

    void resize(int width, int height) {
        gameFramebuffer->resize(width, height);
        drawer->resize(width, height);
        uiDrawer->updateWindowHeight(height);
    }

    int getWidth() {
        return windowFramebuffer->getWidth();
    }

    int getHeight() {
        return windowFramebuffer->getHeight();
    }

    int getGameWidth() {
        return gameFramebuffer->getWidth();
    }

    int getGameHeight() {
        return gameFramebuffer->getHeight();
    }

    void enableFullscreen() {
        if (fullscreen) {
            return;
        }
        fullscreen = true;
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwGetWindowSize(window, &oldWindowW, &oldWindowH);
        glfwGetWindowPos(window, &oldWindowX, &oldWindowY);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 60);
        glfwSwapInterval(1);
    }

    void disableFullscreen() {
        if (!fullscreen) {
            return;
        }
        fullscreen = false;

        glfwSetWindowMonitor(window, NULL, 0, 0, getWidth(), getHeight(), 60);

        glfwSetWindowSize(window, oldWindowW, oldWindowH);
        glfwSetWindowPos(window, oldWindowX, oldWindowY);
        glfwSwapInterval(1);
    }

    bool isFullscreen() {
        return fullscreen;
    }

    ~GlWindow() {
        glfwDestroyWindow(window);
        
        glfwSetErrorCallback(NULL);
	    glfwTerminate();

        delete drawer;
        delete stageDrawer;
        delete levelDrawer;
        delete uiDrawer;
        delete gameTexture;
        delete gameFramebuffer;
        delete windowFramebuffer;
        delete imageShader;
    }
};