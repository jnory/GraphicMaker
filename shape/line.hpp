#ifndef LINE_HPP
#define LINE_HPP

#include "../io/bitmap.hpp"
#include "point.hpp"
#include "shape.hpp"

#define PI 3.14159265

class Line: public Shape {
public:
    Line(Point p1, Point p2);
    Line(Point p, size_t length, double theta);

    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file);
    virtual void describe(std::ostream *out);

private:
    Point p1;
    Point p2;
};

#endif // LINE_HPP