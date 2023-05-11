#pragma once

#include <memory>
#include <unordered_map>
#include <map>

#include "object.h"
#include "scheme.h"

class DefaultListChecker {
public:
    void CheckList(std::vector<std::shared_ptr<Object>>& args_list);
};

class UnaryFunctionChecker {
public:
    void CheckList(std::vector<std::shared_ptr<Object>>& args_list);
};

class BinaryFunctionChecker {
public:
    void CheckList(std::vector<std::shared_ptr<Object>>& args_list);
};

class NonEmptyListChecker {
public:
    void CheckList(std::vector<std::shared_ptr<Object>>& args_list);
};

class DefaultTypeChecker {
public:
    void CheckTypes(std::vector<std::shared_ptr<Object>>& args_list, std::shared_ptr<Scope> scope);
};

class IntegerTypeChecker {
public:
    void CheckTypes(std::vector<std::shared_ptr<Object>>& args_list, std::shared_ptr<Scope> scope);
};

class Quote : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class IsNumber : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Equal : public Function, DefaultListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Greater : public Function, DefaultListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Less : public Function, DefaultListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class NotGreater : public Function, DefaultListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class NotLess : public Function, DefaultListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Sum : public Function, DefaultListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Subtraction : public Function, NonEmptyListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Product : public Function, DefaultListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Division : public Function, NonEmptyListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Maximum : public Function, NonEmptyListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Minimum : public Function, NonEmptyListChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Absolute : public Function, UnaryFunctionChecker, IntegerTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class IsPair : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class IsNull : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class IsList : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class MakePair : public Function, BinaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Front : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class AfterFront : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class MakeList : public Function, DefaultListChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class GetListElement : public Function, BinaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class GetListTail : public Function, BinaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class IsBoolean : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class LogicalNot : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class LogicalAnd : public Function, DefaultListChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class LogicalOr : public Function, DefaultListChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class If : public Function, DefaultListChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Define : public Function, DefaultListChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class Set : public Function, DefaultListChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class SetFront : public Function, BinaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class SetTail : public Function, BinaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class MakeLambda : public Function, DefaultListChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};

class IsSymbol : public Function, UnaryFunctionChecker, DefaultTypeChecker {
public:
    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;
};