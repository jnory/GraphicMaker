#include "line.hpp"
#include "rectangle.hpp"

Rectangle::Rectangle(Point left_bottom, size_t width, size_t height) : left_bottom(left_bottom)
{
    this->width = width;
    this->height = height;
}

Rectangle Rectangle::copy_translate(int diff_x, int diff_y) 
{
    return Rectangle(this->left_bottom.copy_translate(diff_x, diff_y), this->width, this->height);
}

Rectangle Rectangle::copy_scale(double scale) 
{
    Point p((size_t) (this->left_bottom.x * scale), (size_t) (this->left_bottom.y * scale));
    return Rectangle(p, (size_t)(this->width * scale), (size_t)(this->height * scale));
}

Shape *Rectangle::copy()
{
    return new Rectangle(this->left_bottom, this->width, this->height);
}

Shape *Rectangle::translate(int diff_x, int diff_y)
{
    this->left_bottom.x += diff_x;
    this->left_bottom.y += diff_y;

    return this;
}

Shape *Rectangle::scale(double scale)
{
    this->left_bottom.scale(scale);
    this->width = (size_t)(this->width * scale);
    this->height = (size_t)(this->height * scale);

    return this;
}

Shape *Rectangle::scale(double scale, Point center)
{
    this->left_bottom.scale(scale, center);
    this->width = (size_t)(this->width * scale);
    this->height = (size_t)(this->height * scale);

    return this;
}

void Rectangle::draw(BitmapFile *file)
{
    Point right_bottom = this->left_bottom.copy_translate((int)this->width, 0);
    Point left_top = this->left_bottom.copy_translate(0, (int)this->height);
    Point right_top = this->left_bottom.copy_translate((int)this->width, (int)this->height);

    Line left(this->left_bottom, left_top);
    Line top(left_top,     right_top);
    Line right(right_top,    right_bottom);
    Line bottom(right_bottom, this->left_bottom);
    
    left.draw(file);
    top.draw(file);
    right.draw(file);
    bottom.draw(file);
}

void Rectangle::describe(std::ostream *out)
{
    *out << "RECTANGLE ";
    *out << this->left_bottom.x << " " << this->left_bottom.y << " ";
    *out << this->width << " " << this->height << std::endl;
}
