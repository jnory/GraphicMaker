#include <iostream>
#include <cmath>

#include "../io/bitmap.hpp"
#include "point.hpp"
#include "line.hpp"

void swap(size_t *x1, size_t *y1, size_t *x2, size_t *y2){
    size_t tmp;
    tmp = *x1;
    *x1 = *x2;
    *x2 = tmp;
    tmp = *y1;
    *y1 = *y2;
    *y2 = tmp;
}

void xiaolin_wus_algorithm_high_angle(
    BitmapFile *file, size_t x1, size_t y1, size_t y2,
    size_t diff_x, size_t diff_y, Color color
 ){
    double grad = (double) diff_x / diff_y;
    double error = 0.0;
    size_t x = x1;
    if (y2 > y1) {
        for (size_t y = y1; y <= y2; y++) {
            set_color(file, x, y, color * (1 - error));
            set_color(file, x + 1, y, color * error);
            error += grad;
            if (error > 1) {
                error -= 1;
                x++;
            }
        }
    } else {
        for (size_t y = y1; y >= y2; y--) {
            set_color(file, x, y, color * (1 - error));
            set_color(file, x + 1, y, color * error);
            error += grad;
            if (error > 1) {
                error -= 1;
                x++;
            }
        }
    }
}

void xiaolin_wus_algorithm_low_angle(
    BitmapFile *file, size_t x1, size_t y1, size_t x2, size_t y2,
    size_t diff_x, size_t diff_y, Color color
 ){
    int step;
    if (y2 > y1) {
        step = 1;
    } else {
        step = -1;
    }

    double grad = (double) diff_y / diff_x;
    double error = 0.0;
    size_t y = y1;
    for (size_t x = x1; x <= x2; x++) {
        set_color(file, x, y, color * (1 - error));
        set_color(file, x, y + step, color * error);
        error += grad;
        if (error > 1) {
            error -= 1;
            y += step;
        }
    }
}

Line::Line(Point p1, Point p2): p1(p1), p2(p2)
{
    if (this->p1.x == this->p2.x && this->p1.y == this->p2.y) {
        std::cerr 
            << "Warning: Start and end are at the same position: "
            << "(" << this->p1.x << "," << this->p1.y << ")" 
            << std::endl;
    }
}

Line::Line(Point p, size_t length, double theta)
: p1(p), 
  p2(
    p.x + (size_t)(std::cos(theta / 360 * 2 * PI) * length),
    p.y + (size_t)(std::sin(theta / 360 * 2 * PI) * length)
  )
{}

Shape *Line::copy()
{
    return new Line(this->p1, this->p2);
}

Shape *Line::translate(int diff_x, int diff_y)
{
    this->p1.translate(diff_x, diff_y);
    this->p2.translate(diff_x, diff_y);

    return this;
}

Shape *Line::scale(double scale)
{
    this->p1.scale(scale);
    this->p2.scale(scale);

    return this;
}

Shape *Line::scale(double scale, Point center)
{
    this->p1.scale(scale, center);
    this->p2.scale(scale, center);

    return this;
}

void Line::describe(std::ostream *out)
{
    *out << "LINE ";
    *out << this->p1.x << " " << this->p1.y << " ";
    *out << this->p2.x << " " << this->p2.y << std::endl;
}

void Line::draw(BitmapFile *file, DrawingProperty &prop)
{
    size_t x1 = this->p1.x;
    size_t y1 = this->p1.y;
    size_t x2 = this->p2.x;
    size_t y2 = this->p2.y;

    if (x2 < x1) {
        swap(&x1, &y1, &x2, &y2);
    }
    size_t diff_x = x2 - x1;

    size_t diff_y;
    if (y2 > y1) {
        diff_y = y2 - y1;
    } else {
        diff_y = y1 - y2;
    }

    if (diff_x < diff_y) {
        xiaolin_wus_algorithm_high_angle(file, x1, y1, y2, diff_x, diff_y, prop.color());
    } else {
        xiaolin_wus_algorithm_low_angle(file, x1, y1, x2, y2, diff_x, diff_y, prop.color());
    }
}