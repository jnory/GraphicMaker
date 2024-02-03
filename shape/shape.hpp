#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <iostream>
#include "../io/bitmap.hpp"

class Point;

class Shape {
public:
    Shape();
    virtual ~Shape();

    virtual Shape *copy() = 0;
    virtual Shape *translate(int diff_x, int diff_y) = 0;
    virtual Shape *scale(double scale) = 0;
    virtual Shape *scale(double scale, Point center) = 0;
    virtual void draw(BitmapFile *file) = 0;
    virtual void describe(std::ostream *out) {
        *out << "Not Implemented" << std::endl;
    }
};

#endif // SHAPE_HPP
