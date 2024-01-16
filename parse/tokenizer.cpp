#include <sstream>

#include "tokenizer.hpp"


std::vector<std::string> split_code(const std::string &shape_str)
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

Sentence::Sentence(const std::string &line): line_(line) {
    std::stringstream ss(line);
    while(true) {
        std::string token;
        ss >> token;
        if (ss.fail()) {
            break;
        }
        this->tokens_.emplace_back(token);
    }
}

std::vector<Sentence> tokenize(const std::string &code){
    auto lines = split_code(code);
    std::vector<Sentence> sentences;
    sentences.reserve(lines.size());
    for(auto &line: lines) {
        Sentence s(line);
        if (s.empty()) {
            continue;
        }
        sentences.push_back(s);
    }
    return sentences;
}
