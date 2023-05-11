#include <algorithm>

#include "error.h"
#include "builtin_functions.h"
#include "scheme.h"

#define CHECKER(args, scope, args_list)  \
    auto args_list = CellToVector(args); \
    CheckList(args_list);                \
    CheckTypes(args_list, scope)

void DefaultListChecker::CheckList(std::vector<std::shared_ptr<Object>>& args_list) {
    if (args_list.back() != nullptr) {
        throw RuntimeError("Combination must be a proper list");
    }
}

void UnaryFunctionChecker::CheckList(std::vector<std::shared_ptr<Object>>& args_list) {
    if (args_list.back() != nullptr) {
        throw RuntimeError("Combination must be a proper list");
    }
    if (args_list.size() != 3) {
        throw RuntimeError("Unary function requires exactly one argument");
    }
}

void BinaryFunctionChecker::CheckList(std::vector<std::shared_ptr<Object>>& args_list) {
    if (args_list.back() != nullptr) {
        throw RuntimeError("Combination must be a proper list");
    }
    if (args_list.size() != 4) {
        throw RuntimeError("Binary function requires exactly two arguments");
    }
}

void NonEmptyListChecker::CheckList(std::vector<std::shared_ptr<Object>>& args_list) {
    if (args_list.back() != nullptr) {
        throw RuntimeError("Combination must be a proper list");
    }
    if (args_list.size() < 3) {
        throw RuntimeError("Function requires at least one argument");
    }
}

void DefaultTypeChecker::CheckTypes(std::vector<std::shared_ptr<Object>>&, std::shared_ptr<Scope>) {
}

void IntegerTypeChecker::CheckTypes(std::vector<std::shared_ptr<Object>>& args_list,
                                    std::shared_ptr<Scope> scope) {
    for (size_t i = 1; i + 1 < args_list.size(); ++i) {
        args_list[i] = Interpreter::Calculate(args_list[i], scope);
        if (!Is<Number>(args_list[i])) {
            throw RuntimeError("Function requires integer only arguments");
        }
    }
}

std::shared_ptr<Object> Quote::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    return CellToVector(args)[1];
}

std::shared_ptr<Object> IsNumber::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    return std::make_shared<Boolean>(
        Is<Number>(Interpreter::Calculate(CellToVector(args)[1], scope)));
}

std::shared_ptr<Object> Equal::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        if (As<Number>(args_list[i - 1])->GetValue() != As<Number>(args_list[i])->GetValue()) {
            return std::make_shared<Boolean>(false);
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> Greater::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        if (As<Number>(args_list[i - 1])->GetValue() <= As<Number>(args_list[i])->GetValue()) {
            return std::make_shared<Boolean>(false);
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> Less::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        if (As<Number>(args_list[i - 1])->GetValue() >= As<Number>(args_list[i])->GetValue()) {
            return std::make_shared<Boolean>(false);
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> NotGreater::Invoke(std::shared_ptr<Cell> args,
                                           std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        if (As<Number>(args_list[i - 1])->GetValue() > As<Number>(args_list[i])->GetValue()) {
            return std::make_shared<Boolean>(false);
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> NotLess::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        if (As<Number>(args_list[i - 1])->GetValue() < As<Number>(args_list[i])->GetValue()) {
            return std::make_shared<Boolean>(false);
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> Sum::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    int64_t result = 0;
    for (size_t i = 1; i + 1 < args_list.size(); ++i) {
        result += As<Number>(args_list[i])->GetValue();
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> Subtraction::Invoke(std::shared_ptr<Cell> args,
                                            std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    int64_t result = As<Number>(args_list[1])->GetValue();
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        result -= As<Number>(args_list[i])->GetValue();
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> Product::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    int64_t result = 1;
    for (size_t i = 1; i + 1 < args_list.size(); ++i) {
        result *= As<Number>(args_list[i])->GetValue();
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> Division::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    int64_t result = As<Number>(args_list[1])->GetValue();
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        result /= As<Number>(args_list[i])->GetValue();
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> Maximum::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    int64_t result = As<Number>(args_list[1])->GetValue();
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        result = std::max(result, As<Number>(args_list[i])->GetValue());
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> Minimum::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    int64_t result = As<Number>(args_list[1])->GetValue();
    for (size_t i = 2; i + 1 < args_list.size(); ++i) {
        result = std::min(result, As<Number>(args_list[i])->GetValue());
    }
    return std::make_shared<Number>(result);
}

std::shared_ptr<Object> Absolute::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    return std::make_shared<Number>(abs(As<Number>(args_list[1])->GetValue()));
}

std::shared_ptr<Object> IsPair::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    auto to_check = Interpreter::Calculate(args_list[1], scope);
    if (!Is<Cell>(to_check)) {
        return std::make_shared<Boolean>(false);
    }
    auto as_vector = CellToVector(As<Cell>(to_check));
    if ((as_vector.size() == 2 && as_vector.back() != nullptr) ||
        (as_vector.size() == 3 && as_vector.back() == nullptr)) {
        return std::make_shared<Boolean>(true);
    }
    return std::make_shared<Boolean>(false);
}

std::shared_ptr<Object> IsNull::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    return std::make_shared<Boolean>(Interpreter::Calculate(args_list[1], scope) == nullptr);
}

std::shared_ptr<Object> IsList::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    args_list[1] = Interpreter::Calculate(args_list[1], scope);
    if (args_list[1] == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    if (!Is<Cell>(args_list[1])) {
        return std::make_shared<Boolean>(false);
    }
    if (CellToVector(As<Cell>(args_list[1])).back() != nullptr) {
        return std::make_shared<Boolean>(false);
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> MakePair::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    return std::make_shared<Cell>(Interpreter::Calculate(args_list[1], scope),
                                  Interpreter::Calculate(args_list[2], scope));
}

std::shared_ptr<Object> Front::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    args_list[1] = Interpreter::Calculate(args_list[1], scope);
    if (!Is<Cell>(args_list[1])) {
        throw RuntimeError("Function requires only not empty list");
    }
    return As<Cell>(args_list[1])->GetFirst();
}

std::shared_ptr<Object> AfterFront::Invoke(std::shared_ptr<Cell> args,
                                           std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    args_list[1] = Interpreter::Calculate(args_list[1], scope);
    if (!Is<Cell>(args_list[1])) {
        throw RuntimeError("Function requires only not empty list");
    }
    return As<Cell>(args_list[1])->GetSecond();
}

std::shared_ptr<Object> MakeList::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    std::vector<std::shared_ptr<Object>> result_vector;
    for (size_t i = 1; i + 1 < args_list.size(); ++i) {
        result_vector.emplace_back(Interpreter::Calculate(args_list[i], scope));
    }
    result_vector.emplace_back(nullptr);
    return VectorToCell(result_vector);
}

std::shared_ptr<Object> GetListElement::Invoke(std::shared_ptr<Cell> args,
                                               std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    args_list[1] = Interpreter::Calculate(args_list[1], scope);
    args_list[2] = Interpreter::Calculate(args_list[2], scope);
    if (!Is<Cell>(args_list[1]) || !Is<Number>(args_list[2])) {
        throw RuntimeError("Function requires only a proper list and a number");
    }
    auto list = CellToVector(As<Cell>(args_list[1]));
    size_t id = As<Number>(args_list[2])->GetValue();
    if (list.back() != nullptr) {
        throw RuntimeError("Function requires only a proper list and a number");
    }
    if (list.size() <= id + 1) {
        throw RuntimeError("Function is trying to access non-existent element");
    }
    return list[id];
}

std::shared_ptr<Object> GetListTail::Invoke(std::shared_ptr<Cell> args,
                                            std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    args_list[1] = Interpreter::Calculate(args_list[1], scope);
    args_list[2] = Interpreter::Calculate(args_list[2], scope);
    if (!Is<Cell>(args_list[1]) || !Is<Number>(args_list[2])) {
        throw RuntimeError("Function requires only a proper list and a number");
    }
    auto list = CellToVector(As<Cell>(args_list[1]));
    size_t id = As<Number>(args_list[2])->GetValue();
    if (list.back() != nullptr) {
        throw RuntimeError("Function requires only a proper list and a number");
    }
    if (list.size() <= id) {
        throw RuntimeError("Function is trying to access non-existent element");
    }
    return VectorToCell(std::vector<std::shared_ptr<Object>>{list.begin() + id, list.end()});
}

std::shared_ptr<Object> IsBoolean::Invoke(std::shared_ptr<Cell> args,
                                          std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    return std::make_shared<Boolean>(Is<Boolean>(Interpreter::Calculate(args_list[1], scope)));
}

std::shared_ptr<Object> LogicalNot::Invoke(std::shared_ptr<Cell> args,
                                           std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    args_list[1] = Interpreter::Calculate(args_list[1], scope);
    return std::make_shared<Boolean>(Is<Boolean>(args_list[1]) &&
                                     !As<Boolean>(args_list[1])->GetValue());
}

std::shared_ptr<Object> LogicalAnd::Invoke(std::shared_ptr<Cell> args,
                                           std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    std::shared_ptr<Object> last_visited = std::make_shared<Boolean>(true);
    for (size_t i = 1; i + 1 < args_list.size(); ++i) {
        last_visited = args_list[i] = Interpreter::Calculate(args_list[i], scope);
        if (Is<Boolean>(args_list[i]) && !As<Boolean>(args_list[i])->GetValue()) {
            return last_visited;
        }
    }
    return last_visited;
}

std::shared_ptr<Object> LogicalOr::Invoke(std::shared_ptr<Cell> args,
                                          std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    std::shared_ptr<Object> last_visited = std::make_shared<Boolean>(false);
    for (size_t i = 1; i + 1 < args_list.size(); ++i) {
        last_visited = args_list[i] = Interpreter::Calculate(args_list[i], scope);
        if (Is<Boolean>(args_list[i]) && As<Boolean>(args_list[i])->GetValue()) {
            return last_visited;
        }
    }
    return last_visited;
}

std::shared_ptr<Object> If::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    if (args_list.size() == 4) {
        args_list[1] = Interpreter::Calculate(args_list[1], scope);
        if (!Is<Boolean>(args_list[1]) || As<Boolean>(args_list[1])->GetValue()) {
            return Interpreter::Calculate(args_list[2], scope);
        }
        return nullptr;
    } else if (args_list.size() == 5) {
        args_list[1] = Interpreter::Calculate(args_list[1], scope);
        if (!Is<Boolean>(args_list[1]) || As<Boolean>(args_list[1])->GetValue()) {
            return Interpreter::Calculate(args_list[2], scope);
        }
        return Interpreter::Calculate(args_list[3], scope);
    }
    throw SyntaxError("if requires exactly two or three arguments");
}

std::shared_ptr<Object> Define::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    if (!Is<Symbol>(args_list[1])) {
        if (!Is<Cell>(args_list[1])) {
            throw SyntaxError("define first argument must be a name or a list of names");
        }
        if (!Is<Symbol>(As<Cell>(args_list[1])->GetFirst())) {
            throw SyntaxError("define first argument must be a name or a list of names");
        }
        auto arguments = As<Cell>(args_list[1])->GetSecond();
        if (arguments && !Is<Cell>(arguments)) {
            throw RuntimeError("Combination must be a proper list");
        }
        auto commands = As<Cell>(args->GetSecond())->GetSecond();
        if (commands == nullptr) {
            throw SyntaxError("lambda-define must contains at least one command");
        }
        scope->Define(As<Symbol>(As<Cell>(args_list[1])->GetFirst())->GetName(),
                      std::make_shared<Lambda>(As<Cell>(arguments), As<Cell>(commands), scope));
        return nullptr;
    }
    if (args_list.size() != 4) {
        throw SyntaxError("define requires exactly 2 arguments");
    }
    scope->Define(As<Symbol>(args_list[1])->GetName(), Interpreter::Calculate(args_list[2], scope));
    return nullptr;
}

std::shared_ptr<Object> Set::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    if (args_list.size() != 4) {
        throw SyntaxError("set! requires exactly 2 arguments");
    }
    if (!Is<Symbol>(args_list[1])) {
        throw SyntaxError("set! first argument should be a name");
    }
    scope->Set(As<Symbol>(args_list[1])->GetName(), Interpreter::Calculate(args_list[2], scope));
    return nullptr;
}

std::shared_ptr<Object> SetFront::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    auto list = Interpreter::Calculate(args_list[1], scope);
    if (!Is<Cell>(list)) {
        throw RuntimeError("set-car! requires lists only");
    }
    As<Cell>(list)->SetFirst(Interpreter::Calculate(args_list[2], scope));
    return nullptr;
}

std::shared_ptr<Object> SetTail::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    auto list = Interpreter::Calculate(args_list[1], scope);
    if (!Is<Cell>(list)) {
        throw RuntimeError("set-cdr! requires lists only");
    }
    As<Cell>(list)->SetSecond(Interpreter::Calculate(args_list[2], scope));
    return nullptr;
}

std::shared_ptr<Object> MakeLambda::Invoke(std::shared_ptr<Cell> args,
                                           std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    if (args_list.size() == 2) {
        throw SyntaxError("lambda requires arguments and commands");
    }
    if (args_list.size() == 3) {
        throw SyntaxError("lambda requires at least one command");
    }
    auto arguments = args_list[1];
    if (arguments && !Is<Cell>(arguments)) {
        throw SyntaxError("lambda requires arguments as list");
    }
    auto commands = As<Cell>(args->GetSecond())->GetSecond();
    return std::make_shared<Lambda>(As<Cell>(arguments), As<Cell>(commands), scope);
}

std::shared_ptr<Object> IsSymbol::Invoke(std::shared_ptr<Cell> args, std::shared_ptr<Scope> scope) {
    CHECKER(args, scope, args_list);
    return std::make_shared<Boolean>(Is<Symbol>(Interpreter::Calculate(args_list[1], scope)));
}