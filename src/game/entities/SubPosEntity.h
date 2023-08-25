#pragma once

#include "game/entities/IEntity.h"

class SubPosEntity: virtual public IEntity {
    private:
    int xSub;
    int ySub;
    int zoneLayer;

    public:
    SubPosEntity(int x, int y, int zoneLayer): xSub(x * 16), ySub(y * 16), zoneLayer(zoneLayer) {}

    virtual int getZoneLayer() override {
        return zoneLayer;
    }
    virtual void setZoneLayer(int value) override {
        zoneLayer = value;
    }
    virtual int getX() override {
        return div(xSub, 16);
    }
    virtual void setX(int value) override {
        xSub = value * 16;
    }
    virtual int getY() override {
        return div(ySub, 16);
    }
    virtual void setY(int value) override {
        ySub = value * 16;
    }
    
    virtual int getXSub() {
        return xSub;
    }
    virtual void setXSub(int value) {
        xSub = value;
    }
    virtual int getYSub() {
        return ySub;
    }
    virtual void setYSub(int value) {
        ySub = value;
    }
};