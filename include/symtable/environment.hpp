#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <iostream>
#include "interpreter/values.hpp"
#include <unordered_map>
#include <string>

using var_table = std::unordered_map<std::string, interpreter::runtime_val*>;

namespace symtable {
class environment {
public:
  environment(environment* p) : parent(p){}
  environment() : parent(nullptr){}
  ~environment() {
    // Clean up all variables
    for (auto& pair : variables) {
      delete pair.second;
    }
  }

  interpreter::runtime_val* declare_variable(std::string name, interpreter::runtime_val* val){
    // redeclaring in scope just change value
    variables[name] = val;
    return val;
  } 

  // assign variable -- error warning on redeclare 
  interpreter::runtime_val* assign_variable(std::string name, interpreter::runtime_val* val){
    const auto env = resolve(name);
    env->variables[name] = val;
    return val;
  } 

  // resolve env then return if exists - nullptr if not
  interpreter::runtime_val* lookup_variable(std::string name){
    const auto env = resolve(name);
    return env->variables[name];
  }

  // variable scope resolution
  environment* resolve(std::string name){
    // if variable found in curr scope
    if(variables.find(name) != variables.end()){
      return this;
    }
    // check parent scope
    else {
      // TODO: SILLY SKUNK MODE (SSM)
      //
      if(parent == nullptr){
        std::cerr << "Cannot resolve [" << name << "] as it does not exist.\n";
        exit(1);
      }

      return this->parent->resolve(name);
    }
  }

private:
  environment* parent;
  var_table variables; 
};
};

#endif // !ENVIRONMENT_HPP
