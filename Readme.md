# Mini Lisp

## Introduction

This project is a 400+ lines reference from "[(How to Write a (Lisp) Interpreter (in Python))](http://www.norvig.com/lispy.html)". lisp interpreter. The whole project mainly uses stl to implement and use many of the `c++17` features. The tokenize & parser are implemented by the [lex & bison(yacc)](https://github.com/cliffxzx/mini-lisp/tree/lex/bison) version in [here](https://github.com/cliffxzx/mini-lisp/tree/lex/bison).

[The Spec file](./docs%2FCompiler%20Final%20Project.pdf), [Grammar](./docs/MiniLisp.pdf)

| No. | Name                 | Description                                      | Complete |
| --- | -------------------- | ------------------------------------------------ | :------: |
| 1.  | Syntax Validation    | Print “syntax error” when parsing invalid syntax |    ✅    |
| 2.  | Print                | Implement print-num statement                    |    ✅    |
| 3.  | Numerical Operations | Implement all numerical operations               |    ✅    |
| 4.  | Logical Operations   | Implement all logical operations                 |    ✅    |
| 5.  | `if` Expression      | Implement `if` expression                        |    ✅    |
| 6.  | Variable Definition  | Able to define a variable                        |    ✅    |
| 7.  | Function             | Able to declare and call an anonymous function   |    ✅    |
| 8.  | Named Function       | Able to declare and call a named function        |    ✅    |
| 9.  | Recursion            | Support recursive function call                  |    ✅    |
| 10. | Type Checking        | Print error messages for type errors             |    ✅    |
| 11. | Nested Function      | Nested function (static scope)                   |    ✅    |
| 12. | First-class Function | Able to pass functions, support closure          |    ✅    |

## Environment

- OS: macOS 11.5.2 (Darwin Kernel Version 20.6.0)
- Device: MacBookPro14,1
- Compiler: clang 13.0.0
- Compile flags: `-Wno-register -O0 -g -Wall -std=c++20`

## Start

> $ make && ./mini_lisp -r

## Problems Encountered

### What is the best way of preventing memory leaks in a yacc-based parser?

- https://stackoverflow.com/questions/40227135/why-yacc-have-memory-leak-at-exit

### Are GCC and Clang parsers really handwritten?

- https://stackoverflow.com/questions/6319086/are-gcc-and-clang-parsers-really-handwritten

### C++ using declaration recursive

- https://vittorioromeo.info/index/blog/variants_lambdas_part_2.html

### `variant`

- https://zhuanlan.zhihu.com/p/366537214
- https://kheresy.wordpress.com/2017/10/16/cpp17-variant/

### Bison variant

- https://stackoverflow.com/questions/50776052/flex-bison-cannot-use-semantic-type
