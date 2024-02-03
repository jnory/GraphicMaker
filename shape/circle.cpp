#include <iostream>
#include <cassert>
#include <utility>

#include "../io/bitmap.hpp"
#include "point.hpp"
#include "circle.hpp"

void set_pixel_safe(BitmapFile *file, size_t x_pos, size_t x_neg, size_t y_pos, size_t y_neg, uint8_t brightness)
{
    if (x_pos < x_neg) {
        return;
    }
    if (y_pos < y_neg) {
        return;
    }
    set_pixel(file, x_pos - x_neg, y_pos -y_neg, brightness);
}


void xiaolin_wus_algorithm_circle(
    BitmapFile *file, size_t center_x, size_t center_y, size_t r
 ){
    size_t x = r;
    size_t sq_error = 0;
    for (size_t y = 0; y <= x; y++) {
        auto brightness = (uint8_t)(255 * ((double)sq_error / (2.0 * (double)r)));
        // std::cout << "x = " << x << " y = " << y << " sq_error = " << sq_error << " brightness = " << (int)brightness << std::endl;

        // low angle
        set_pixel_safe(file, center_x + x,     0,     center_y + y, 0, brightness);
        set_pixel_safe(file, center_x + x + 1, 0,     center_y + y, 0, 255 - brightness);
        set_pixel_safe(file, center_x + x,     0,     center_y,     y, brightness);
        set_pixel_safe(file, center_x + x + 1, 0,     center_y,     y, 255 - brightness);
        set_pixel_safe(file, center_x,         x,     center_y + y, 0, brightness);
        set_pixel_safe(file, center_x,         x + 1, center_y + y, 0, 255 - brightness);
        set_pixel_safe(file, center_x,         x    , center_y,     y, brightness);
        set_pixel_safe(file, center_x        , x + 1, center_y,     y, 255 - brightness);

        // high angle
        set_pixel_safe(file, center_x + y, 0, center_y + x,     0,     brightness);
        set_pixel_safe(file, center_x + y, 0, center_y + x + 1, 0,     255 - brightness);
        set_pixel_safe(file, center_x,     y, center_y + x,     0,     brightness);
        set_pixel_safe(file, center_x,     y, center_y + x + 1, 0,     255 - brightness);
        set_pixel_safe(file, center_x + y, 0, center_y,         x,     brightness);
        set_pixel_safe(file, center_x + y, 0, center_y,         x + 1, 255 - brightness);
        set_pixel_safe(file, center_x,     y, center_y        , x,     brightness);
        set_pixel_safe(file, center_x,     y, center_y,         x + 1, 255 - brightness);

        size_t diff = 2 * y + 1;
        if (sq_error < diff) {
            size_t diff2 = 2 * x - 1;
            sq_error += diff2 - diff;
            x--;
        } else {
            sq_error -= diff;
        }
    }
}

Circle::Circle(Point center, size_t r)
: center_(std::move(center)), r_(r){}

Shape *Circle::copy()
{
    return new Circle(this->center_, this->r_);
}

Shape *Circle::translate(int diff_x, int diff_y)
{
    this->center_.translate(diff_x, diff_y);
    return this;
}

Shape *Circle::scale(double scale)
{
    assert(scale > 0);
    this->r_ = (size_t) (this->r_ * scale);

    return this;
}

Shape *Circle::scale(double scale, Point center)
{
    this->center_.scale(scale, center);
    this->scale(scale);

    return this;
}

void Circle::describe(std::ostream *out)
{
    *out << "CIRCLE ";
    *out << this->center_.x << " " << this->center_.y << " ";
    *out << this->r_ << std::endl;
}

void Circle::draw(BitmapFile *file)
{
    xiaolin_wus_algorithm_circle(
        file,
        this->center_.x,
        this->center_.y,
        this->r_
    );
}