#include "error.h"
#include "builtin_functions.h"
#include "object.h"

#include <iostream>

Number::Number(int64_t value) : value_(value) {
}

int64_t Number::GetValue() const {
    return value_;
}

Boolean::Boolean(bool b) : value_(b) {
}

bool Boolean::GetValue() {
    return value_;
}

Symbol::Symbol(const std::string& s) : name_(s) {
}

const std::string& Symbol::GetName() const {
    return name_;
}

Cell::Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
    : first_(first), second_(second) {
}

void Cell::SetFirst(std::shared_ptr<Object> shd_ptr) {
    first_ = shd_ptr;
}

void Cell::SetSecond(std::shared_ptr<Object> shd_ptr) {
    second_ = shd_ptr;
}

std::shared_ptr<Object> Cell::GetFirst() const {
    return first_;
}

std::shared_ptr<Object> Cell::GetSecond() const {
    return second_;
}

std::vector<std::shared_ptr<Object>> CellToVector(std::shared_ptr<Cell> obj) {
    if (obj == nullptr) {
        return {nullptr};
    }
    std::vector<std::shared_ptr<Object>> ans = {obj->GetFirst()};
    std::shared_ptr<Object> ptr = obj->GetSecond();
    while (ptr != nullptr && Is<Cell>(ptr)) {
        ans.emplace_back(As<Cell>(ptr)->GetFirst());
        ptr = As<Cell>(ptr)->GetSecond();
    }
    ans.emplace_back(ptr);
    return ans;
}

std::shared_ptr<Cell> VectorToCell(std::vector<std::shared_ptr<Object>> vec) {
    if (vec == std::vector<std::shared_ptr<Object>>{nullptr}) {
        return nullptr;
    }
    auto root = std::make_shared<Cell>();
    auto cur_vertex = root;
    cur_vertex->SetFirst(vec[0]);
    for (size_t i = 1; i + 1 < vec.size(); ++i) {
        cur_vertex->SetSecond(std::make_shared<Cell>(vec[i], nullptr));
        cur_vertex = As<Cell>(cur_vertex->GetSecond());
    }
    cur_vertex->GetSecond() = vec.back();
    return root;
}

Lambda::Lambda(std::shared_ptr<Cell> args, std::shared_ptr<Cell> commands,
               std::shared_ptr<Scope> scope)
    : parent_(scope) {
    auto args_list = CellToVector(args);
    if (args_list.back() != nullptr) {
        throw RuntimeError("Combination must be a proper list");
    }
    variables_.resize(args_list.size() - 1);
    for (size_t i = 0; i + 1 < args_list.size(); ++i) {
        if (!Is<Symbol>(args_list[i])) {
            throw RuntimeError("Arguments list must only contains names");
        }
        variables_[i] = As<Symbol>(args_list[i])->GetName();
    }
    auto list = CellToVector(commands);
    command_list_.resize(list.size() - 1);
    for (size_t i = 0; i + 1 < list.size(); ++i) {
        command_list_[i] = list[i];
    }
}

std::shared_ptr<Object> Lambda::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    auto args_list = CellToVector(args);
    if (args_list.back() != nullptr) {
        throw RuntimeError("Combination must be a proper list");
    }
    if (args_list.size() - 2 != variables_.size()) {
        throw RuntimeError("The amount of given arguments doesn't much the amount of requiring");
    }
    std::shared_ptr<Scope> lambda_scope(std::make_shared<Scope>(parent_));
    for (size_t i = 1; i + 1 < args_list.size(); ++i) {
        lambda_scope->Define(variables_[i - 1], Interpreter::Calculate(args_list[i], scope));
    }
    std::shared_ptr<Object> result = nullptr;
    for (size_t i = 0; i < command_list_.size(); ++i) {
        result = Interpreter::Calculate(command_list_[i], lambda_scope);
    }
    return result;
}
