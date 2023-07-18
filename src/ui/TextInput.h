#pragma once

#include "ui/ui.h"
#include "TileMappings.h"

class TextInput: public IUIElement {
    private:
    const char* label;
    int width;
    int maxLength;
    char* textBuffer;
    UITextInputType type;
    void (*callback)(TextInput* input, char* value);
    void* pointer;

    char* value;
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
        adjustScroll();
    }

    void deleteInput() {
        if (cursorPos == textLength) {
            return;
        }
        for (int i = cursorPos + 1; i < textLength; i++) {
            textBuffer[i - 1] = textBuffer[i];
        }
        textLength--;
        adjustScroll();
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
    TextInput(const char* label, int width, int maxLength, char* textBuffer, void (*callback)(TextInput* input, char* value)): label(label), width(width), maxLength(maxLength), textBuffer(textBuffer), type(type), callback(callback) {
        framebufferTexture = new Texture(width * 8 - 2, 14);
        framebuffer = new GlFramebuffer(1, framebufferTexture, false);
    }

    void hover(int x, int y, int gameWidth, int gameHeight) {
        hoverX = x;
        hoverY = y;
        hovered = !(hoverX < 0 || hoverX >= width * 8 || hoverY < 0 || hoverY >= 16);
    }

    void click() {
        selected = hovered;
        if (hovered) {
            if (callback != nullptr) {
                callback(this, value);
            }
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
        adjustScroll();
    }

    UIElementType getElementType() {
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