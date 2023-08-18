#pragma once

class CollisionBox {
    private:
    int xOff;
    int yOff;
    int width;
    int height;
    
    public:
    CollisionBox(int xOff, int yOff, int width, int height): xOff(xOff), yOff(yOff), width(width), height(height) {}

    int getXOff() {
        return xOff;
    }
    int getYOff() {
        return yOff;
    }
    int getWidth() {
        return width;
    }
    int getHeight() {
        return height;
    }

    bool collidesWith(CollisionBox& box, int relX, int relY) {
        int b1x1 = xOff;
        int b1x2 = b1x1 + width;
        int b1y1 = yOff;
        int b1y2 = b1y1 + height;
        
        int b2x1 = box.getXOff() + relX;
        int b2x2 = b2x2 + box.getWidth();
        int b2y1 = box.getYOff() + relY;
        int b2y2 = b2y1 + box.getHeight();

        bool xCollide = (b1x1 >= b2x1 && b1x1 <= b2x2) || (b1x2 >= b2x1 && b1x2 <= b2x2) || (b2x1 >= b1x1 && b2x1 <= b1x2) || (b2x2 >= b1x1 && b2x2 <= b1x2);
        bool yCollide = (b1y1 >= b2y1 && b1y1 <= b2y2) || (b1y2 >= b2y1 && b1y2 <= b2y2) || (b2y1 >= b1y1 && b2y1 <= b1y2) || (b2y2 >= b1y1 && b2y2 <= b1y2);
        return xCollide && yCollide;
    }
};