// I don't know if I'll actually use this yet, this was more for doing instanced stuff

#pragma once

#include <GL/glew.h>
#include <iostream>

class GlAttribute {
    private:
    GLuint programId;
    GLint attribLocation;
    public:
    GlAttribute(GLuint programId, std::string name) {
        attribLocation = glGetAttribLocation(programId, name.c_str());
    }
};