#ifndef NODE_TYPE_HPP
#define NODE_TYPE_HPP

#include <iostream>
#include <vector>
#include <memory>

namespace parser {

enum node_type {
  PROGRAM, 
  NUMERIC_LITERAL,
  IDENTIFIER,
  BINARY_EXP,
  UNARY_EXP,
  CALL_EXP,
  FUN_DEC,
  VAR_DEC,
};

// could refactor to make abstract... not worth?
struct statement {
  node_type kind;
  statement(node_type k) : kind(k) {}
  virtual ~statement() = default;
};

struct program :public statement {
  std::vector<std::unique_ptr<statement>> body;
  program() : statement(node_type::PROGRAM) {}
  void add(std::unique_ptr<statement> stmt){
    body.push_back(std::move(stmt)); 
  }
};

struct expression :public statement { 
  expression(node_type k) : statement(k) {}
};

struct binary_exp :public expression {
    std::unique_ptr<expression> left;
    std::unique_ptr<expression> right;
    std::string op;

    binary_exp(std::unique_ptr<expression> l,
               std::unique_ptr<expression> r,
               std::string o)
        : expression(node_type::BINARY_EXP),
          left(std::move(l)), right(std::move(r)), op(std::move(o)) {}
};

struct identifier :public expression {
  std::string symbol; // x, foo, etc
  identifier(): expression(node_type::IDENTIFIER) {};
  identifier(std::string sym):
    expression(node_type::IDENTIFIER), symbol(sym) {};
};

// refactor for float / double / unsigned etc
struct numeric_literal :public expression {
  std::string symbol; // x, foo, etc
  numeric_literal(): expression(node_type::NUMERIC_LITERAL) {};
  numeric_literal(std::string sym):
    expression(node_type::IDENTIFIER), symbol(sym) {};
};


}

#endif 
