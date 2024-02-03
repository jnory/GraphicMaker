#include <sstream>

#include "tokenizer.hpp"

const Token TokenIterator::eos_ = Token("EOS", true);


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
            is_previous_space = false;
        } else if (c == '(' || c == ')' || c == '{' || c == '}') {
            found = true;
            force_found = true;
            is_previous_operator = false;
            is_previous_space = false;
        } else if (c == ' ' || c == '\t') {
            if(!is_previous_space) {
                found = true;
            } else {
                previous = i;
            }
            is_space = true;
        }

        if(found) {
            if(is_previous_space) {
                previous++;
            }
            if(previous < i) {
                auto token = line.substr(previous, i);
                this->tokens_.emplace_back(token);
            }
            previous = i;
        }
        is_previous_space = is_space;
    }
    if(is_previous_space) {
        previous++;
    }
    if(previous < line.size()) {
        auto token = line.substr(previous);
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

TokenIterator::TokenIterator(const std::vector<Sentence> &&sentences)
    : sentences_(sentences), head_sentence_(0), token_idx_(0) {}

const Token &TokenIterator::look_ahead(bool skip_eos) const {
    size_t i = this->head_sentence_;
    size_t j = this->token_idx_;
    auto token = &this->get_(i, j);
    if (!skip_eos){
        return *token;
    }

    while (token->is_eos()) {
        i++;
        j = 0;
        if(i >= this->sentences_.size()) {
            return eos_;
        }
        token = &this->get_(i, j);
    }
    return *token;
}

const Token &TokenIterator::next(bool skip_eos) {
    // TODO: improve efficiency
    auto &token = this->look_ahead(skip_eos);
    this->succ(skip_eos);
    return token;
}

void TokenIterator::succ(bool skip_eos) {
    if (!skip_eos){
        this->token_idx_++;
        return;
    }

    size_t i = this->head_sentence_;
    size_t j = this->token_idx_;
    auto token = &this->get_(i, j);
    while (token->is_eos()) {
        i++;
        j = 0;
        token = &this->get_(i, j);
    }
    this->head_sentence_ = i;
    this->token_idx_ = j;
}

