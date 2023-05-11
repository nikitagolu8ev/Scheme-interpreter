#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>

#include "object.h"

class Object;

class Scope {
public:
    Scope(std::initializer_list<std::pair<std::string, std::shared_ptr<Object>>> list);
    explicit Scope(std::shared_ptr<Scope> scope);

    std::shared_ptr<Object> Get(const std::string& name);

    void Define(const std::string& name, std::shared_ptr<Object> obj);
    void Set(const std::string& name, std::shared_ptr<Object> obj);

private:
    std::unordered_map<std::string, std::shared_ptr<Object>> defined_objects_;
    std::shared_ptr<Scope> parent_ = nullptr;
};

std::shared_ptr<Scope> GetGlobalScope();

class Interpreter {
public:
    std::string Run(const std::string&);

    static std::shared_ptr<Object> Calculate(std::shared_ptr<Object> obj,
                                             std::shared_ptr<Scope> scope);
    static std::string ToString(std::shared_ptr<Object> obj);

private:
    std::shared_ptr<Scope> scope_ = std::make_shared<Scope>(GetGlobalScope());
};
