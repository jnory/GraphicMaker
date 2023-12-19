#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "parser.hpp"
#include "../shape/point.hpp"
#include "../shape/line.hpp"
#include "../shape/square.hpp"
#include "../shape/triangle.hpp"
#include "../shape/equilateral_triangle.hpp"
#include "../shape/rectangle.hpp"
#include "../shape/regular_polygon.hpp"
#include "../shape/png_image.hpp"


std::vector<std::string> split_code(std::string shape_str) 
{
    std::vector<std::string> lines;
    auto it = std::begin(shape_str);
    auto end = std::end(shape_str);
    while(it != end) {
        auto found = std::find(it, end, '\n');
        std::string line(it, found);
        lines.push_back(line);
        if (found == end) {
            break;
        }
        it = found + 1;
    }

    return lines;
}

Shape *make_point_from_text(std::string line) 
{
    std::string params = line.substr(5);
    std::stringstream ss(params);

    size_t x;
    size_t y;
    ss >> x;
    ss >> y;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new Point(x, y);
}

Shape *make_line_from_text(std::string line) 
{
    std::string params = line.substr(4);
    std::stringstream ss(params);

    size_t x1;
    size_t y1;
    size_t x2;
    size_t y2;
    ss >> x1;
    ss >> y1;
    ss >> x2;
    ss >> y2;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new Line(Point(x1, y1), Point(x2, y2));
}

Shape *make_square_from_text(std::string line) 
{
    std::string params = line.substr(6);
    std::stringstream ss(params);

    size_t x;
    size_t y;
    size_t width;
    ss >> x;
    ss >> y;
    ss >> width;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new Square(Point(x, y), width);
}

Shape *make_triangle_from_text(std::string line) 
{
    std::string params = line.substr(9);
    std::stringstream ss(params);

    size_t x1;
    size_t y1;
    size_t x2;
    size_t y2;
    size_t x3;
    size_t y3;
    ss >> x1;
    ss >> y1;
    ss >> x2;
    ss >> y2;
    ss >> x3;
    ss >> y3;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new Triangle(Point(x1, y1), Point(x2, y2), Point(x3, y3));
}

Shape *make_equilateral_triangle_from_text(std::string line) 
{
    std::string params = line.substr(20);
    std::stringstream ss(params);

    size_t x;
    size_t y;
    size_t side;
    double theta;
    ss >> x;
    ss >> y;
    ss >> side;
    ss >> theta;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new EquilateralTriangle(Point(x, y), side, theta);
}

Shape *make_rectangle_from_text(std::string line) 
{
    std::string params = line.substr(9);
    std::stringstream ss(params);

    size_t left_bottom_x;
    size_t left_bottom_y;
    size_t width;
    size_t height;
    ss >> left_bottom_x;
    ss >> left_bottom_y;
    ss >> width;
    ss >> height;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new Rectangle(Point(left_bottom_x, left_bottom_y), width, height);
}

Shape *make_regular_polygon_from_text(std::string line) 
{
    std::string params = line.substr(15);
    std::stringstream ss(params);

    size_t n;
    size_t center_x;
    size_t center_y;
    size_t r;

    ss >> n;
    ss >> center_x;
    ss >> center_y;
    ss >> r;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new RegularPolygon(n, Point(center_x, center_y), r);
}


Shape *make_png_image_from_text(std::string line) 
{
    std::string params = line.substr(7);
    std::stringstream ss(params);

    std::string path;
    size_t x;
    size_t y;

    ss >> path;
    ss >> x;
    ss >> y;
    if (ss.fail()) {
        printf("Parse error: params=%s\n", params.c_str());
        return NULL;
    }

    return new PngImage(path, Point(x, y));
}

Shape *make_shape_from_text(std::string &line)
{
    if(line.size() == 0) {
        return NULL;
    }

    if (line.substr(0, 5) == "POINT") {
        return make_point_from_text(line);
    } else if (line.substr(0, 4) == "LINE") {
        return make_line_from_text(line);
    } else if (line.substr(0, 6) == "SQUARE") {
        return make_square_from_text(line);
    } else if (line.substr(0, 8) == "TRIANGLE") {
        return make_triangle_from_text(line);
    } else if (line.substr(0, 20) == "EQUILATERAL_TRIANGLE") {
        return make_equilateral_triangle_from_text(line);
    } else if (line.substr(0, 9) == "RECTANGLE") {
        return make_rectangle_from_text(line);
    } else if (line.substr(0, 15) == "REGULAR_POLYGON") {
        return make_regular_polygon_from_text(line);
    } else if (line.substr(0, 7) == "LOADPNG") {
        return make_png_image_from_text(line);
    } else {
        printf("Unsupported: line=%s\n", line.c_str());
        return NULL;
    }
}

std::vector<Shape *> parse(std::string shape_str)
{
    // ここで文字列を解釈してShape型データを作り、配列にまとめて返します。
    std::vector<std::string> lines;
    lines = split_code(shape_str);

    
    std::vector<Shape *> shapes;
    for(std::string &line: lines) {
        auto s = make_shape_from_text(line);
        if (s != NULL) {
            shapes.push_back(s);
        }
    }

    return shapes;
}