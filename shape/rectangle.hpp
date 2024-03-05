#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "../io/bitmap.hpp"
#include "point.hpp"
#include "shape.hpp"

class Rectangle: public Shape {
public:
    Rectangle(Point left_bottom, size_t width, size_t height);

    Rectangle copy_translate(int diff_x, int diff_y);
    Rectangle copy_scale(double scale);

    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file, DrawingProperty &prop);
    virtual void describe(std::ostream *out);

private:
    Point left_bottom;
    size_t width;
    size_t height;
};

#endif