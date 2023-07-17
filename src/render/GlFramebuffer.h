#pragma once

#include <GL/glew.h>

#include "render/Texture.h"

class GlFramebuffer {
    protected:
    bool useDepthBuffer;
    GLsizei width;
    GLsizei height;
    GLuint depthBufferId;
    GLuint framebufferId;
    Texture* textures;
    int numTextures;

    static GlFramebuffer* currentFramebuffer;

    public:
    GlFramebuffer(int numTextures, Texture textures[], bool useDepthBuffer): textures(textures), numTextures(numTextures) {
        this->useDepthBuffer = useDepthBuffer;

        width = textures[0].getWidth();
        height = textures[0].getHeight();

        glGenFramebuffers(1, &framebufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

        if (useDepthBuffer) {
            glGenRenderbuffers(1, &depthBufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);
        }

        GLenum* drawBuffers = new GLenum[numTextures];
        for (int i = 0; i < numTextures; i++) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textures[i].getTextureId(), 0);
            drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(numTextures, drawBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw "Unable to create framebuffer";
        }
    }

    GlFramebuffer(GLsizei width, GLsizei height) {
        framebufferId = 0;
        useDepthBuffer = false;
        this->width = width;
        this->height = height;
    }

    GlFramebuffer(): GlFramebuffer(0, 0) {}

    void resize(int width, int height) {
        this->width = width;
        this->height = height;
        bind();

        if (useDepthBuffer) {
            glGenRenderbuffers(1, &depthBufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);
        }

        for (int i = 0; i < numTextures; i++) {
            textures[i].resize(width, height);
        }
    }

    virtual void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
        glViewport(0, 0, getWidth(), getHeight());
        currentFramebuffer = this;
    }

    GLuint getFramebufferId() {
        return framebufferId;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    Texture* getTextures() {
        return textures;
    }

    static GlFramebuffer* getCurrentFramebuffer() {
        return currentFramebuffer;
    }

    ~GlFramebuffer() {
        std::cout << "Framebuffer destroyed\n";

        glDeleteFramebuffers(1, &framebufferId);
        if (useDepthBuffer) {
            glDeleteRenderbuffers(1, &depthBufferId);
        }
    }
};

GlFramebuffer* GlFramebuffer::currentFramebuffer = nullptr;