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

