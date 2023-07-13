#pragma once

#include "render/shader/ShaderProgram.h"

#include <iostream>

// TODO: Add support for single pallete swap with 2 uniform3fv arrays (4 colors -> 4 colors)

class ImageShader : public ShaderProgram {
    private:
    public:
    GLint aVertex;
    GLint aTexCoord;
    GLint uViewProjectionMatrix;
    GLint uStartColors;
    GLint uEndColors;

    ImageShader(): ShaderProgram() {}
    ImageShader(std::string vertexShader, std::string fragmentShader): ShaderProgram(vertexShader, fragmentShader) {}

    virtual void link() {
        ShaderProgram::link();

        aVertex = glGetAttribLocation(getProgramId(), "aVertex");
	    glEnableVertexAttribArray(aVertex);
        aTexCoord = glGetAttribLocation(getProgramId(), "aTexCoord");
	    glEnableVertexAttribArray(aTexCoord);

        uViewProjectionMatrix = glGetUniformLocation(getProgramId(), "uViewProjectionMatrix");

        uStartColors = glGetUniformLocation(getProgramId(), "uStartColors");
        uEndColors = glGetUniformLocation(getProgramId(), "uEndColors");
    }
};