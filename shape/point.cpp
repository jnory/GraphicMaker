#include <algorithm>

#include "../io/bitmap.hpp"
#include "point.hpp"


void set_pixel(BitmapFile *file, size_t x, size_t y, uint8_t brightness) 
{
    uint8_t *data = file->data; // 画像データの格納先
    size_t width = file->infoHeader.biWidth; // 横幅
    size_t height = file->infoHeader.biHeight; // 高さ
    if (width < x || height < y) {
        return;
    }

    size_t position = (y * width + x) * 3;
    data[position] = std::min<size_t>(data[position], brightness);
    data[position + 1] = std::min<size_t>(data[position + 1], brightness);
    data[position + 2] = std::min<size_t>(data[position + 2], brightness);
}

void set_color(BitmapFile *file, size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint8_t *data = file->data; // 画像データの格納先
    size_t width = file->infoHeader.biWidth; // 横幅
    size_t height = file->infoHeader.biHeight; // 高さ
    if (width < x || height < y) {
        return;
    }

    size_t position = (y * width + x) * 3;
    double ratio = (double) a / 255.0;
    data[position] = data[position] * (1 - ratio) + b * ratio;
    data[position + 1] = data[position + 1] * (1 - ratio) + g * ratio;
    data[position + 2] = data[position + 2] * (1 - ratio) + r * ratio;
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
    this->x *= scale;
    this->y *= scale;
    return this;
}

Shape *Point::scale(double scale, Point center)
{
    if (center.x <= this->x) {
        size_t diff_x = this->x - center.x;
        this->x += diff_x * scale;
    } else {
        size_t diff_x = center.x - this->x;
        this->x -= diff_x * scale;
    }
    if (center.y <= this->y) {
        size_t diff_y = this->y - center.y;
        this->y += diff_y * scale;
    } else {
        size_t diff_y = center.y - this->y;
        this->y -= diff_y * scale;
    }

    return this;
}

void Point::draw(BitmapFile *file)
{
    set_pixel(file, this->x, this->y, 0);
}

void Point::describe(std::ostream *out)
{
    *out << "POINT " << this->x << " " << this->y << std::endl;
}

Point Point::copy_translate(int diff_x, int diff_y)
{
    return Point(this->x + diff_x, this->y + diff_y);
}