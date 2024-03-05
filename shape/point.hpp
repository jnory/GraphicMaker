#ifndef POINT_HPP
#define POINT_HPP

#include <cstddef>

#include "shape.hpp"

#define PI 3.14159265

void set_color(BitmapFile *file, size_t x, size_t y, Color color);

class Point : public Shape {
public:
    size_t x;
    size_t y;

    Point();
    Point(size_t x, size_t y);

    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file, DrawingProperty &prop);
    virtual void describe(std::ostream *out);

    Point copy_translate(int diff_x, int diff_y);
};

#endif //POINT_HPP