#include <iostream>
#include <cassert>
#include <utility>

#include "../io/bitmap.hpp"
#include "point.hpp"
#include "circle.hpp"

void set_color_safe(BitmapFile *file, size_t x_pos, size_t x_neg, size_t y_pos, size_t y_neg, double brightness, Color color)
{
    if (x_pos < x_neg) {
        return;
    }
    if (y_pos < y_neg) {
        return;
    }
    set_color(file, x_pos - x_neg, y_pos -y_neg, color * brightness);
}


void xiaolin_wus_algorithm_circle(
    BitmapFile *file, size_t center_x, size_t center_y, size_t r, Color color
 ){
    size_t x = r;
    size_t sq_error = 0;
    for (size_t y = 0; y <= x; y++) {
        auto brightness = (double)sq_error / (2.0 * (double)r);
        // std::cout << "x = " << x << " y = " << y << " sq_error = " << sq_error << " brightness = " << (int)brightness << std::endl;

        // low angle
        set_color_safe(file, center_x + x, 0, center_y + y, 0, 1 - brightness, color);
        set_color_safe(file, center_x + x + 1, 0, center_y + y, 0, brightness, color);
        set_color_safe(file, center_x + x, 0, center_y, y, 1 - brightness, color);
        set_color_safe(file, center_x + x + 1, 0, center_y, y, brightness, color);
        set_color_safe(file, center_x, x, center_y + y, 0, 1 - brightness, color);
        set_color_safe(file, center_x, x + 1, center_y + y, 0, brightness, color);
        set_color_safe(file, center_x, x, center_y, y, 1 - brightness, color);
        set_color_safe(file, center_x, x + 1, center_y, y, brightness, color);

        // high angle
        set_color_safe(file, center_x + y, 0, center_y + x, 0, 1 - brightness, color);
        set_color_safe(file, center_x + y, 0, center_y + x + 1, 0, brightness, color);
        set_color_safe(file, center_x, y, center_y + x, 0, 1 - brightness, color);
        set_color_safe(file, center_x, y, center_y + x + 1, 0, brightness, color);
        set_color_safe(file, center_x + y, 0, center_y, x, 1 - brightness, color);
        set_color_safe(file, center_x + y, 0, center_y, x + 1, brightness, color);
        set_color_safe(file, center_x, y, center_y, x, 1 - brightness, color);
        set_color_safe(file, center_x, y, center_y, x + 1, brightness, color);

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

void Circle::draw(BitmapFile *file, DrawingProperty &prop)
{
    xiaolin_wus_algorithm_circle(
        file,
        this->center_.x,
        this->center_.y,
        this->r_,
        prop.color()
    );
}