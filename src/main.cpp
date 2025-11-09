#include <iostream>  
#include <lexer/tokenizer.hpp>
#include <parser/parser.hpp>
#include <utils/dump.hpp>

using namespace std;

int main(int argc, char** argv){
  if(argc != 2){
    std::cout << "Meow, No Input File...\n";
  } 
   
  auto tokens = lexer::tokenize_file(argv[1]); 
  parser::parse parsed(tokens);
  auto program = parsed.make_ast();
  utils::dump_program(program);
}
