# 🐱 Meow Lang

A purr-fectly adorable programming language for cat lovers and coding enthusiasts!

## 🎀 Overview

Meow Lang is a cute, expressive programming language that brings the joy of cats to your code. With syntax inspired by our feline friends, writing code has never been more fun!

## ✨ Features

- **Adorable Syntax**: Keywords like `meow` make your code purr
- **Simple & Expressive**: Easy to learn, fun to write
- **Cat-Themed**: Because everything is better with cats! 🐾

## 📦 Language Features

### Keywords

- `if` / `elif` / `else` - Conditional logic
- `until` - Loop control
- `fun` - Function declarations
- `meow` - Special cat-themed keyword
- `return` - Return values from functions
- `true` - Boolean true value
- `and` / `or` / `not` - Logical operators

### Operators

- **Arithmetic**: `+`, `-`, `*`, `/`, `%`, `^`
- **Comparison**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical**: `and`, `or`, `not`

### Data Types

- Numbers (integers and floats)
- Strings (enclosed in double quotes)
- Booleans (`true`)

## 🚀 Getting Started

### Prerequisites

- C++ compiler with C++11 support or later
- Make (optional, for build automation)

### Building

```bash
# Compile your Meow Lang interpreter
g++ -g -O2 main.cpp -o meow_lang

# Run a Meow Lang program
./meow_lang your_program.meow
```

## 📝 Example Code

```
# A simple Meow Lang program
meow

fun greet(name) {
    return "Hello, " + name + "!"
}

until condition {
    meow
}
```

## 🏗️ Project Structure

```
.
├── lexer/
│   ├── token.hpp          # Token definitions
│   └── tokenizer.hpp      # Lexical analyzer
├── utils/
│   └── string_ops.hpp     # String utility functions
└── main.cpp               # Entry point
```

## 🐾 Debugging

Use GDB for debugging:

```bash
# Compile with debug symbols
g++ -g -O0 main.cpp -o meow_lang

# Run with GDB
gdb ./meow_lang
```

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Add more cat-themed keywords! 🐱

## 🙏 Acknowledgments

Thanks to all the cats who inspired this language by walking across keyboards and creating interesting syntax errors!

---

## ⚠️ Disclaimer

This README was generated with AI assistance. Please verify all technical details and update as needed for your specific implementation.
