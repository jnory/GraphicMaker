#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "../io/bitmap.hpp"

class Environment {
public:
    BitmapFile *get_file() {
        return this->file_;
    }

private:
    BitmapFile *file_;
};

#endif // ENVIRONMENT_HPP
