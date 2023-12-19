#include <fstream>
#include <string>

#include "../shape/shape.hpp"
#include "parser.hpp"
#include "reader.hpp"


std::vector<Shape *> read_file(std::string path) {
    std::ifstream reader(path);
    std::vector<Shape *> shapes;
    if(!reader) {
        return shapes;
    }
    std::string line;

    while(std::getline(reader, line)) {
        auto shape = make_shape_from_text(line);
        shapes.push_back(shape);
    }

    return shapes;
}