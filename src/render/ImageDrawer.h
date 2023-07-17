#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>

#include "render/Texture.h"
#include "render/shader/ImageShader.h"
#include "render/GlFramebuffer.h"

#include "TileMappings.h"

const unsigned char emptyPallete[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};
const GLfloat quadVertices[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};
const GLfloat quadTexCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};
const GLint quadFaces[] = {
    0, 1, 2,
    2, 1, 3
};

class ImageDrawer {
    private:
    ImageShader* shader;
    glm::mat4 projection;

    GLuint quadVerticesBuffer;
    GLuint quadFacesBuffer;
    GLuint quadTexCoordsBuffer;
    public:
    ImageDrawer(ImageShader& shader, int width, int height): shader(&shader) {
        resize(width, height);

        glGenBuffers(1, &quadVerticesBuffer);
        glGenBuffers(1, &quadFacesBuffer);
        glGenBuffers(1, &quadTexCoordsBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, quadVerticesBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadFacesBuffer);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadFaces), quadFaces, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordsBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords), quadTexCoords, GL_DYNAMIC_DRAW);
    }

    void resize(int width, int height) {
        projection = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
    }

    void resizeToFramebuffer() {
        Texture* texture = GlFramebuffer::getCurrentFramebuffer()->getTextures();
        resize(texture->getWidth(), texture->getHeight());
    }

    void bind() {
        shader->bind();

        glBindBuffer(GL_ARRAY_BUFFER, quadVerticesBuffer);
        glVertexAttribPointer(shader->aVertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordsBuffer);
        glVertexAttribPointer(shader->aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    void removePalleteSwap() {
        setPalleteSwap(emptyPallete, false);
    }

    void setPalleteSwap(const unsigned char pallete1[16], const unsigned char pallete2[16], bool useTransparent) {
        GLfloat array1[16];
        GLfloat array2[16];
        for (int i = 0; i < 16; i++) {
            array1[i] = (float) pallete1[i] / 255;
            array2[i] = (float) pallete2[i] / 255;
        }
        if (useTransparent) {
            array2[0] = 0.0f;
            array2[1] = 0.0f;
            array2[2] = 0.0f;
            array2[3] = 0.0f;
        }

        glUniform4fv(shader->uStartColors, 4, array1);
        glUniform4fv(shader->uEndColors, 4, array2);
    }

    void setPalleteSwap(const unsigned char pallete[16], bool useTransparent) {
        setPalleteSwap(pallete, pallete, useTransparent);
    }

    void drawImage(const Texture& texture, int x, int y, float cx, float cy, float cw, float ch) {
        if (cw == 0 || ch == 0) {
            return;
        }

        GLfloat data[] = {
            cx, cy,
            cx + cw, cy,
            cx, cy + ch,
            cx + cw, cy + ch
        };

        glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordsBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadTexCoords), data);

        shader->bindTexture(texture);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float) x, (float) y, 0.0f));
        model = glm::scale(model, glm::vec3(cw * texture.getWidth(), ch * texture.getHeight(), 1.0f));
        // std::printf("Scaled width: %f\n", cw * texture.getWidth());

        glm::mat4 viewModel = projection * model;

        glUniformMatrix4fv(shader->uViewProjectionMatrix, 1, false, glm::value_ptr(viewModel));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadFacesBuffer);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void drawImage(const Texture& texture, int x, int y) {
        drawImage(texture, x, y, 0.0f, 0.0f, 1.0f, 1.0f);
    }

    void drawImage(const Texture& texture, int x, int y, int cx, int cy, int cw, int ch) {
        float w = texture.getWidth();
        float h = texture.getHeight();
        drawImage(texture, x, y, cx / w, cy / h, cw / w, ch / h);
    }

    void drawTile(const Tile& tile, int x, int y) {
        drawImage(*tile.texture, x, y, tile.cx, tile.cy, tile.cw, tile.ch);
    }

    void drawTile(int tile, int x, int y) {
        drawTile(Tiles::getTile(tile), x, y);
    }

    glm::mat4 getLetterboxedProjection(const Texture& texture, float windowAspect) {
        float textureAspect = (float) texture.getWidth() / (float) texture.getHeight();
        if (textureAspect < windowAspect) {
            float barWidth = (windowAspect - textureAspect) / textureAspect * 0.5f;
            return glm::ortho(-barWidth, 1.0f + barWidth, 0.0f, 1.0f);
        } else {
            float barHeight = ((1.0 / windowAspect) - (1.0 / textureAspect)) * textureAspect * 0.5f;
            return glm::ortho(0.0f, 1.0f, -barHeight, 1.0f + barHeight);
        }
    }

    void drawLetterboxedImage(const Texture& texture, float windowAspect) {
        glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordsBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords), quadTexCoords, GL_DYNAMIC_DRAW);

        shader->bindTexture(texture);
        
        glm::mat4 projection = getLetterboxedProjection(texture, windowAspect);

        glUniformMatrix4fv(shader->uViewProjectionMatrix, 1, false, glm::value_ptr(projection));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};