
#ifndef VALUES_HPP
#define VALUES_HPP

#include <string>
#include <memory>

namespace interpreter {

enum value_type {
  NIL, 
  NUMBER,
  BOOL,
  STR,
};

struct runtime_val {
  value_type type;

  virtual std::unique_ptr<runtime_val> clone() const = 0;

  explicit runtime_val(value_type t): type(t) {}
  virtual ~runtime_val() = default;
};

struct nil_val: public runtime_val {
  nil_val(): runtime_val(NIL) {}

  std::unique_ptr<runtime_val> clone() const override {
    return std::make_unique<nil_val>(*this);
  }
};

struct number_val: public runtime_val {
  double value;
  explicit number_val(double v = 0.0): runtime_val(NUMBER), value(v) {}

  std::unique_ptr<runtime_val> clone() const override {
    return std::make_unique<number_val>(*this);
  }
};

struct bool_val: public runtime_val {
  bool value;
  explicit bool_val(bool v = true): runtime_val(BOOL), value(v) {}

  std::unique_ptr<runtime_val> clone() const override {
    return std::make_unique<bool_val>(*this);
  }
};

struct str_val: public runtime_val {
  std::string value;
  explicit str_val(std::string v = ""): runtime_val(STR), value(v) {}

  std::unique_ptr<runtime_val> clone() const override {
    return std::make_unique<str_val>(*this);
  }
};

using rtpoint = std::unique_ptr<runtime_val>;

// Factory functions
inline rtpoint make_nil() {
  return std::make_unique<nil_val>();
}

inline rtpoint make_number(double val) {
  return std::make_unique<number_val>(val);
}

inline rtpoint make_bool(bool val) {
  return std::make_unique<bool_val>(val);
}

inline rtpoint make_string(std::string val) {
  return std::make_unique<str_val>(std::move(val));
}

inline bool is_nil(const runtime_val* val) {
  return val && val->type == NIL;
}

inline bool is_number(const runtime_val* val) {
  return val && val->type == NUMBER;
}

inline bool is_truthy(const runtime_val* val) {
  if (!val) return false;
  if (val->type == NIL) return false;
  if (val->type == BOOL) return static_cast<const bool_val*>(val)->value;
  return true;
}

}

#endif


