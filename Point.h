#ifndef POINT_H
#define POINT_H

class Point {
public:
    int x, y, r;
    Point(int x, int y, int r): x(x), y(y), r(r) {};

    bool operator==(const Point& other) const;
};

#endif // POINT_H