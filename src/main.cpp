#include <iostream>  
#include <lexer/tokenizer.hpp>
#include <parser/parser.hpp>
#include <utils/dump.hpp>
#include "interpreter/interpreter.hpp"
#include "interpreter/values.hpp"
#include "interpreter/casting.hpp"
#include "lexer/token.hpp"
#include "symtable/environment.hpp"

using namespace std;

int main(int argc, char** argv){
  if(argc != 2){
    std::cout << "Meow, No Input File...\n";
  } 

  auto env = new symtable::environment();
   
  auto tokens = lexer::tokenize_file(argv[1]); 
  parser::parse parsed(tokens);
  auto program = parsed.make_ast();
  // utils::dump_program(program);

  
  auto ret = interpreter::eval_program(&program, env);

  if(ret->type == interpreter::NUMBER){
    std::cout << "NUMBER: ";
    auto num = interpreter::as_ptr<interpreter::number_val>(ret);
    std::cout << num->value;
    
  }

  if(ret->type == interpreter::NIL){
    std::cout << "NIL";
  }

}
