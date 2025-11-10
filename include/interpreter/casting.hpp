#ifndef CASTING_HPP
#define CASTING_HPP

#include <memory>
#include <interpreter/values.hpp>
#include <parser/node_types.hpp>

namespace interpreter {

template<typename T>
T* as_ptr(const std::unique_ptr<runtime_val>& val) {
    return static_cast<T*>(val.get());
}

template<typename T>
T* as_ptr(runtime_val* val) {
    return static_cast<T*>(val);
}

template<typename T>
T* as_ast(parser::statement* node) {
  return static_cast<T*>(node);
}

}
#endif
