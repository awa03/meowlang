#include <iostream>  
#include <memory>
#include <lexer/tokenizer.hpp>
#include <parser/parser.hpp>
#include <utils/dump.hpp>
#include "interpreter/interpreter.hpp"
#include "interpreter/values.hpp"
#include "interpreter/casting.hpp"
#include "lexer/token.hpp"
#include "symtable/environment.hpp"

using namespace std;

// Helper function to print runtime values
void print_value(interpreter::runtime_val* val) {
  switch(val->type) {
    case interpreter::NUMBER: {
      auto num = interpreter::as_ptr<interpreter::number_val>(val);
      std::cout << num->value;
      break;
    }
    case interpreter::NIL: {
      std::cout << "nil";
      break;
    }
    case interpreter::BOOL: {
      auto boolean = interpreter::as_ptr<interpreter::bool_val>(val);
      std::cout << (boolean->value ? "true" : "false");
      break;
    }
    default:
      std::cout << "unknown";
  }
}

// Execute and print result
void execute_and_print(parser::program& program, symtable::environment* env) {
  auto ret = interpreter::eval_program(&program, env);
  print_value(ret);
  std::cout << std::endl;
  delete ret; 
}

// REPL mode
void repl(symtable::environment* env) {
  std::cout << "Meow REPL - Enter expressions (Ctrl+C to exit)\n";
  
  while(true) {
    std::cout << "> ";
    std::string input;
    
    if(!std::getline(std::cin, input)) {
      break; // EOF or error
    }
    
    if(input.empty()) {
      continue;
    }
    
    try {
      // tokenize
      auto tokens = lexer::new_tokenizer_runtime(input); 
      lexer::scan_tokens(tokens);
      
      // parse
      parser::parse parsed(tokens.tokens);
      auto program = parsed.make_ast();
      
      // execute and print
      execute_and_print(program, env);
      
    } catch(const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
  }
}

// exec file
void execute_file(const char* filepath, symtable::environment* env) {
  try {
    auto tokens = lexer::tokenize_file(filepath);
    
    parser::parse parsed(tokens);
    auto program = parsed.make_ast();
    
    // utils::dump_program(program);
    
    execute_and_print(program, env);
    
  } catch(const std::exception& e) {
    std::cerr << "Error executing file: " << e.what() << std::endl;
    exit(1);
  }
}

// initialize global environment
symtable::environment* create_global_env() {
  auto env = new symtable::environment(nullptr);
  
  // declare global constants
  env->declare_variable("true", new interpreter::bool_val(true));
  env->declare_variable("false", new interpreter::bool_val(false));
  
  return env;
}

int main(int argc, char** argv) {
  // Create global environment
  auto env = create_global_env();
  
  if(argc == 1) {
    // no arguments - enter REPL mode
    repl(env);
  } 
  else if(argc == 2) {
    // one argument - execute file
    execute_file(argv[1], env);
  }
  else {
    // Too many arguments
    std::cerr << "Usage: " << argv[0] << " [filename]\n";
    std::cerr << "  No arguments: Start REPL\n";
    std::cerr << "  With filename: Execute file\n";
    delete env;
    return 1;
  }
  
  // Cleanup
  delete env;
  return 0;
}
