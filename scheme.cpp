#include <sstream>

#include "builtin_functions.h"
#include "error.h"
#include "parser.h"
#include "scheme.h"
#include "tokenizer.h"

Scope::Scope(std::initializer_list<std::pair<std::string, std::shared_ptr<Object>>> list) {
    for (const auto& [name, obj] : list) {
        Define(name, obj);
    }
}

Scope::Scope(std::shared_ptr<Scope> scope) : parent_(scope) {
}

std::shared_ptr<Object> Scope::Get(const std::string& name) {
    auto it = defined_objects_.find(name);
    if (it == defined_objects_.end()) {
        if (parent_ == nullptr) {
            throw NameError("Unknown variable : " + name);
        }
        return parent_->Get(name);
    }
    return it->second;
}

void Scope::Define(const std::string& name, std::shared_ptr<Object> obj) {
    defined_objects_[name] = obj;
}

void Scope::Set(const std::string& name, std::shared_ptr<Object> obj) {
    auto it = defined_objects_.find(name);
    if (it == defined_objects_.end()) {
        if (parent_ == nullptr) {
            throw NameError("Unknown variable : " + name);
        }
        parent_->Set(name, obj);
        return;
    }
    it->second = obj;
}

std::shared_ptr<Scope> GetGlobalScope() {
    static std::shared_ptr<Scope> global_scope = std::make_shared<Scope>(
        std::initializer_list<std::pair<std::string, std::shared_ptr<Object>>>{
            {"quote", std::make_shared<Quote>()},
            {"number?", std::make_shared<IsNumber>()},
            {"=", std::make_shared<Equal>()},
            {">", std::make_shared<Greater>()},
            {"<", std::make_shared<Less>()},
            {"<=", std::make_shared<NotGreater>()},
            {">=", std::make_shared<NotLess>()},
            {"+", std::make_shared<Sum>()},
            {"-", std::make_shared<Subtraction>()},
            {"*", std::make_shared<Product>()},
            {"/", std::make_shared<Division>()},
            {"max", std::make_shared<Maximum>()},
            {"min", std::make_shared<Minimum>()},
            {"abs", std::make_shared<Absolute>()},
            {"pair?", std::make_shared<IsPair>()},
            {"null?", std::make_shared<IsNull>()},
            {"list?", std::make_shared<IsList>()},
            {"cons", std::make_shared<MakePair>()},
            {"car", std::make_shared<Front>()},
            {"cdr", std::make_shared<AfterFront>()},
            {"list", std::make_shared<MakeList>()},
            {"list-ref", std::make_shared<GetListElement>()},
            {"list-tail", std::make_shared<GetListTail>()},
            {"boolean?", std::make_shared<IsBoolean>()},
            {"not", std::make_shared<LogicalNot>()},
            {"and", std::make_shared<LogicalAnd>()},
            {"or", std::make_shared<LogicalOr>()},
            {"if", std::make_shared<If>()},
            {"define", std::make_shared<Define>()},
            {"set!", std::make_shared<Set>()},
            {"set-car!", std::make_shared<SetFront>()},
            {"set-cdr!", std::make_shared<SetTail>()},
            {"lambda", std::make_shared<MakeLambda>()},
            {"symbol?", std::make_shared<IsSymbol>()}});
    return global_scope;
}

std::string Interpreter::Run(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};
    std::shared_ptr<Object> result = Read(&tokenizer);
    if (!tokenizer.IsEnd()) {
        throw SyntaxError("Input isn't one whole object");
    }
    return ToString(Calculate(result, scope_));
}

std::shared_ptr<Object> Interpreter::Calculate(std::shared_ptr<Object> obj,
                                               std::shared_ptr<Scope> scope) {
    if (obj == nullptr) {
        throw RuntimeError("List doesn't return any value");
    }
    if (Is<Number>(obj) || Is<Boolean>(obj)) {
        return obj;
    }
    if (Is<Symbol>(obj)) {
        return scope->Get(As<Symbol>(obj)->GetName());
    }
    if (Is<Cell>(obj)) {
        auto func = Calculate(As<Cell>(obj)->GetFirst(), scope);
        if (!Is<Function>(func)) {
            throw RuntimeError("List doesn't return any value");
        }
        return As<Function>(func)->Invoke(As<Cell>(obj), scope);
    }
    throw RuntimeError("Unknown object");
}

std::string Interpreter::ToString(std::shared_ptr<Object> obj) {
    if (obj == nullptr) {
        return "()";
    }
    if (Is<Number>(obj)) {
        return std::to_string(As<Number>(obj)->GetValue());
    }
    if (Is<Boolean>(obj)) {
        return As<Boolean>(obj)->GetValue() ? "#t" : "#f";
    }
    if (Is<Symbol>(obj)) {
        return As<Symbol>(obj)->GetName();
    }
    if (Is<Cell>(obj)) {
        std::vector<std::shared_ptr<Object>> list{CellToVector(As<Cell>(obj))};
        std::string ans = "(";
        for (size_t i = 0; i + 1 != list.size(); ++i) {
            ans += ToString(list[i]) + " ";
        }
        if (list.back() == nullptr) {
            ans.back() = ')';
        } else {
            ans += ". " + ToString(list.back()) + ")";
        }
        return ans;
    }
    throw RuntimeError("Function doesn't return any value by themselves");
}