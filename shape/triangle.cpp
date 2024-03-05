#include "line.hpp"
#include "triangle.hpp"

Triangle::Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3)
{}

Triangle Triangle::copy_translate(int diff_x, int diff_y) 
{
    return Triangle(
        this->p1.copy_translate(diff_x, diff_y), 
        this->p2.copy_translate(diff_x, diff_y), 
        this->p3.copy_translate(diff_x, diff_y)
    );
}

Triangle Triangle::copy_scale(double scale) 
{
    Point new_p2(
        (size_t)((this->p2.x - this->p1.x) * scale + this->p1.x),
        (size_t)((this->p2.y - this->p1.y) * scale + this->p1.y)
    );
    Point new_p3(
        (size_t)((this->p3.x - this->p1.x) * scale + this->p1.x),
        (size_t)((this->p3.y - this->p1.y) * scale + this->p1.y)
    );
    return Triangle(this->p1, new_p2, new_p3);
}

Shape *Triangle::copy()
{
    return new Triangle(this->p1, this->p2, this->p3);
}

Shape *Triangle::translate(int diff_x, int diff_y)
{
    this->p1.x += diff_x;
    this->p1.y += diff_y;

    return this;
}

Shape *Triangle::scale(double scale)
{
    this->p1.scale(scale);
    this->p2.scale(scale);
    this->p3.scale(scale);

    return this;
}

Shape *Triangle::scale(double scale, Point center)
{
    this->p1.scale(scale, center);
    this->p2.scale(scale, center);
    this->p3.scale(scale, center);

    return this;
}

void Triangle::draw(BitmapFile *file, DrawingProperty &prop)
{
    Line line1(this->p1, this->p2);
    Line line2(this->p2, this->p3);
    Line line3(this->p3, this->p1);

    line1.draw(file, prop);
    line2.draw(file, prop);
    line3.draw(file, prop);
}

void Triangle::describe(std::ostream *out)
{
    *out << "TRIANGLE ";
    *out << this->p1.x << " " << this->p1.y << " ";
    *out << this->p2.x << " " << this->p2.y << " ";
    *out << this->p3.x << " " << this->p3.y << std::endl;
}
