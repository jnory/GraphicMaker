#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "../io/bitmap.hpp"
#include "shape.hpp"
#include "point.hpp"

struct Triangle: public Shape {
public:
    Triangle(Point p1, Point p2, Point p3);

    Triangle copy_translate(int diff_x, int diff_y);
    Triangle copy_scale(double scale);
    
    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file, DrawingProperty &prop);
    virtual void describe(std::ostream *out);

private:
    Point p1;
    Point p2;
    Point p3;
};

#endif // TRIANGLE_HPP
