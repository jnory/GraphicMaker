#ifndef SQUARE_HPP
#define SQUARE_HPP


#include "../io/bitmap.hpp"
#include "point.hpp"
#include "line.hpp"
#include "rectangle.hpp"

struct Square: public Rectangle {
public:
    Square(Point left_bottom, size_t width);

    virtual void describe(std::ostream *out);

    virtual Shape *copy();

private:
    Point left_bottom;
    size_t width;
};

#endif // SQUARE_HPP
