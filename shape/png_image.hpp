#ifndef PNG_IMAGE_HPP
#define PNG_IMAGE_HPP

#include <vector>
#include <string>

#include "../io/bitmap.hpp"
#include "point.hpp"
#include "shape.hpp"

class PngImage: public Shape {
public:
    PngImage(std::string path, Point left_bottom);

    virtual Shape *copy();
    virtual Shape *translate(int diff_x, int diff_y);
    virtual Shape *scale(double scale);
    virtual Shape *scale(double scale, Point center);
    virtual void draw(BitmapFile *file);
    virtual void describe(std::ostream *out);

private:
    std::string path;
    std::vector<unsigned char> data;
    unsigned int width;
    unsigned int height;
    Point left_bottom;
};

#endif // PNG_IMAGE
