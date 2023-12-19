#include "square.hpp"
#include "line.hpp"

Square::Square(Point left_bottom, size_t width) 
: Rectangle(left_bottom, width, width), left_bottom(left_bottom), width(width) 
{}

void Square::describe(std::ostream *out)
{
    *out << "SQUARE " << this->left_bottom.x
         << " " << this->left_bottom.y
         << " " << this->width << std::endl;
}

Shape *Square::copy()
{
    return new Square(this->left_bottom, width);
}
