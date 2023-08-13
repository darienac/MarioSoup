#pragma once

#include <iostream>
#include <stb/stb_image.h>
#include <GL/glew.h>

#include "ResourceReader.h"

class Texture {
    private:
    GLuint textureId;
    int width;
    int height;

    public:
    Texture(const std::string& fileName) {
        textureId = -1;
        
        load(fileName);
    }

    Texture(int width, int height) {
        glGenTextures(1, &textureId);
        if ((int) textureId < 0) {
            throw "Unable to create texture";
        }

        resize(width, height);
    }

    void resize(int width, int height) {
        this->width = width;
        this->height = height;

        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void load(const std::string& fileName) {
        if (textureId != (GLuint) -1) {
            throw "Texture already loaded";
        }

        stbi_set_flip_vertically_on_load(true);

        int avChannels;
        stbi_uc *decodedImage = stbi_load(ResourceReader::getFullPath(ResourceReader::Texture, fileName).c_str(), &width, &height, &avChannels, 4);

        glGenTextures(1, &textureId);
        if (textureId < 0) {
            throw "Unable to create texture";
        }

        bind();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, decodedImage);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(decodedImage);
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    GLuint getTextureId() const {
        return textureId;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    ~Texture() {
        if (getTextureId() == (GLuint) -1) {
            std::printf("Fake texture %d deleted\n", getTextureId());
            return;
        }
        std::printf("Texture %d deleted\n", getTextureId());
        glDeleteTextures(1, &textureId);
    }
};