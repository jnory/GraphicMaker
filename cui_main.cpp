#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "parse/parser.hpp"

struct Opt {
    std::string code;
    std::string output;
    bool err;
};

void print_help(char **argv) {
    std::cout << "[Usage]" << std::endl;
    std::cout << "  " << argv[0] << " source output" << std::endl << std::endl;
    std::cout << "[parameters]" << std::endl;
    std::cout << "  source: path to source file" << std::endl;
    std::cout << "  output: path to bmp file" << std::endl;
}

Opt parse_args(int argc, char **argv) {
    if (argc == 2) {
        return Opt{argv[1], "", true};
    }
    if (argc != 3) {
        print_help(argv);
        return Opt{"", "", true};
    }
    std::string source(argv[1]);
    std::string output(argv[2]);
    return Opt{source, output, false};
}

std::string load_code(const std::string &source) {
    std::istream *ss;
    bool require_delete = false;
    if(source == "-") {
        ss = &std::cin;
    } else {
        ss = new std::ifstream(source);
        require_delete = true;
    }

    std::stringstream code;
    code << ss->rdbuf();
    if(require_delete) {
        delete ss;
    }
    return code.str();
}

int main(int argc, char **argv) {
    auto opt = parse_args(argc, argv);
    if (opt.err) {
        print_help(argv);
        return -1;
    }
    if (opt.code == "-h" || opt.code == "--help") {
        print_help(argv);
        return 0;
    }

    auto code = load_code(opt.code);
    auto block = parse(code);
    BitmapFile file;
    initialize_bmp_file(&file, 512, 512);
    Environment env(&file);
    block->run(env);
    auto save_result = save_bmp_file((char *)opt.output.c_str(), &file);
    if (save_result != 0) {
        std::cerr << "unexpected error: unable to save to file: code=" << save_result << std::endl;
    }

    return 0;
}