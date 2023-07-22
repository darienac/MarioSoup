#pragma once

class GameObject {
    private:
    const char* name;
    int tilePreview;

    public:
    GameObject(const char* name, int tilePreview): name(name), tilePreview(tilePreview) {}

    const char* getName() {
        return name;
    }

    int getTilePreview() {
        return tilePreview;
    }
};