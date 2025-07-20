#include "Rectangle.h"
#include "Point.h"
#include <vector>
#include <algorithm>
#include <iostream>

#define RADIUS 5

class Qnode
{
    std::vector<Qnode> children;
    std::vector<Point> points;
    bool divided = false;
    Rectangle boundary;
    int capacity = 4;

public:
    Qnode(Rectangle boundary)
        : boundary(boundary) {}

    bool insert(Point pt)
    {
        if (!boundary.contains(pt))
        {
            return false;
        }

        if (points.size() < capacity)
        {
            points.push_back(pt);
            return true;
        }
        else
        {
            if (!divided)
            {
                subdivide();
            }
            for (int i = 0; i < children.size(); i++)
            {
                if (children[i].insert(pt))
                    return true;
            }
        }
        return false;
    }

    void subdivide()
    {
        const int hw = boundary.get_width() / 2;
        const int hh = boundary.get_height() / 2;

        children.push_back(Qnode{Rectangle{boundary.get_x() + hw, boundary.get_y(), hw, hh}});
        children.push_back(Qnode{Rectangle{boundary.get_x(), boundary.get_y(), hw, hh}});
        children.push_back(Qnode{Rectangle{boundary.get_x(), boundary.get_y() + hh, hw, hh}});
        children.push_back(Qnode{Rectangle{boundary.get_x() + hw, boundary.get_y() + hh, hw, hh}});

        for (const auto &pt : points)
        {
            for (auto &child : children)
            {
                if (child.insert(pt))
                    break;
            }
        }

        points.clear();
        divided = true;
    }

    void queryHelper(const Rectangle &range, std::vector<Point> &found)
    {
        if (!boundary.intersects(range))
            return;

        for (const auto &pt : points)
        {
            if (range.contains(pt))
            {
                found.push_back(pt);
            }
        }

        if (divided)
        {
            for (auto &child : children)
            {
                child.queryHelper(range, found);
            }
        }
    }

    std::vector<Point> query(Rectangle &range)
    {
        std::vector<Point> found;
        queryHelper(range, found);
        return found;
    }

    bool remove(Point &pt)
    {
        if (!boundary.contains(pt))
        {
            return false;
        }
        // try to remove from this node
        // std::find to locate the element
        auto it = std::find(points.begin(), points.end(), pt);
        if (it != points.end())
        {
            points.erase(it);
            return true;
        }

        // otherwise, recuse into children (if any)
        if (divided)
        {
            for (auto &child : children)
            {
                if (child.remove(pt))
                {
                    _tryMerge();
                    return true;
                }
            }
        }
        return false;
    }

    void _tryMerge()
    {
        if (!divided)
            return;

        // check if all children are leaves with zero points
        bool allEmptyLeaves = std::all_of(children.begin(), children.end(), [](const Qnode &c)
                                          { return !c.divided && c.points.empty(); });

        if (allEmptyLeaves)
        {
            children.clear();
            divided = false;
        }
    }
};

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 600;

    const Rectangle qtree_rect{0, 0, screen_width, screen_height};
    Qnode qtree{qtree_rect};
    qtree.insert(Point{20, 20, RADIUS});
    qtree.insert(Point{100, 100, RADIUS});
    qtree.insert(Point{150, 150, RADIUS});
    qtree.insert(Point{300, 300, RADIUS});
    qtree.insert(Point{400, 500, RADIUS});

    Rectangle viewport{0, 0, 200, 200};
    std::vector<Point> hits = qtree.query(viewport);
    for (const auto p : hits)
    {
        std::cout << "point x, y: " << "{" << p.x << ", " << p.y << "}" << std::endl;
    }
    return 0;
};