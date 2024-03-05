#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <iostream>
#include "../io/bitmap.hpp"

class Point;

struct Color {
    Color(uint8_t r, uint8_t g, uint8_t b): R(r), G(g), B(b), A(255) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a): R(r), G(g), B(b), A(a) {}

    Color operator*(double ratio) const {
        // 255 - (255 - R) * ratio
        // 255 - 255*ratio + R*ratio
        // 255 * (1 - ratio) + R * ratio
        return {
            (uint8_t) (255 - 255 * ratio + R * ratio),
            (uint8_t) (255 - 255 * ratio + G * ratio),
            (uint8_t) (255 - 255 * ratio + B * ratio), A};
    }

    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
};

class DrawingProperty {
public:
    explicit DrawingProperty(Color color): color_(color) {}
    Color color() const {
        return color_;
    }

private:
    Color color_;
};

class Shape {
public:
    Shape();
    virtual ~Shape();

    virtual Shape *copy() = 0;
    virtual Shape *translate(int diff_x, int diff_y) = 0;
    virtual Shape *scale(double scale) = 0;
    virtual Shape *scale(double scale, Point center) = 0;
    virtual void draw(BitmapFile *file, DrawingProperty &prop) = 0;
    virtual void describe(std::ostream *out) {
        *out << "Not Implemented" << std::endl;
    }
};

#endif // SHAPE_HPP
