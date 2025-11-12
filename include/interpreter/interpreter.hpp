
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <stdexcept>
#include "interpreter/values.hpp"
#include "interpreter/casting.hpp"
#include "parser/node_types.hpp"
#include "symtable/environment.hpp"

namespace interpreter {

// Forward declarations
rtpoint eval_program(parser::program* exp, symtable::environment* env);
rtpoint eval(parser::statement* ast_node, symtable::environment* env);

// Evaluate numeric binary operations
inline rtpoint eval_num_binary_exp(const number_val* lhs, const number_val* rhs,
                                   const std::string& op) {
  double result = 0.0;

  if (op == "+") {
    result = lhs->value + rhs->value;
  } else if (op == "-") {
    result = lhs->value - rhs->value;
  } else if (op == "*") {
    result = lhs->value * rhs->value;
  } else if (op == "/") {
    if (rhs->value == 0.0) {
      throw std::runtime_error("INTERPRETER: Division by zero error");
    }
    result = lhs->value / rhs->value;
  } else if (op == "%") {
    result = static_cast<double>(static_cast<int>(lhs->value) % static_cast<int>(rhs->value));
  } else {
    throw std::runtime_error("Unknown numeric operator: " + op);
  }

  return make_number(result);
}

// coalesce nil values: return first non-nil, or nil if both are nil
inline rtpoint nil_coalesce(rtpoint lhs, rtpoint rhs) {
  if (!is_nil(lhs.get())) {
    return lhs;
  }
  return rhs;
}

// Evaluate binary expressions
inline rtpoint eval_binary_exp(parser::binary_exp* exp, symtable::environment* env) {
  auto lhs = eval(exp->left.get(), env);
  auto rhs = eval(exp->right.get(), env);

  // Both numbers: perform numeric operation
  if (is_number(lhs.get()) && is_number(rhs.get())) {
    auto* lhs_num = static_cast<number_val*>(lhs.get());
    auto* rhs_num = static_cast<number_val*>(rhs.get());
    return eval_num_binary_exp(lhs_num, rhs_num, exp->op);
  }

  // String concatenation
  if (lhs->type == STR || rhs->type == STR) {
    std::string result;
    
    if (lhs->type == STR) {
      result += static_cast<str_val*>(lhs.get())->value;
    } else if (!is_nil(lhs.get())) {
      if (lhs->type == NUMBER) {
        result += std::to_string(static_cast<number_val*>(lhs.get())->value);
      } else if (lhs->type == BOOL) {
        result += static_cast<bool_val*>(lhs.get())->value ? "true" : "false";
      }
    }

    if (rhs->type == STR) {
      result += static_cast<str_val*>(rhs.get())->value;
    } else if (!is_nil(rhs.get())) {
      if (rhs->type == NUMBER) {
        result += std::to_string(static_cast<number_val*>(rhs.get())->value);
      } else if (rhs->type == BOOL) {
        result += static_cast<bool_val*>(rhs.get())->value ? "true" : "false";
      }
    }

    return make_string(result);
  }

  // Either operand is nil: return first non-nil, or nil
  return nil_coalesce(std::move(lhs), std::move(rhs));
}

// Evaluate identifiers
inline rtpoint eval_identifier(parser::identifier* ident, symtable::environment* env) {
  auto val = env->lookup_variable(ident->symbol);
  if (!val) {
    throw std::runtime_error("Undefined variable: " + ident->symbol);
  }
  // Return a copy of the value
  return val->clone();
}

// Evaluate variable declarations
inline rtpoint eval_var_decl(parser::var_dec* new_var, symtable::environment* env) {
  rtpoint value = new_var->type 
    ? eval(new_var->type.get(), env) 
    : make_nil();

  auto* result = env->declare_variable(new_var->identifier, std::move(value));
  // Return a new unique_ptr wrapping the stored value
  return std::make_unique<runtime_val>(*result);
}



// Main evaluation function
inline rtpoint eval(parser::statement* ast_node, symtable::environment* env) {
  if (!ast_node) {
    return make_nil();
  }

  try {
    switch (ast_node->kind) {
      case parser::NUMERIC_LITERAL: {
        auto num_literal = static_cast<parser::numeric_literal*>(ast_node);
        try {
          double val = std::stod(num_literal->symbol);
          return make_number(val);
        } catch (const std::invalid_argument& e) {
          std::cerr << "ERROR: Invalid number format: '" << num_literal->symbol << "'\n";
          return make_nil();
        }
      }

      case parser::NULL_LITERAL:
        return make_nil();

      case parser::BINARY_EXP:
        return eval_binary_exp(static_cast<parser::binary_exp*>(ast_node), env);

      case parser::PROGRAM:
        return eval_program(static_cast<parser::program*>(ast_node), env);

      case parser::IDENTIFIER:
        return eval_identifier(static_cast<parser::identifier*>(ast_node), env);

      case parser::VAR_DEC:
        return eval_var_decl(static_cast<parser::var_dec*>(ast_node), env);

      default:
        throw std::runtime_error("AST Node kind not implemented for interpretation");
    }
  } catch (const std::exception& e) {
    std::cerr << "INTERPRETER ERROR: " << e.what() << "\n";
    return make_nil();
  }
}

// Evaluate a program (sequence of statements)
inline rtpoint eval_program(parser::program* prog, symtable::environment* env) {
  if (!prog) {
    return make_nil();
  }

  rtpoint last_eval = make_nil();
  for (const auto& statement : prog->body) {
    last_eval = eval(statement.get(), env);
  }
  return last_eval;
}

}

#endif // !INTERPRETER_HPP

