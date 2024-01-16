#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "parser.hpp"
#include "tokenizer.hpp"
#include "../shape/point.hpp"
#include "../shape/line.hpp"
#include "../shape/rectangle.hpp"
#include "../shape/square.hpp"
#include "../shape/triangle.hpp"
#include "../shape/equilateral_triangle.hpp"
#include "../shape/regular_polygon.hpp"
#include "../shape/png_image.hpp"
#include "../shape/circle.hpp"


Shape *make_point_from_text(const Sentence &s)
{
    auto tokens = s.tokens();
    try {
        auto x = tokens[1].get<size_t>();
        auto y = tokens[2].get<size_t>();
        return new Point(x, y);
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_line_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto x1 = tokens[1].get<size_t>();
        auto y1 = tokens[2].get<size_t>();
        auto x2 = tokens[3].get<size_t>();
        auto y2 = tokens[4].get<size_t>();
        return new Line(Point(x1, y1), Point(x2, y2));
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_square_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto x = tokens[1].get<size_t>();
        auto y = tokens[2].get<size_t>();
        auto width = tokens[3].get<size_t>();
        return new Square(Point(x, y), width);
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_triangle_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto x1 = tokens[1].get<size_t>();
        auto y1 = tokens[2].get<size_t>();
        auto x2 = tokens[3].get<size_t>();
        auto y2 = tokens[4].get<size_t>();
        auto x3 = tokens[5].get<size_t>();
        auto y3 = tokens[6].get<size_t>();
        return new Triangle(Point(x1, y1), Point(x2, y2), Point(x3, y3));
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_equilateral_triangle_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto x = tokens[1].get<size_t>();
        auto y = tokens[2].get<size_t>();
        auto side = tokens[3].get<size_t>();
        auto theta = tokens[4].get<double>();
        return new EquilateralTriangle(Point(x, y), side, theta);
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_rectangle_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto left_bottom_x = tokens[1].get<size_t>();
        auto left_bottom_y = tokens[2].get<size_t>();
        auto width = tokens[3].get<size_t>();
        auto height = tokens[4].get<size_t>();
        return new Rectangle(Point(left_bottom_x, left_bottom_y), width, height);
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_regular_polygon_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto n = tokens[1].get<size_t>();
        auto center_x = tokens[2].get<size_t>();
        auto center_y = tokens[3].get<size_t>();
        auto r = tokens[4].get<size_t>();
        return new RegularPolygon(n, Point(center_x, center_y), r);
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_circle_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto center_x = tokens[1].get<size_t>();
        auto center_y = tokens[2].get<size_t>();
        auto r = tokens[3].get<size_t>();
        return new Circle(Point(center_x, center_y), r);
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}


Shape *make_png_image_from_text(const Sentence& s)
{
    auto tokens = s.tokens();
    try {
        auto path = tokens[1].get<std::string>();
        auto x = tokens[2].get<size_t>();
        auto y = tokens[3].get<size_t>();
        return new PngImage(path, Point(x, y));
    } catch (std::runtime_error &) {
        std::cerr << "Parse error: source =" << s.source() << std::endl;
        return nullptr;
    }
}

Shape *make_shape_from_text(const Sentence &s)
{
    auto &tokens = s.tokens();
    auto first = tokens[0].get<std::string>();
    if (first == "POINT") {
        return make_point_from_text(s);
    } else if (first == "LINE") {
        return make_line_from_text(s);
    } else if (first == "SQUARE") {
        return make_square_from_text(s);
    } else if (first == "TRIANGLE") {
        return make_triangle_from_text(s);
    } else if (first == "EQUILATERAL_TRIANGLE") {
        return make_equilateral_triangle_from_text(s);
    } else if (first == "RECTANGLE") {
        return make_rectangle_from_text(s);
    } else if (first == "REGULAR_POLYGON") {
        return make_regular_polygon_from_text(s);
    } else if (first == "CIRCLE") {
        return make_circle_from_text(s);
    } else if (first == "LOADPNG") {
        return make_png_image_from_text(s);
    } else {
        std::cerr << "Unsupported: source=" << s.source() << std::endl;
        return nullptr;
    }
}

std::vector<Shape *> parse(const std::string &shape_str)
{
    auto sentences = tokenize(shape_str);
    std::vector<Shape *> shapes;
    for(auto &sentence: sentences) {
        auto s = make_shape_from_text(sentence);
        if (s != nullptr) {
            shapes.push_back(s);
        }
    }
    return shapes;
}