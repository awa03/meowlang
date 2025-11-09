#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include "lexer/token.hpp"
#include "parser/node_types.hpp"

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

private:
  lexer::token_list tokens;

  bool eof() {
    return tokens[0].type == lexer::END_OF_FILE;
  }

  std::unique_ptr<statement> parse_stmt(){
    return parse_exp(); 
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
      || curr_tok().type == lexer::POW
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

  std::unique_ptr<expression> parse_prim_exp(){
    const auto tok = curr_tok();
    switch(tok.type){
      case lexer::IDENTIFIER:
        return std::make_unique<identifier>(advance());
      case lexer::NUMBER:
        return std::make_unique<numeric_literal>(advance());
      case lexer::PLUS:
        return std::make_unique<numeric_literal>(advance());
      default: 
        std::cerr << "\nUNEXPECTED TOKEN FOUND DURING PARSING...\n";
        std::cerr << lexer::token_type_to_string(tok.type) << "[line:";
        std::cerr << tok.line << "]\n";
        exit(1);
    } 
  }
  
};
}

#endif
