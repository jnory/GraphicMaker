#include <algorithm>
#include<cassert>

#include "../io/bitmap.hpp"
#include "point.hpp"


void set_color(BitmapFile *file, size_t x, size_t y, Color color)
{
    uint8_t *data = file->data; // 画像データの格納先
    size_t width = file->infoHeader.biWidth; // 横幅
    size_t height = file->infoHeader.biHeight; // 高さ
    if (width < x || height < y) {
        return;
    }

    size_t position = (y * width + x) * 3;
    double ratio = (double) color.A / 255.0;
    data[position] = (uint8_t)(data[position] * (1 - ratio) + color.B * ratio);
    data[position + 1] = (uint8_t)(data[position + 1] * (1 - ratio) + color.G * ratio);
    data[position + 2] = (uint8_t)(data[position + 2] * (1 - ratio) + color.R * ratio);
}

Point::Point()
{
    this->x = 0;
    this->y = 0;
}

Point::Point(size_t x, size_t y)
{
    this->x = x;
    this->y = y;
}

Shape *Point::copy()
{
    return new Point(this->x, this->y);
}

Shape *Point::translate(int diff_x, int diff_y)
{
    this->x += diff_x;
    this->y += diff_y;
    return this;
}

Shape *Point::scale(double scale)
{
    this->x = (size_t)(this->x * scale);
    this->y = (size_t)(this->x * scale);
    return this;
}

Shape *Point::scale(double scale, Point center)
{
    assert(scale > 0);
    if (center.x <= this->x) {
        size_t diff_x = this->x - center.x;
        this->x += (size_t)(diff_x * scale);
    } else {
        size_t diff_x = center.x - this->x;
        this->x -= (size_t)(diff_x * scale);
    }
    if (center.y <= this->y) {
        size_t diff_y = this->y - center.y;
        this->y += (size_t)(diff_y * scale);
    } else {
        size_t diff_y = center.y - this->y;
        this->y -= (size_t)(diff_y * scale);
    }

    return this;
}

void Point::draw(BitmapFile *file, DrawingProperty &prop)
{
    set_color(file, this->x, this->y, prop.color());
}

void Point::describe(std::ostream *out)
{
    *out << "POINT " << this->x << " " << this->y << std::endl;
}

Point Point::copy_translate(int diff_x, int diff_y)
{
    return Point(this->x + diff_x, this->y + diff_y);
}