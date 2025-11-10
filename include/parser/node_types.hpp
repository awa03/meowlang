#ifndef NODE_TYPE_HPP
#define NODE_TYPE_HPP

#include <string>
#include <vector>
#include <memory>

namespace parser {

enum node_type {
  // statements
  PROGRAM,          // Root Node
  VAR_DEC,          // x = ...
  
  // Expressions 
  NUMERIC_LITERAL,  // Numeric (5, 21.21)
  NULL_LITERAL,     // Nil 
  IDENTIFIER,       // variable
  BINARY_EXP,       // 5 + 5

  // TODO
  UNARY_EXP,
  CALL_EXP,
  FUN_DEC,
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

struct var_dec :public statement {
  bool mut;
  std::string identifier;
  expression* type;


  var_dec(): statement(node_type::VAR_DEC){} // NODE Type

  // idk how to make this formatted in a less ugly way sorry lmao
  var_dec
  (
    bool _mut,
    std::string id,
    expression* typ
  ):
    statement(node_type::VAR_DEC), // NODE Type
    mut(_mut),                     // mutable 
    identifier(id),                // Identifier name
    type(typ){}                    // expr type
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
    expression(node_type::NUMERIC_LITERAL), symbol(sym) {};
};

struct nil_literal :public expression {
  std::string symbol; // x, foo, etc
  nil_literal(): expression(node_type::NULL_LITERAL) {};
  nil_literal(std::string sym):
    expression(node_type::NULL_LITERAL), symbol(sym) {};
};

}

#endif 
