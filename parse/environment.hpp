#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

#include "../io/bitmap.hpp"
#include "../shape/shape.hpp"

enum ImmediateType {
    IM_INTEGER,
    IM_FLOAT,
    // IM_STRING, // TODO: implement
};

union ImmediateValue {
    int64_t integer;
    double floating;
};

class Immediate {
public:
    explicit Immediate(int64_t value): type_(IM_INTEGER), value_({.integer = value}){};
    explicit Immediate(double value): type_(IM_FLOAT), value_({.floating = value}){};

    template<typename T>
    T get() const {
        throw std::runtime_error("unexpected type");
    }

    template<> int get() const {
        assert(type_ == IM_INTEGER);
        // TODO: check value range
        return this->value_.integer;
    }

    template<> int64_t get() const {
        if (type_ == IM_INTEGER) {
            return this->value_.integer;
        } else {
            return (int64_t) this->value_.floating;
        }
    }

    template<> double get() const {
        if(type_ == IM_FLOAT) {
            return this->value_.floating;
        } else {
            return (double) this->value_.integer;
        }
    }

    template<> size_t get() const {
        // TODO: check value range
        if (type_ == IM_INTEGER) {
            return (size_t) this->value_.integer;
        } else {
            assert(type_ == IM_FLOAT);
            return (size_t) this->value_.floating;
        }
    }

    Immediate operator+(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() + other.get_as_float();
            return Immediate(value);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() + other.get_as_integer();
            return Immediate(value);
        }
    }

    Immediate operator-(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() - other.get_as_float();
            return Immediate(value);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() - other.get_as_integer();
            return Immediate(value);
        }
    }

    Immediate operator*(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() * other.get_as_float();
            return Immediate(value);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() * other.get_as_integer();
            return Immediate(value);
        }
    }

    Immediate operator/(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() / other.get_as_float();
            return Immediate(value);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() / other.get_as_integer();
            return Immediate(value);
        }
    }

    Immediate operator>(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() > other.get_as_float();
            return Immediate(value?1ll:0ll);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() > other.get_as_integer();
            return Immediate(value?1ll:0ll);
        }
    }

    Immediate operator<(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() < other.get_as_float();
            return Immediate(value?1ll:0ll);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() < other.get_as_integer();
            return Immediate(value?1ll:0ll);
        }
    }

    Immediate operator==(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() == other.get_as_float();
            return Immediate(value?1ll:0ll);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() == other.get_as_integer();
            return Immediate(value?1ll:0ll);
        }
    }

    Immediate operator<=(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() <= other.get_as_float();
            return Immediate(value?1ll:0ll);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() <= other.get_as_integer();
            return Immediate(value?1ll:0ll);
        }
    }

    Immediate operator>=(Immediate &other) {
        if (type_ == IM_FLOAT || other.type_ == IM_FLOAT) {
            auto value = get_as_float() >= other.get_as_float();
            return Immediate(value?1ll:0ll);
        } else {
            assert(type_ == IM_INTEGER);
            assert(other.type_ == IM_INTEGER);
            auto value = get_as_integer() >= other.get_as_integer();
            return Immediate(value?1ll:0ll);
        }
    }

private:
    double get_as_float() {
        if(type_ == IM_INTEGER) {
            return (double) value_.integer;
        } else {
            assert(type_ == IM_FLOAT);
            return value_.floating;
        }
    }

    int64_t get_as_integer() {
        if(type_ == IM_INTEGER) {
            return value_.integer;
        } else {
            assert(type_ == IM_FLOAT);
            return (int64_t) value_.floating;
        }
    }

    ImmediateType type_;
    ImmediateValue value_;
};


class Environment {
public:
    explicit Environment(BitmapFile *file): file_(file), color_(0, 0, 0) {}

    BitmapFile *get_file() {
        return this->file_;
    }

    Immediate lookup_variable(std::string &name) {
        auto found = this->variables_.find(name);
        if (found == std::end(this->variables_)) {
            throw std::runtime_error("variable " + name + " not found");
        }
        return found->second;
    }

    Immediate set_variable(std::string &name, Immediate &value) {
        auto result = this->variables_.insert({name, value});
        if(!result.second) {
            result.first->second = value;
        }
        return value;
    }

    void debug_print() {
        std::cerr << "===" << std::endl;
        for(auto &p: variables_) {
            std::cerr << p.first << "|||" << p.second.get<int>() << std::endl;
        }
    }

    Color get_color() const {
        return color_;
    }

    void set_color(Color color) {
        color_ = color;
    }

private:
    BitmapFile *file_;
    Color color_;
    std::unordered_map<std::string, Immediate> variables_;
};

#endif // ENVIRONMENT_HPP
