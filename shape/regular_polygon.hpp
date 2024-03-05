#ifndef REGULAR_POLYGON_HPP
#define REGULAR_POLYGON_HPP

#include <vector>

#include "../io/bitmap.hpp"
#include "point.hpp"

class RegularPolygon: public Shape {
public:
    RegularPolygon(size_t n, Point center, size_t r);

    void update();

    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file, DrawingProperty &prop);
    virtual void describe(std::ostream *out);

private:
    size_t n;
    Point center;
    size_t r;

    std::vector<Point> points;
};

#endif // REGULAR_POLYGON_HPP