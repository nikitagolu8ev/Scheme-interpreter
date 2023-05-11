#pragma once

#include <memory>
#include <string>
#include <vector>

#include "scheme.h"
#include "tokenizer.h"

class Scope;

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    Number(int64_t value);
    int64_t GetValue() const;

private:
    int64_t value_;
};

class Boolean : public Object {
public:
    Boolean(bool b);
    bool GetValue();

private:
    bool value_;
};

class Symbol : public Object {
public:
    Symbol(const std::string& s);
    const std::string& GetName() const;

private:
    std::string name_;
};

class Cell : public Object {
public:
    Cell() = default;
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second);

    void SetFirst(std::shared_ptr<Object> shd_ptr);
    void SetSecond(std::shared_ptr<Object> shd_ptr);

    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;

private:
    std::shared_ptr<Object> first_ = nullptr;
    std::shared_ptr<Object> second_ = nullptr;
};

std::vector<std::shared_ptr<Object>> CellToVector(std::shared_ptr<Cell> obj);
std::shared_ptr<Cell> VectorToCell(std::vector<std::shared_ptr<Object>> vec);

class Function : public Object {
public:
    virtual ~Function() = default;
    virtual std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                           std::shared_ptr<Scope> scope) = 0;
};

class Lambda : public Function {
public:
    Lambda(std::shared_ptr<Cell> args, std::shared_ptr<Cell> commands,
           std::shared_ptr<Scope> scope);

    std::shared_ptr<Object> Invoke(std::shared_ptr<Cell> args,
                                   std::shared_ptr<Scope> scope) override;

private:
    std::shared_ptr<Scope> parent_;
    std::vector<std::string> variables_;
    std::vector<std::shared_ptr<Object>> command_list_;
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj) != nullptr;
}