#pragma once

#include <istream>
#include <optional>
#include <variant>

struct SymbolToken {
    std::string name;

    SymbolToken(const std::string& str);
    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int64_t value;

    ConstantToken(int64_t number);
    bool operator==(const ConstantToken& other) const;
};

using Token = std::variant<QuoteToken, ConstantToken, BracketToken, SymbolToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd();
    void Next();
    Token GetToken();

private:
    bool IsBlank(char c);
    bool IsSymbolBegin(char c);
    bool IsSymbolMiddle(char c);

    void ToTokenBegin();
    int64_t ReadNumber();

    std::istream& input_stream_;
    Token last_read_token_;
    bool is_end_ = false;
};