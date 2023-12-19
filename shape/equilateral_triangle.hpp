#ifndef EQUILATERAL_TRIANGLE
#define EQUILATERAL_TRIANGLE


#include "triangle.hpp"

struct EquilateralTriangle: public Triangle {
public:
    EquilateralTriangle(Point left_bottom, size_t side);
    EquilateralTriangle(Point left_bottom, size_t side, double theta);

    virtual void describe(std::ostream *out);

    virtual Shape *copy();

private:
    Point left_bottom;
    size_t side;
    double theta;
};


#endif // EQUILATERAL_TRIANGLE
