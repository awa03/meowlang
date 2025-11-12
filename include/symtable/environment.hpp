#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <iostream>
#include "interpreter/values.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace symtable {

using var_table = std::unordered_map<std::string, std::unique_ptr<interpreter::runtime_val>>;

class environment {
public:
  explicit environment(environment* p = nullptr) : parent(p) {}
  
  ~environment() = default;  // unique_ptr handles cleanup automatically

  interpreter::runtime_val* declare_variable(const std::string& name, interpreter::runtime_val* val) {
    // redeclaring in scope just change value
    variables[name] = std::unique_ptr<interpreter::runtime_val>(val);
    return variables[name].get();
  }

  interpreter::runtime_val* declare_variable(const std::string& name, interpreter::rtpoint val) {
    variables[name] = std::move(val);
    return variables[name].get();
  }

  // assign variable -- error warning on redeclare 
  interpreter::runtime_val* assign_variable(const std::string& name, interpreter::runtime_val* val) {
    const auto env = resolve(name);
    if (!env) {
      throw std::runtime_error("Cannot assign to undefined variable: " + name);
    }
    env->variables[name] = std::unique_ptr<interpreter::runtime_val>(val);
    return env->variables[name].get();
  }

  interpreter::runtime_val* assign_variable(const std::string& name, interpreter::rtpoint val) {
    const auto env = resolve(name);
    if (!env) {
      throw std::runtime_error("Cannot assign to undefined variable: " + name);
    }
    env->variables[name] = std::move(val);
    return env->variables[name].get();
  }

  // resolve env then return if exists - nullptr if not
  interpreter::runtime_val* lookup_variable(const std::string& name) {
    const auto env = resolve(name);
    if (!env) {
      return nullptr;
    }
    return env->variables[name].get();
  }

  // variable scope resolution
  environment* resolve(const std::string& name) {
    // if variable found in curr scope
    if (variables.find(name) != variables.end()) {
      return this;
    }
    // check parent scope
    else {
      if (parent == nullptr) {
        return nullptr;
      }
      return parent->resolve(name);
    }
  }

private:
  environment* parent;
  var_table variables;
};

}

#endif
