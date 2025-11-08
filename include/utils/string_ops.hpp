#ifndef STRING_OPS_HPP
#define STRING_OPS_HPP
#include <iostream>

namespace utils {
inline std::string sub_str(std::string& src, size_t start, size_t end){
  std::string buff; 
  while(start < end){
    buff += src[start];
  }
  return buff;
}
}

#endif
