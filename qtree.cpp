#include "Rectangle.h"
#include "Point.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>


#define RADIUS 5
int gen_range(int, int);


class Qnode
{
    std::vector<Qnode> children;
    std::vector<Point> points;
    bool divided = false;
    Rectangle boundary;
    int capacity = 4;

public:
    Qnode(const Rectangle &boundary)
        : boundary(boundary) {}

    bool insert(const Point &pt)
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

    std::vector<Point> query(const Rectangle &range)
    {
        std::vector<Point> found;
        queryHelper(range, found);
        return found;
    }

    bool remove(const Point &pt)
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

    void update(const Point &pt)
    {
        remove(pt);
        insert(pt);
    }
};

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 600;

    const Rectangle qtree_rect{0, 0, screen_width, screen_height};
    Qnode qtree{qtree_rect};

    
    auto insert_start = std::chrono::high_resolution_clock::now();
    
    int num_of_points = 1000;

    for (size_t i = 0; i < num_of_points; i++)
    {
        qtree.insert(Point(gen_range(0, screen_width), gen_range(0, screen_height), RADIUS));
    }

    auto insert_end = std::chrono::high_resolution_clock::now();
    auto insert_duration = std::chrono::duration_cast<std::chrono::microseconds>(insert_end - insert_start);
    std::cout << "Insertion of " << num_of_points << " took " << insert_duration.count() << " microseconds.\n";

    

    Rectangle viewport{0, 0, 400, 400};
    auto query_start = std::chrono::high_resolution_clock::now();
    std::vector<Point> hits = qtree.query(viewport);

    auto query_end = std::chrono::high_resolution_clock::now();
    auto query_duration = std::chrono::duration_cast<std::chrono::microseconds>(query_end - query_start);
    std::cout << "Queried " << hits.size() << " items, took " << query_duration.count() << " microseconds.\n";


    std::cout << "range is x, y is : {" << viewport.get_x() << ", " << viewport.get_y() << "} and w, h: {" << viewport.get_width() << ", " << viewport.get_height() << "}\n"; 
    for (const auto p : hits)
    {
        std::cout << "point x, y: " << "{" << p.x << ", " << p.y << "}" << std::endl;
    }
    return 0;
};

int gen_range(int a, int b)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(a, b);
    return distr(gen);
}