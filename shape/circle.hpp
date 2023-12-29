#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "../io/bitmap.hpp"
#include "point.hpp"
#include "shape.hpp"

class Circle: public Shape {
public:
    Circle(Point center, size_t r);

    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file);
    virtual void describe(std::ostream *out);

private:
    Point center_;
    size_t r_;
};

#endif // CIRCLE_HPP