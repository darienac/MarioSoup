#pragma once

#include "controls/IControls.h"
#include <GLFW/glfw3.h>

class KeyboardControls: public IControls {
    private:
    bool* keys;

    public:
    KeyboardControls(bool* keys): keys(keys) {}

    virtual bool left() override {
        return keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT];
    }

    virtual bool right() override {
        return keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT];
    }

    virtual bool up() override {
        return keys[GLFW_KEY_W] || keys[GLFW_KEY_UP];
    }

    virtual bool down() override {
        return keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN];
    }

    virtual bool jump() override {
        return keys[GLFW_KEY_X] || keys[GLFW_KEY_SPACE];
    }

    virtual bool action() override {
        return keys[GLFW_KEY_Z] || keys[GLFW_KEY_LEFT_SHIFT];
    }
};