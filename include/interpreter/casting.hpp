#ifndef CASTING_HPP
#define CASTING_HPP

#include <memory>
#include <interpreter/values.hpp>
#include <parser/node_types.hpp>

namespace interpreter {

template<typename T>
std::unique_ptr<T> rt_as_type_ptr(std::unique_ptr<runtime_val> val) {
  return std::unique_ptr<T>(
    static_cast<T*>(val.release())
  );
}

template<typename T>
T* as_ptr(runtime_val* val) {
    return static_cast<T*>(val);
}

template<typename T>
T* as_ast(parser::statement* node) {
  return static_cast<T*>(node);
}

template<typename Derived, typename Base>
std::unique_ptr<Derived> get_derived_type(std::unique_ptr<Base>&& base){
  if(auto d = dynamic_cast<Derived*>(base.get())){
    base.release();
    return std::unique_ptr<Derived>(d);
  }
  return nullptr;
}


}
#endif
