#include "Rectangle.h"

void Rectangle::set_values(int a, int b, int c, int d)
{
    x = a;
    y = b;
    width = c;
    height = d;
}

bool Rectangle::operator==(const Rectangle& other) const {
    return x == other.x && y == other.y && width == other.width && height == other.height ;
}