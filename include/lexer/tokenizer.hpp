#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdlib.h>

#include "lexer/token.hpp"
#include "utils/string_ops.hpp"

namespace lexer {

  void scan_string(tokenizer& tok_obj); // forward decleration bc why not
  void scan_number(tokenizer& tok_obj);
  void scan_identifier(tokenizer& tok_obj);
  void scan_comment(tokenizer& tok_obj);

  inline tokenizer new_tokenizer_runtime(std::string dump){
    tokenizer _create_tokenizer(dump, dump.length());
    return _create_tokenizer;
  } 

  inline tokenizer new_tokenizer(std::string file_name){
    // check file
    std::ifstream file(file_name); 
    if(!file.is_open()){
      std::cerr << "Could not open <" << file_name << ">\n";
      exit(1);
    }

    // read contents into buff
    std::stringstream buff;
    buff << file.rdbuf();

    std::string dump = buff.str();
    // make the tokenizer obj
    tokenizer _create_tokenizer(dump, dump.length());
    return _create_tokenizer; 
  }

  inline char advance(tokenizer& tok_obj){
    return tok_obj.source[tok_obj.pos++];
  }

  inline bool match(tokenizer& tok_obj, char expected){
    if(tok_obj.is_end()) return false;
    int& curr = tok_obj.pos;

    // not expected
    if(tok_obj.source[curr] != expected) return false;

    // advance 
    curr++;
    return true; 
  }

  inline void scan_token(tokenizer& tok_obj){
    char c = advance(tok_obj);
    int& line_num = tok_obj.line;
    switch(c){
    case '(': tok_obj.add_tok({LPAREN, "(", line_num}); break;
    case ')': tok_obj.add_tok({RPAREN, ")", line_num}); break;
    case '{': tok_obj.add_tok({LBRACE, "{", line_num}); break;
    case '}': tok_obj.add_tok({RBRACE, "}", line_num}); break;
    case ',': tok_obj.add_tok({COMMA, ",", line_num}); break;
    case '*': tok_obj.add_tok({MULT, "*", line_num}); break;
    case '/': tok_obj.add_tok({DIV, "/", line_num}); break;
    case '+': tok_obj.add_tok({PLUS, "+", line_num}); break;
    case '-': tok_obj.add_tok({MINUS, "-", line_num}); break;
    case '.': tok_obj.add_tok({DOT, ".", line_num}); break;
    case '%': tok_obj.add_tok({MOD, "%", line_num}); break;
    case '^': tok_obj.add_tok({POW, "^", line_num}); break;
    case ';': tok_obj.add_tok({LEND, ";", line_num}); break;
    case '$': scan_comment(tok_obj); break;
    case '!': tok_obj.add_tok({match(tok_obj, '=')? NOT_EQ : NOT, "", line_num}); break;
    case '=': tok_obj.add_tok({match(tok_obj, '=')? EQ_EQ : ASSIGN, "", line_num}); break;
    case '<': tok_obj.add_tok({match(tok_obj, '=')? LESS_EQ : LESS, "", line_num}); break;
    case '>': tok_obj.add_tok({match(tok_obj, '=')? GREATER_EQ : GREATER, "", line_num}); break;
    case '\n':tok_obj.line++; break;
    case '\b':break; // ignore 
    case '\r':break; // ignore
    case '\t':break; // ignore  --- Enable onelining
    case ' ' :break; // ignore 
    case '"' :scan_string(tok_obj); break; // string case -- check until next "
    default:
      if(isdigit(c)){
        scan_number(tok_obj);
      } else if(isalpha(c)) {
        scan_identifier(tok_obj);
      } else {
        std::cerr << "Unexpected character on line " << tok_obj.line << ": " << c << "\n";
      }
  }  
  }

  inline void scan_comment(tokenizer& tok_obj){
    std::cout << "\n\nRUNNING COMMENT SCAN" << "\n\n";
    while(tok_obj.peak() != '\n' && tok_obj.peak() != '$'){
      std::cout << "peaking at: "<< tok_obj.peak() << "\n";
      advance(tok_obj); 
    } 

    // needed if using inline commenting
    if(tok_obj.peak() == '\n'){
      return;
    }

    
    advance(tok_obj); 
  }
  inline void scan_identifier(tokenizer& tok_obj){
    while(isalnum(tok_obj.peak())) {
      advance(tok_obj);
    }

    int& start = tok_obj.start;
    int& end = tok_obj.pos;
    std::string& src = tok_obj.source;

    // grab the found keyword
    std::string extracted_keyword = utils::sub_str(src, start, end);

    // check if valid keyword
    auto tok_type = keywords.find(extracted_keyword);
    if(tok_type != keywords.end()){
      tok_obj.add_tok({tok_type->second, extracted_keyword, tok_obj.line});
    }
    // if not found in map then call IDENTIFIER
    else tok_obj.add_tok({IDENTIFIER, extracted_keyword, tok_obj.line});
  }

  inline void scan_number(tokenizer& tok_obj){
    int& start= tok_obj.start;

    // scan entire whole value
    while(!tok_obj.is_end() && isdigit(tok_obj.peak())) advance(tok_obj);
    // handle decimal numbers
    if(tok_obj.peak() == '.' && isdigit(tok_obj.peak_next())) {
      advance(tok_obj);

      // handle fractional portion
      while(!tok_obj.is_end() && isdigit(tok_obj.peak())) advance(tok_obj);
    }

    std::string& src = tok_obj.source; 
    int end= tok_obj.pos;
    int& line= tok_obj.line;
    std::string value = utils::sub_str(src, start, end);
    tok_obj.add_tok(token(NUMBER, value, line));
}

inline void scan_string(tokenizer& tok_obj){
  int& start = tok_obj.start;
  while(!tok_obj.is_end() && tok_obj.peak() != '"'){
    if(tok_obj.peak() == '\n') tok_obj.line++; // may needrefactor
    advance(tok_obj);
  }

  if(tok_obj.is_end()){
    std::cerr << "Unterminated string on line " << tok_obj.line << "\n";
    return;
  }

  advance(tok_obj); // consume closing quote
  int end = tok_obj.pos;
  std::string str_val = utils::sub_str(tok_obj.source, start, end);
  tok_obj.add_tok({STRING, str_val, tok_obj.line});
}

  
  inline void scan_tokens(tokenizer& tok_obj){
    while(!tok_obj.is_end()){
      tok_obj.start = tok_obj.pos;
      scan_token(tok_obj);
    }

    int& curr_line = tok_obj.line;
    tok_obj.add_tok({END_OF_FILE, "", curr_line});
  }

  inline std::vector<token> tokenize_file(std::string file_name){
    auto tok_obj = new_tokenizer(file_name); 
    scan_tokens(tok_obj);
    // tok_obj.dump_tokens();
    return tok_obj.tokens;
  }     

}

#endif
