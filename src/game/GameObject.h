#pragma once

class GameObject {
    private:
    const char* id;
    const char* name;
    int tilePreview;

    public:
    GameObject(const char* id, const char* name, int tilePreview): id(id), name(name), tilePreview(tilePreview) {}

    const char* getName() {
        return name;
    }

    int getTilePreview() {
        return tilePreview;
    }

    const char* getId() {
        return id;
    }

    ~GameObject() {
        std::printf("Game Object %s destroyed\n", getId());
    }
};