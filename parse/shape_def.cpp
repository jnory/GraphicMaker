#include <cassert>

#include "../shape/point.hpp"
#include "../shape/line.hpp"
#include "../shape/rectangle.hpp"
#include "../shape/square.hpp"
#include "../shape/triangle.hpp"
#include "../shape/equilateral_triangle.hpp"
#include "../shape/regular_polygon.hpp"
#include "../shape/png_image.hpp"
#include "../shape/circle.hpp"
#include "shape_def.hpp"
#include "statement.hpp"


Shape *build_point(Parameters &params, Environment &env)
{
    assert(params.size() == 2);
    auto x = params[0]->get_value(env).get<size_t>();
    auto y = params[1]->get_value(env).get<size_t>();
    return new Point(x, y);
}

Shape *build_line(Parameters &params, Environment &env)
{
    assert(params.size() == 4);
    auto x1 = params[0]->get_value(env).get<size_t>();
    auto y1 = params[1]->get_value(env).get<size_t>();
    auto x2 = params[2]->get_value(env).get<size_t>();
    auto y2 = params[3]->get_value(env).get<size_t>();
    return new Line(Point(x1, y1), Point(x2, y2));
}

Shape *build_square(Parameters &params, Environment &env)
{
    assert(params.size() == 3);
    auto x = params[0]->get_value(env).get<size_t>();
    auto y = params[1]->get_value(env).get<size_t>();
    auto width = params[2]->get_value(env).get<size_t>();
    return new Square(Point(x, y), width);
}

Shape *build_triangle(Parameters &params, Environment &env)
{
    assert(params.size() == 6);
    auto x1 = params[0]->get_value(env).get<size_t>();
    auto y1 = params[1]->get_value(env).get<size_t>();
    auto x2 = params[2]->get_value(env).get<size_t>();
    auto y2 = params[3]->get_value(env).get<size_t>();
    auto x3 = params[4]->get_value(env).get<size_t>();
    auto y3 = params[5]->get_value(env).get<size_t>();
    return new Triangle(Point(x1, y1), Point(x2, y2), Point(x3, y3));
}

Shape *build_equilateral_triangle(Parameters &params, Environment &env)
{
    assert(params.size() == 4);
    auto x = params[0]->get_value(env).get<size_t>();
    auto y = params[1]->get_value(env).get<size_t>();
    auto side = params[2]->get_value(env).get<size_t>();
    auto theta = params[3]->get_value(env).get<double>();
    return new EquilateralTriangle(Point(x, y), side, theta);
}

Shape *build_rectangle(Parameters &params, Environment &env)
{
    assert(params.size() == 4);
    auto left_bottom_x = params[0]->get_value(env).get<size_t>();
    auto left_bottom_y = params[1]->get_value(env).get<size_t>();
    auto width = params[2]->get_value(env).get<size_t>();
    auto height = params[3]->get_value(env).get<size_t>();
    return new Rectangle(Point(left_bottom_x, left_bottom_y), width, height);
}

Shape *build_regular_polygon(Parameters &params, Environment &env)
{
    assert(params.size() == 4);
    auto n = params[0]->get_value(env).get<size_t>();
    auto center_x = params[1]->get_value(env).get<size_t>();
    auto center_y = params[2]->get_value(env).get<size_t>();
    auto r = params[3]->get_value(env).get<size_t>();
    return new RegularPolygon(n, Point(center_x, center_y), r);
}

Shape *build_circle(Parameters &params, Environment &env)
{
    assert(params.size() == 3);
    auto center_x = params[0]->get_value(env).get<size_t>();
    auto center_y = params[1]->get_value(env).get<size_t>();
    auto r = params[2]->get_value(env).get<size_t>();
    return new Circle(Point(center_x, center_y), r);
}


Shape *build_png_image(Parameters &params, Environment &env)
{
    assert(params.size() == 3);
    auto path = params[0]->get_value(env).get<std::string>();
    auto x = params[1]->get_value(env).get<size_t>();
    auto y = params[2]->get_value(env).get<size_t>();
    return new PngImage(path, Point(x, y));
}

bool is_known_shape(const std::string &type) {
    // TODO: improve efficiency
    if (type == "POINT") {
        return true;
    } else if (type == "LINE") {
        return true;
    } else if (type == "SQUARE") {
        return true;
    } else if (type == "TRIANGLE") {
        return true;
    } else if (type == "EQUILATERAL_TRIANGLE") {
        return true;
    } else if (type == "RECTANGLE") {
        return true;
    } else if (type == "REGULAR_POLYGON") {
        return true;
    } else if (type == "CIRCLE") {
        return true;
    } else if (type == "LOADPNG") {
        return true;
    } else {
        return false;
    }
}

Shape *build_shape(const std::string &type, Parameters &parameters, Environment &env) {
    if (type == "POINT") {
        return build_point(parameters, env);
    } else if (type == "LINE") {
        return build_line(parameters, env);
    } else if (type == "SQUARE") {
        return build_square(parameters, env);
    } else if (type == "TRIANGLE") {
        return build_triangle(parameters, env);
    } else if (type == "EQUILATERAL_TRIANGLE") {
        return build_equilateral_triangle(parameters, env);
    } else if (type == "RECTANGLE") {
        return build_rectangle(parameters, env);
    } else if (type == "REGULAR_POLYGON") {
        return build_regular_polygon(parameters, env);
    } else if (type == "CIRCLE") {
        return build_circle(parameters, env);
    } else if (type == "LOADPNG") {
        return build_png_image(parameters, env);
    } else {
        return nullptr;
    }
}

