# Problems

## What is the best way of preventing memory leaks in a yacc-based parser?

- https://stackoverflow.com/questions/40227135/why-yacc-have-memory-leak-at-exit

## Are GCC and Clang parsers really handwritten?

- https://stackoverflow.com/questions/6319086/are-gcc-and-clang-parsers-really-handwritten

## c++ using declaretion recursive
https://vittorioromeo.info/index/blog/variants_lambdas_part_2.html

## variant
https://zhuanlan.zhihu.com/p/366537214
https://kheresy.wordpress.com/2017/10/16/cpp17-variant/

## bison variant
https://stackoverflow.com/questions/50776052/flex-bison-cannot-use-semantic-type

##
print_num ? print-num
lambda ? fun

# MAYBE ONE DAY USE （TO BE DELETE)
https://stackoverflow.com/questions/31101839/flex-bison-yylex-function-does-not-take-3-arguments

## Cout parameter pack
https://stackoverflow.com/questions/27375089/what-is-the-easiest-way-to-print-a-variadic-parameter-pack-using-stdostream


class lisp_ostream : private ostream {
public:
  lisp_ostream() = default;
  lisp_ostream(streambuf *buf) : ostream(buf){};

  template <typename T>
  friend lisp_ostream &operator<<(lisp_ostream &os, const T &val) {
    ostream &stdos = static_cast<ostream &>(os);
    stdos << val;
    return os;
  }

  friend lisp_ostream &operator<<(lisp_ostream &os1, ostream &(*)(ostream &));

  ostream &get_ostream() { return *this; }
};

inline lisp_ostream &operator<<(lisp_ostream &os, const bool &val) { return os << (val ? "#t" : "#f"); }

auto t = overloaded {
  [](auto arg) { std::cout << arg << ' '; },
  [](double arg) { std::cout << std::fixed << arg << ' '; },
  [](const std::string &arg) { std::cout << std::quoted(arg) << ' '; }
};