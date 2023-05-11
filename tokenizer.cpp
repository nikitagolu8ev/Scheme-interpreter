#include "tokenizer.h"
#include "error.h"

SymbolToken::SymbolToken(const std::string& str) : name(str) {
}

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

ConstantToken::ConstantToken(int64_t number) : value(number) {
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* in) : input_stream_(*in) {
    Next();
}

bool Tokenizer::IsBlank(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void Tokenizer::ToTokenBegin() {
    while (input_stream_ && IsBlank(input_stream_.peek())) {
        input_stream_.get();
    }
}

bool Tokenizer::IsEnd() {
    return is_end_;
}

int64_t Tokenizer::ReadNumber() {
    int64_t number = 0;
    while (std::isdigit(input_stream_.peek())) {
        number = number * 10 + (input_stream_.get() - '0');
    }
    return number;
}

bool Tokenizer::IsSymbolBegin(char c) {
    return std::isalpha(c) || c == '<' || c == '=' || c == '>' || c == '*' || c == '/' || c == '#';
}

bool Tokenizer::IsSymbolMiddle(char c) {
    return IsSymbolBegin(c) || std::isdigit(c) || c == '?' || c == '!' || c == '-';
}

void Tokenizer::Next() {
    ToTokenBegin();
    if (input_stream_.peek() == EOF) {
        is_end_ = true;
        return;
    }
    char first_symbol = input_stream_.get();
    if (first_symbol == '\'') {
        last_read_token_ = QuoteToken{};
        return;
    }
    if (first_symbol == '.') {
        last_read_token_ = DotToken{};
        return;
    }
    if (first_symbol == '(') {
        last_read_token_ = BracketToken::OPEN;
        return;
    }
    if (first_symbol == ')') {
        last_read_token_ = BracketToken::CLOSE;
        return;
    }
    if (first_symbol == '-' || first_symbol == '+') {
        if (std::isdigit(input_stream_.peek())) {
            if (first_symbol == '-') {
                last_read_token_ = ConstantToken{-ReadNumber()};
            } else {
                last_read_token_ = ConstantToken{ReadNumber()};
            }
        } else {
            last_read_token_ = SymbolToken{{first_symbol}};
        }
        return;
    }
    if (std::isdigit(first_symbol)) {
        input_stream_.unget();
        last_read_token_ = ConstantToken{ReadNumber()};
        return;
    }
    if (IsSymbolBegin(first_symbol)) {
        std::string name = {first_symbol};
        while (IsSymbolMiddle(input_stream_.peek())) {
            name.push_back(input_stream_.get());
        }
        last_read_token_ = SymbolToken{name};
        return;
    }
    throw SyntaxError("Can't identify token type");
}

Token Tokenizer::GetToken() {
    return last_read_token_;
}
