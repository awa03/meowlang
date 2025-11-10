#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include "interpreter/values.hpp"
#include "interpreter/casting.hpp"
#include "parser/node_types.hpp"
#include "symtable/environment.hpp"

namespace interpreter {


runtime_val* eval_program(parser::program* exp, symtable::environment* env);
runtime_val* eval(parser::statement* ast_node, symtable::environment* env);

inline runtime_val* eval_num_binary_exp(number_val* lhs, number_val* rhs,
                                        std::string op){
  int result = 0;   

  // addition
  if(op == "+"){
    result = lhs->value + rhs->value;
  }
  
  // subtraction
  else if(op == "-"){
    result = lhs->value - rhs->value;
  }

  // multiplication
  else if(op == "*"){
    result = lhs->value * rhs->value;
  }

  // division
  else if(op == "/"){
    if(rhs->value == 0){
      std::cerr << "INTERPRETER: Divide by zero error, exiting\n";
      exit(1);
    }
    result = lhs->value / rhs->value;
  }

  // modulus -- need to int cast for all vals
  else {
    result = (int)lhs->value % (int)rhs->value;
  }

  return new number_val(result);
}

inline runtime_val* eval_binary_exp(parser::binary_exp* exp, symtable::environment* env){

  // get both sides 
  auto lhs = eval(exp->left.get(), env); 
  auto rhs = eval(exp->right.get(), env); 

  // check to see if able to binary eval 
  if(lhs->type == NUMBER && rhs->type == NUMBER){
    return eval_num_binary_exp(
      as_ptr<number_val>(lhs), // cast to type
      as_ptr<number_val>(rhs),
      exp->op  
    );
  }

  // Clean up memory
  delete lhs;
  delete rhs;
  

  // one or both are null
  return new nil_val();
}

inline runtime_val* eval_identifier(parser::identifier* ident, symtable::environment* env){
  auto val = env->lookup_variable(ident->symbol);
  return val;
}

// evaluate abstract syntax tree
inline runtime_val* eval(parser::statement* ast_node, symtable::environment* env){
  switch(ast_node->kind){
    case parser::NUMERIC_LITERAL:{
      auto num_literal = as_ast<parser::numeric_literal>(ast_node);

      // case adding null
      try {
        double val = std::stod(num_literal->symbol);
        return new number_val(val);
      }

      catch(const std::invalid_argument& e){
        std::cerr << "ERROR: Invalid number format: '" << num_literal->symbol << "'\n";
        return new nil_val();
      }
    }

    case parser::NULL_LITERAL:
      return new nil_val();

    // case parser::STRING:
    //   return nullptr;

    case parser::BINARY_EXP:
      return eval_binary_exp(static_cast<parser::binary_exp*>(ast_node), env);

    case parser::PROGRAM:
      return eval_program(static_cast<parser::program*>(ast_node), env);


    case parser::IDENTIFIER:
      return eval_identifier(static_cast<parser::identifier*>(ast_node), env);

    default:
      std::cerr << "This AST Node is not setup for interpretation...\n";
      exit(1);
  } 
}

inline runtime_val* eval_program(parser::program* exp, symtable::environment* env){
  runtime_val* last_eval = new nil_val();
  for(const auto& statement : exp->body){
    last_eval = eval(statement.get(), env); 
  } 
  return last_eval;
}


}

#endif // !INTERPRETER_HPP
