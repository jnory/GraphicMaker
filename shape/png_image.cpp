#include <iostream>

#include <lodepng.h>

#include "png_image.hpp"
#include "point.hpp"

PngImage::PngImage(std::string path, Point left_bottom): left_bottom(left_bottom)
{
    auto err = lodepng::decode(this->data, this->width, this->height, path);
    if(err) {
        std::cerr << "Decode error: path=" << path << std::endl;
        this->width = 0;
        this->height = 0;
        return;
    }
}

Shape *PngImage::copy()
{
    // temporary
    return new PngImage(this->path, this->left_bottom);
}

Shape *PngImage::translate(int diff_x, int diff_y)
{
    this->left_bottom.translate(diff_x, diff_y);
    return this;
}

Shape *PngImage::scale(double)
{
    std::cerr << "PngImage::scale not implemented." << std::endl;
    return this;
}

Shape *PngImage::scale(double, Point)
{
    std::cerr << "PngImage::scale not implemented." << std::endl;
    return this;
}

void PngImage::draw(BitmapFile *file, DrawingProperty &)
{
    for(size_t x = 0; x < this->width; x++) {
        for(size_t y = 0; y < this->height; y++) {
            size_t index = (y * this->width + x) * 4;
            size_t to_x = this->left_bottom.x + x;
            size_t to_y = this->left_bottom.y + this->height - y;
            set_color(
                file,
                to_x,
                to_y,
                Color(
                    this->data[index],  // R
                    this->data[index + 1],  // G
                    this->data[index + 2],  // B
                    this->data[index + 3]  // A
                )
            );
        }
    }
}

void PngImage::describe(std::ostream *out)
{
    *out << "LOADPNG " 
         << this->path << " " 
         << this->left_bottom.x << " "
         << this->left_bottom.y << std::endl;
}
