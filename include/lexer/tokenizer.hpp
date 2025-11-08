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

  inline token_type check_keyword(std::string& word){
    if (word == "if") return token_type::IF;
    if (word == "elif") return token_type::ELIF;
    if (word == "else") return token_type::ELSE;
    if (word == "until") return token_type::UNTIL;
    if (word == "fun") return token_type::FUN;
    if (word == "true") return token_type::TRUE;
    if (word == "meow") return token_type::MEOW;
    if (word == "return") return token_type::RETURN;
    if (word == "and") return token_type::AND;
    if (word == "or") return token_type::OR;
    if (word == "not") return token_type::NOT;
    return token_type::IDENTIFIER;
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

    // make the tokenizer obj
    tokenizer _create_tokenizer(buff.str(), buff.str().length());
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
    switch(c){
    case '(': tok_obj.add_tok(LPAREN); break;
    case ')': tok_obj.add_tok(RPAREN); break;
    case '{': tok_obj.add_tok(RBRACE); break;
    case '}': tok_obj.add_tok(LBRACE); break;
    case ',': tok_obj.add_tok(COMMA); break;
    case '*': tok_obj.add_tok(MULT); break;
    case '/': tok_obj.add_tok(DIV); break;
    case '+': tok_obj.add_tok(PLUS); break;
    case '.': tok_obj.add_tok(MINUS); break;
    case '%': tok_obj.add_tok(MOD); break;
    case '^': tok_obj.add_tok(POW); break;
    case '#': tok_obj.add_tok(LEND); break;
    case '!': tok_obj.add_tok(match(tok_obj, '=')? NOT_EQ : NOT); break;
    case '=': tok_obj.add_tok(match(tok_obj, '=')? EQ_EQ : ASSIGN); break;
    case '<': tok_obj.add_tok(match(tok_obj, '=')? LESS_EQ : LESS); break;
    case '>': tok_obj.add_tok(match(tok_obj, '=')? GREATER_EQ : GREATER); break;
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
        while(!tok_obj.is_end() && (isalnum(tok_obj.peak()) || tok_obj.peak() == '_')) advance(tok_obj);
        int end = tok_obj.pos;
        std::string word = utils::sub_str(tok_obj.source, tok_obj.start, end);
        tok_obj.add_tok({check_keyword(word), word, tok_obj.line});
      } else {
        std::cerr << "Unexpected character on line " << tok_obj.line << ": " << c << "\n";
      }

  }  
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
    double value = std::stod(utils::sub_str(src, start, end));
    tok_obj.add_tok(token(NUMBER, value, line));
}

inline void scan_string(tokenizer& tok_obj){
  int& start = tok_obj.start;
  while(!tok_obj.is_end() && tok_obj.peak() != '"'){
    if(tok_obj.peak() == '\n') tok_obj.line++;
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
      tok_obj.start = tok_obj.pos;
    }

    int& curr_line = tok_obj.line;
    tok_obj.add_tok({END_OF_FILE, "", curr_line});
  }

  inline std::vector<token> tokenize_file(std::string file_name){
    auto tok_obj = new_tokenizer(file_name); 
    scan_tokens(tok_obj);
    // tok_obj.dump_tokens();
  }     

}

#endif
