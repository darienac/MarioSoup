#pragma once

#include "ui/ui.h"
#include "TileMappings.h"
#include "render/GlFramebuffer.h"

class TextInput: public IUIElement {
    private:
    const char* label;
    int width;
    int maxLength;
    char* textBuffer;
    void (*callback)(TextInput* input, char* value);
    void* pointer;

    bool hovered = false;
    bool selected = false;
    int cursorPos = 0;
    int scrollX = 0;
    int textLength = 0;
    int hoverX = 0;
    int hoverY = 0;
    Texture* framebufferTexture;
    GlFramebuffer* framebuffer;

    void backspaceInput() {
        if (cursorPos == 0) {
            return;
        }
        for (int i = cursorPos; i < textLength; i++) {
            textBuffer[i - 1] = textBuffer[i];
        }
        textLength--;
        cursorPos--;
        textBuffer[textLength] = '\0';
        adjustScroll();
        if (callback != nullptr) {
            callback(this, textBuffer);
        }
    }

    void deleteInput() {
        if (cursorPos == textLength) {
            return;
        }
        for (int i = cursorPos + 1; i < textLength; i++) {
            textBuffer[i - 1] = textBuffer[i];
        }
        textLength--;
        textBuffer[textLength] = '\0';
        adjustScroll();
        if (callback != nullptr) {
            callback(this, textBuffer);
        }
    }

    void leftInput() {
        if (cursorPos > 0) {
            cursorPos--;
        }
        adjustScroll();
    }

    void rightInput() {
        if (cursorPos < textLength) {
            cursorPos++;
        }
        adjustScroll();
    }

    void adjustScroll() {
        int cursorPosX = cursorPos * 8 + 3;
        int trueWidth = width * 8 - 2;
        if (cursorPosX + scrollX > (trueWidth - 9)) {
            scrollX = (trueWidth - 9) - cursorPosX;
        } else if (cursorPosX + scrollX < 1) {
            scrollX = 1 - cursorPosX;
        }
    }

    public:
    TextInput(const char* label, int width, int maxLength, char* textBuffer, void (*callback)(TextInput* input, char* value)): label(label), width(width), maxLength(maxLength), textBuffer(textBuffer), callback(callback) {
        framebufferTexture = new Texture(width * 8 - 2, 14);
        framebuffer = new GlFramebuffer(1, framebufferTexture, false);
        textBuffer[0] = '\0';
    }

    bool hover(int x, int y, int gameWidth, int gameHeight) override {
        hoverX = x;
        hoverY = y;
        hovered = !(hoverX < 0 || hoverX >= width * 8 || hoverY < 0 || hoverY >= 16);
        return hovered;
    }

    void click() {
        selected = hovered;
        if (hovered) {
            int relX = hoverX - scrollX;
            cursorPos = (relX - 3) / 8;
            if (cursorPos < 0) {
                cursorPos = 0;
            }
            if (cursorPos > textLength) {
                cursorPos = textLength;
            }
            adjustScroll();
        }
    }

    void activate() {
        selected = true;
    }

    void clear() {
        textBuffer[0] = '\0';
        cursorPos = 0;
        scrollX = 0;
        textLength = 0;
    }

    void charInput(int codepoint) {
        if (!selected) {
            return;
        }
        if (codepoint == GLFW_KEY_BACKSPACE) {
            backspaceInput();
            return;
        } else if (codepoint == GLFW_KEY_DELETE) {
            deleteInput();
            return;
        } else if (codepoint == GLFW_KEY_LEFT) {
            leftInput();
            return;
        } else if (codepoint == GLFW_KEY_RIGHT) {
            rightInput();
            return;
        } else if (Tiles::textEncoding.find((char) codepoint) == std::string::npos) {
            return;
        }

        if (textLength >= maxLength) {
            return;
        }
        for (int i = textLength - 1; i >= cursorPos; i--) {
            textBuffer[i + 1] = textBuffer[i];
        }
        textBuffer[cursorPos] = codepoint;
        textLength++;
        cursorPos++;
        textBuffer[textLength] = '\0';
        adjustScroll();
        if (callback != nullptr) {
            callback(this, textBuffer);
        }
    }

    UIElementType getElementType() override {
        return UIElementType::TEXT_INPUT;
    }

    int getWidth() {
        return width;
    }

    const char* getLabel() {
        return label;
    }

    int getCursorPos() {
        return cursorPos;
    }

    bool isSelected() {
        return selected;
    }

    char* getTextBuffer() {
        return textBuffer;
    }

    int getMaxLength() {
        return maxLength;
    }

    int getScrollX() {
        return scrollX;
    }

    int getTextLength() {
        return textLength;
    }

    GlFramebuffer* getFramebuffer() {
        return framebuffer;
    }

    void setPointer(void* pointer) {
        this->pointer = pointer;
    }

    void* getPointer() {
        return pointer;
    }

    ~TextInput() {
        delete framebuffer;
        delete framebufferTexture;
    }
};