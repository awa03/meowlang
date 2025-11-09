# Mathmatical Parsing

For prerequisite knowledge of the lexer keywords please refer to [lexer documentation](../lexer/keywords.md)

## Multiple Expressions

Consider the example `5 + 10 * 2`. In order to parse and resolve the equation we must seperate the relevant operations.
We must also consider order of precedance within operations (*pemdas*). Since multiplication (`*`) has a higher precedance than addition (`+`),
we must solve for `10 * 2` before adding 5. 

In order to accomplish this we will parse operations in *reverse order*. 

```cpp
std::unique_ptr<expression> parse_exp(){
    return parse_additive_exp(); 
}
```

## Additive Parsing
 

```cpp
std::unique_ptr<expression> parse_additive_exp(){
    // returns primary exp type if not mult exp 
    // in the same way as the function below
    auto left = parse_mult_exp();  

    // attempt to gather additive 
    while (curr_tok().type == lexer::PLUS || curr_tok().type == lexer::MINUS){
      auto oper = advance();
      auto right = parse_mult_exp();
      left = std::make_unique<binary_exp>(
        std::move(left), 
        std::move(right), 
        oper
      );
    }

    // returns exp type if not mult or additive
    return left;
}
```


## Multiplicative Parsing

Building Multiplicative expression nodes is very similar to additive, and if we wanted to add 
more expression types with higher precedance (such as `^` or `√`) we could simply add another 
expression check down the chain of calls (*TODO*).

```cpp
std::unique_ptr<expression> parse_mult_exp(){
    auto left = parse_prim_exp();  // attempt to classify

    // all have equal precedance
    while (
         curr_tok().type == lexer::MULT 
      || curr_tok().type == lexer::DIV 
      || curr_tok().type == lexer::MOD // same precedance as mult / div
    ){
      auto oper = advance();
      auto right = parse_prim_exp();
      left = std::make_unique<binary_exp>(
        std::move(left), 
        std::move(right), 
        oper
      );
    }

    return left;
}
```

## Example Output

```txt
Program {
  BinaryExpression: + {
    left:
      Identifier: 5
    right:
      BinaryExpression: * {
        left:
          Identifier: 10
        right:
          Identifier: 2
      }
  }
}
```
