#include <iostream>  
#include <lexer/tokenizer.hpp>
using namespace std;

int main(int argc, char** argv){
  if(argc != 2){
    std::cout << "Meow, No Input File...\n";
  } 
   
  lexer::tokenize_file(argv[1]); 
}
