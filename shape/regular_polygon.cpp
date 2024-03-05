#include <iostream>
#include <cmath>

#include "regular_polygon.hpp"
#include "point.hpp"
#include "line.hpp"


RegularPolygon::RegularPolygon(size_t n, Point center, size_t r) 
: n(n), center(center), r(r)
{
    this->update();
}

void RegularPolygon::update()
{
    for (size_t i = 0; i < n; i++) {
        this->points.push_back(
            Point(
                (size_t)(center.x + r * std::cos(2 * PI * i / n)),
                (size_t)(center.y + r * std::sin(2 * PI * i / n))
            )
        );
    }
}

Shape *RegularPolygon::copy()
{
    return new RegularPolygon(this->n, this->center, this->r);
}

Shape *RegularPolygon::translate(int diff_x, int diff_y)
{
    this->center.translate(diff_x, diff_y);
    this->update();
    return this;
}

Shape *RegularPolygon::scale(double scale)
{
    this->r = (size_t) (this->r * scale);
    this->center.scale(scale);
    this->update();
    return this;
}

Shape *RegularPolygon::scale(double scale, Point center_)
{
    this->r = (size_t) (this->r * scale);
    this->center.scale(scale, center_);
    this->update();
    return this;
}

void RegularPolygon::draw(BitmapFile *file, DrawingProperty &prop)
{
    for (size_t i = 0; i < this->n; i++) {
        size_t j = (i + 1) % this->n;
        Line line(this->points[i], this->points[j]);
        line.draw(file, prop);
    }
}

void RegularPolygon::describe(std::ostream *out)
{
    *out << "REGULAR_POLYGON " << this->n << " ";
    *out << this->center.x << " " << this->center.y << " ";
    *out << this->r << std::endl;
}
