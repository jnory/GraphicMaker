#ifndef POINT_HPP
#define POINT_HPP

#include <cstddef>

#include "shape.hpp"

void set_pixel(BitmapFile *file, size_t x, size_t y, uint8_t brightness);
void set_color(BitmapFile *file, size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

struct Point : public Shape {
    size_t x;
    size_t y;

    Point();
    Point(size_t x, size_t y);

    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file);
    virtual void describe(std::ostream *out);

    Point copy_translate(int diff_x, int diff_y);
};

#endif //POINT_HPP