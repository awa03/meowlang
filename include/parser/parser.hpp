#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <string>
#include "lexer/token.hpp"
#include "parser/node_types.hpp"
#include "utils/dump.hpp"

namespace parser {
class parse {


public:
  parse(lexer::token_list toks) : tokens(toks){}

  // generate abstract syntax tree
  program make_ast(){
    program prgrm;

    while(!eof()){
      prgrm.body.push_back(parse_stmt());
    }

    return prgrm;
  }

  lexer::token curr_tok(){
    return tokens[0];
  }

  std::string advance(){
    const auto prev_val = curr_tok().value;
    tokens.erase(tokens.begin());
    return prev_val;
  }

  lexer::token_type advance_type(){
    const auto prev_val = curr_tok().type;
    tokens.erase(tokens.begin());
    return prev_val;
  }

private:
  lexer::token_list tokens;

  bool eof() {
    return tokens[0].type == lexer::END_OF_FILE;
  }

std::unique_ptr<statement> parse_var_dec(){
  bool is_mut = advance_type() == lexer::token_type::MUT; // check mut first

  // ensure that assigning if mut
  if(is_mut && curr_tok().type == lexer::token_type::VAR){
    advance(); // eat VAR 
  }

  const auto iden = expect(lexer::token_type::IDENTIFIER, "Expected Identifier Following `mut`/`var`");
  
  // check if semicolon (uninitialized declaration)
  if(curr_tok().type == lexer::token_type::LEND){
    advance(); // eat semicolon
    
    if(!is_mut){
      throw std::runtime_error("Must assign value to constant expression [" 
      + std::to_string(curr_tok().line) +"]\n"); 
      exit(1);
    }

    return std::make_unique<var_dec>(is_mut, iden.value, nullptr);
  } 

  // Otherwise expect assignment
  expect(lexer::token_type::ASSIGN, "Expected assignment following identifier in variable decleration");
  auto decl = std::make_unique<var_dec>(
    is_mut,
    iden.value,
    parse_exp()
  );
  
  expect(lexer::token_type::LEND, "Expected `;` following variable decleration");
  return decl;
}
  std::unique_ptr<statement> parse_stmt(){
    switch(curr_tok().type) {
    case lexer::VAR:
      return parse_var_dec();
      
    case lexer::MUT:
      return parse_var_dec();

    default:
      return parse_exp(); 
    }
  }

  std::unique_ptr<expression> parse_exp(){
    return parse_additive_exp(); 
  }


  std::unique_ptr<expression> parse_mult_exp(){
    auto left = parse_prim_exp(); 

    while (
         curr_tok().type == lexer::MULT 
      || curr_tok().type == lexer::DIV 
      || curr_tok().type == lexer::MOD
    ){
      auto oper = advance();
      auto right = parse_prim_exp();
      left = std::make_unique<binary_exp>(
        std::move(left), 
        std::move(right), 
        oper
      );
    }

    return left;
  }

  /*
   * for 5+5 for example
   * first get left: 5
   * then advance to operator
   * then get the next operator
   * assign to left for recursive
   * */
  std::unique_ptr<expression> parse_additive_exp(){
    auto left = parse_mult_exp(); 

    while (curr_tok().type == lexer::PLUS || curr_tok().type == lexer::MINUS){
      auto oper = advance();
      auto right = parse_mult_exp();
      left = std::make_unique<binary_exp>(
        std::move(left), 
        std::move(right), 
        oper
      );
    }

    return left;
  }

  lexer::token expect(lexer::token_type type, std::string err){
    const auto prev = curr_tok();

    if(prev.type != type){
      throw std::runtime_error("PARSER: " + err + "\n" +
      "\tExpecting: "+ lexer::token_type_to_string(type) + "\n" + 
      "\tRecieved: " + lexer::token_type_to_string(prev.type) + "\n");
      exit(1);
    }

    tokens.erase(tokens.begin());
    return prev;
  }

  // identify type
  std::unique_ptr<expression> parse_prim_exp(){
    const auto tok = curr_tok();
    switch(tok.type){

      case lexer::IDENTIFIER:
        return std::make_unique<identifier>(advance());

      case lexer::NUMBER:
        return std::make_unique<numeric_literal>(advance());

      case lexer::LPAREN: {
        advance();
        auto val = parse_exp();

        expect(
          lexer::RPAREN,
          "Unexpected token found within parenthesised expression."
        );

        return val;
      }

      // unexpected -- should never reach
      case lexer::RPAREN: {
        throw std::runtime_error("\nPARSER: Unexpected closing parenthesis ')' on line " + std::to_string(tok.line) + "\n");
        exit(1);
      }

      // null value
      case lexer::NIL: {
        advance(); // advance past null
        return std::make_unique<nil_literal>("nil"); // add 
      }
      
      case lexer::LEND: {
      }

      default: 
        throw std::runtime_error("\nPARSER: Unexpected token [" + tok.value + "] on line " + std::to_string(tok.line) + "\n");
        exit(1);
    } 
  }
  
};
}

#endif
