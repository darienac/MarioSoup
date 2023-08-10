#pragma once

class IControls {
    public:
    virtual bool left() = 0;
    virtual bool right() = 0;
    virtual bool up() = 0;
    virtual bool down() = 0;

    virtual bool jump() = 0;
    virtual bool action() = 0;

    virtual ~IControls() {}
};