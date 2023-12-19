#include "line.hpp"
#include "equilateral_triangle.hpp"

EquilateralTriangle::EquilateralTriangle(Point left_bottom, size_t side)
    : Triangle(
        left_bottom, 
        left_bottom.copy_translate(side, 0), 
        left_bottom.copy_translate(side / 2, side * 1.73205 / 2)
    ), left_bottom(left_bottom), side(side), theta(0)
{}

EquilateralTriangle::EquilateralTriangle(Point left_bottom, size_t side, double theta)
    : Triangle(
        left_bottom, 
        Point(
            left_bottom.x + side * std::cos(theta / 360 * 2 * PI),
            left_bottom.y + side * std::sin(theta / 360 * 2 * PI)
        ), 
        Point(
            left_bottom.x + side * std::cos((theta + 60) / 360 * 2 * PI),
            left_bottom.y + side * std::sin((theta + 60) / 360 * 2 * PI)
        ) 
    ), left_bottom(left_bottom), side(side), theta(theta)
{}

void EquilateralTriangle::describe(std::ostream *out)
{
    *out << "EQUILATERAL_TRIANGLE " 
        << this->left_bottom.x 
        << " " << this->left_bottom.y 
        << " " << this->side 
        << " " << this->theta << std::endl;
}

Shape *EquilateralTriangle::copy()
{
    return new EquilateralTriangle(this->left_bottom, this->side);
}
