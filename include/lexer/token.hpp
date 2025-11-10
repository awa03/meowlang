#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

namespace lexer {
struct token; // forward dec

using token_value = std::string;
using token_list = std::vector<token>;



enum token_type{


  // Literals
  IDENTIFIER, // variables, constants, etc ()
  STRING,     // "string"
  NUMBER,     // number literal will support int, float, double (5)
  NIL,        // null value 


  // Single Char Tokens
  PLUS,       // + operator
  MINUS,      // - operator
  MULT,       // * operator
  DIV,        // / operator
  MOD,        // % operator
  POW,        // ^ operator
  LBRACE,     // left brace { 
  RBRACE,     // right brace }
  LPAREN,     // left parenthesis 
  RPAREN,     // right parenthesis
  COMMA,      // , for seperation
  DOT,        // for interfaces
  LEND,       // line terminator


  // one or two char tokens
  GREATER,    // > operator
  LESS,       // < operator
  GREATER_EQ, // >= operator
  LESS_EQ,    // <= operator
  ASSIGN,     // = operator
  NOT_EQ,     // != operator 
  EQ_EQ,      // == operator


  // keywords  ------------------------------
  IF,         // if conditional
  OR,         // || operator
  AND,        // && operator
  NOT,        // ! operator (negation)
  ELSE,       // else other block

  WHILE,      // while loop -- may do no for loop 
  FUN,        // function 
  TRUE,       // true value
  MEOW,       // print 
  RETURN,     // return value
  END_OF_FILE,// EOF
  VAR,
  DEL, 
  BREAK, 
  CONTINUE,
  CONST,
  IMPORT,

  COMMENT,


  UNKOWN      // Unkown
};

static const std::unordered_map<std::string, token_type> keywords = {
  // Keywords
  {"new", VAR},
  {"del", DEL},
  {"is", ASSIGN},
  {"nil", NIL},
  {"check", IF},
  {"or", OR},
  {"and", AND},
  {"nah", NOT},
  {"not", NOT},
  {"otherwise", ELSE},
  {"loop", WHILE},
  {"fun", FUN},
  {"true", TRUE},
  {"meow", MEOW},
  {"result", RETURN},
  {"nap", BREAK},
  {"pursue", CONTINUE},
  {"fetch", IMPORT},

  {"stay", CONST}
};


inline std::string token_type_to_string(token_type type) {
  switch(type) {
    // Literals
    case IDENTIFIER: return "IDENTIFIER";
    case STRING:     return "STRING";
    case NUMBER:     return "NUMBER";
    case NIL:        return "NIL";

    // Single Char Tokens
    case PLUS:       return "PLUS";
    case MINUS:      return "MINUS";
    case MULT:       return "MULT";
    case DIV:        return "DIV";
    case MOD:        return "MOD";
    case POW:        return "POW";
    case LBRACE:     return "LBRACE";
    case RBRACE:     return "RBRACE";
    case LPAREN:     return "LPAREN";
    case RPAREN:     return "RPAREN";
    case COMMA:      return "COMMA";
    case DOT:        return "DOT";
    case LEND:       return "LEND";

    // One or two char tokens
    case GREATER:    return "GREATER";
    case LESS:       return "LESS";
    case GREATER_EQ: return "GREATER_EQ";
    case LESS_EQ:    return "LESS_EQ";
    case ASSIGN:     return "ASSIGN";
    case NOT_EQ:     return "NOT_EQ";
    case EQ_EQ:      return "EQ_EQ";

    // Keywords
    case IF:         return "IF";
    case OR:         return "OR";
    case AND:        return "AND";
    case NOT:        return "NAH";
    case ELSE:       return "ELSE";
    case WHILE:      return "WHILE";
    case FUN:        return "FUN";
    case TRUE:       return "TRUE";
    case MEOW:       return "MEOW";
    case RETURN:     return "RETURN";
    case END_OF_FILE:return "END_OF_FILE";
    case VAR        :return "VARIABLE_DEC";
    case DEL        :return "VARIABLE_DEL";

    default:         return "NOT IMP";
  }
}

struct token {
  token_type type; 
  token_value value;

  int line = 0; // for error msging
  // int column -- TODO / WONTFIX
  token(token_type _type, token_value _value) : 
    type(_type), value(_value){}


  token(token_type _type, token_value _value, int _line) : 
    type(_type), value(_value), line(_line) {}
};

struct tokenizer {
  std::string source; // open file
  int pos = 0;        // pos in string
  int start = 0;      // start pos 
  int line = 1;       // curr line num -- itr with \n ?
  int max;            // max (EOF), prevent overflow / outbounds

  token_list tokens;

  tokenizer(std::string src, int _max) :source(src), max(_max) {};  // want max to init 
  tokenizer(int _max) : max(_max) {};  // want max to init 

  // check if within bounds
  bool is_end(){
    return (pos >= max);
  }

  // show next char -- null if invalid
  char peak(){
    if (is_end()) return '\0';
    return source[pos];
  }

  char peak_next(){
    if (pos + 1 >= max) return '\0';
    return source[pos+1];
  }

  // show prev char -- null if invalid
  char peak_back(){
    if (pos - 1 < 0) return '\0';
    return source[pos-1];
  }

  void add_tok(token_type type){
    tokens.push_back(token(type, ""));
  }

  void add_tok(token tok){
    tokens.push_back(tok);
  }

  void dump_tokens(){
    for(auto& tok : tokens){
      std::cout << "TYPE: " << token_type_to_string(tok.type) << "\n";
      std::cout << "LINE: " << tok.line << "\n";
      std::cout << "VALUE: " << tok.value << "\n";
    }
  }

};

}

#endif
