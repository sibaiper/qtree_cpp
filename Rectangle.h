#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Point.h"

class Rectangle
{
    int x, y, width, height;

public:
    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
    void set_values(int, int, int, int);
    int area() { return width * height; }
    bool intersects(Rectangle rect)
    {
        return !(
            this->x > rect.x + rect.width ||
            this->x + this->width < rect.x ||
            this->y > rect.y + rect.height ||
            this->y + this->height < rect.y);
    }
    bool contains(const Point &pt) const
    {
        return (
            pt.x >= x &&
            pt.x <= x + width &&
            pt.y >= y &&
            pt.y <= y + height);
    }
    int get_width() { return width; }
    int get_height() { return height; }
    int get_x() { return x; }
    int get_y() { return y; }
};

#endif // RECTANGLE_H