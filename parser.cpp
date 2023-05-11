#include "error.h"
#include "parser.h"

#include <iostream>

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    Token cur_token = tokenizer->GetToken();
    if (cur_token == Token{BracketToken::CLOSE}) {
        tokenizer->Next();
        return nullptr;
    }
    std::shared_ptr<Cell> root(new Cell);
    std::shared_ptr<Cell> cur_vertex = root;
    while (!tokenizer->IsEnd()) {
        cur_vertex->SetFirst(Read(tokenizer));
        cur_token = tokenizer->GetToken();
        if (cur_token == Token{BracketToken::CLOSE}) {
            break;
        } else if (cur_token == Token{DotToken{}}) {
            break;
        }
        cur_vertex->SetSecond(std::make_shared<Cell>());
        cur_vertex = As<Cell>(cur_vertex->GetSecond());
    }
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Bracket sequence is not correct");
    }
    if (cur_token == Token{DotToken{}}) {
        tokenizer->Next();
        cur_vertex->SetSecond(Read(tokenizer));
        cur_token = tokenizer->GetToken();
        if (tokenizer->IsEnd() || cur_token != Token{BracketToken::CLOSE}) {
            throw SyntaxError("Scheme pair can only be used at the end of the list");
        }
    }
    if (cur_token != Token{BracketToken::CLOSE}) {
        throw SyntaxError("Bracket sequence is not correct");
    }
    tokenizer->Next();
    return root;
}

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Unexpected end");
    }
    Token cur_token = tokenizer->GetToken();
    tokenizer->Next();
    if (cur_token == Token{BracketToken::OPEN}) {
        return ReadList(tokenizer);
    }
    if (std::holds_alternative<ConstantToken>(cur_token)) {
        return std::make_shared<Number>(std::get<ConstantToken>(cur_token).value);
    }
    if (std::holds_alternative<SymbolToken>(cur_token)) {
        if (std::get<SymbolToken>(cur_token).name == "#t") {
            return std::make_shared<Boolean>(true);
        }
        if (std::get<SymbolToken>(cur_token).name == "#f") {
            return std::make_shared<Boolean>(false);
        }
        return std::make_shared<Symbol>(std::get<SymbolToken>(cur_token).name);
    }
    if (std::holds_alternative<QuoteToken>(cur_token)) {
        return std::make_shared<Cell>(std::make_shared<Symbol>("quote"),
                                      std::make_shared<Cell>(Read(tokenizer), nullptr));
    }
    if (std::holds_alternative<DotToken>(cur_token)) {
        throw SyntaxError("Unexpected dot");
    }
    if (cur_token == Token{BracketToken::CLOSE}) {
        throw SyntaxError("Bracket sequence is not correct");
    }
    throw SyntaxError("Unknown token");
}