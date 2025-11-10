#ifndef VALUES_HPP
#define VALUES_HPP

#include <string>

namespace interpreter {

enum value_type {
  NIL, 
  NUMBER,
};

struct runtime_val {
  value_type type;
  runtime_val(value_type t): type(t) {}
   virtual ~runtime_val() = default;
};

struct nil_val: public runtime_val {
  std::string value;
  nil_val(): runtime_val(NIL), value("nil") {}
};


struct number_val: public runtime_val {
  double value;
  number_val(double v= 0.0): runtime_val(NUMBER), value(v) {}
};

}

#endif
