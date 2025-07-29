#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
public:
    int x, y, width, height;
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
    
    int get_width() { return width; }
    int get_height() { return height; }
    int get_x() { return x; }
    int get_y() { return y; }

    bool operator==(const Rectangle& other) const;
};

#endif // RECTANGLE_H