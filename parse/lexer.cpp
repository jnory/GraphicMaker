#include <sstream>

#include "lexer.hpp"

const Token TokenIterator::eos_ = Token("<EOS>", true);


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
    size_t previous = 0;
    bool is_previous_operator = false;
    bool is_previous_space = false;
    bool force_found = false;
    for(size_t i = 1; i < line.size(); i++) {
        bool found = false;
        bool is_space = false;
        char c = line[i];
        if(force_found) {
            found = true;
            force_found = false;
        }

        if(c == '=' || c == '>' || c == '<' || c == '+' || c == '-' || c == '*' || c == '/') {
            if(!is_previous_operator) {
                found = true;
            }
            is_previous_operator = true;
        } else if (c == '(' || c == ')' || c == '{' || c == '}') {
            found = true;
            force_found = true;
            is_previous_operator = false;
        } else if (c == ' ' || c == '\t') {
            found = true;
            is_space = true;
        } else if (is_previous_operator) {
            found = true;
            is_previous_operator = false;
        }

        if(found) {
            // std::cerr << "* previous=" << previous << " i=" << i << " c=" << c
            //           << " is_space=" << is_space << " is_previous_operator=" << is_previous_operator
            //           << " is_previous_space=" << is_previous_space << " force_found=" << force_found << std::endl;
            if(is_previous_space) {
                previous++;
            }
            if(previous < i) {
                auto token = line.substr(previous, (i - previous));
                this->tokens_.emplace_back(token);
            }
            previous = i;
            is_previous_space = is_space;
        }
    }
    // std::cerr << "* previous=" << previous
    //           << " is_previous_operator=" << is_previous_operator
    //           << " is_previous_space=" << is_previous_space << " force_found=" << force_found << std::endl;
    if(is_previous_space) {
        previous++;
    }
    if(previous < line.size()) {
        auto token = line.substr(previous);
        this->tokens_.emplace_back(token);
    }
}

std::vector<Sentence> lex(const std::string &code){
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

TokenIterator::TokenIterator(const std::vector<Sentence> &&sentences)
    : sentences_(sentences), head_(0), last_(0)
{
    for(auto &sentence: sentences) {
        auto &tokens = sentence.tokens();
        for(const auto& token: tokens) {
            this->tokens_.push_back(token);
        }
        this->tokens_.push_back(TokenIterator::eos_);
    }
}

const Token &TokenIterator::look_ahead(bool skip_eos) {
    size_t i = this->head_;
    while(skip_eos && this->tokens_[i].is_eos()) {
        i++;
    }
    this->last_ = i;
    return this->tokens_[i];
}

const Token &TokenIterator::next(bool skip_eos) {
    // TODO: improve efficiency
    auto &token = this->look_ahead(skip_eos);
    this->succ();
    return token;
}

void TokenIterator::succ() {
    this->last_++;
    this->head_ = this->last_;
}

