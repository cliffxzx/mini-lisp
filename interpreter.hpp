#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__ 1

#include <algorithm>
#include <cstddef>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "parser.tab.hh"
#include "scanner.hpp"

#include "types.hpp"

using MiniLisp::Types;
using std::cout;
using std::endl;
using std::function;
using std::istream;
using std::map;
using std::multiplies;
using std::ostream;
using std::plus;
using std::streambuf;
using std::string;
using std::variant;
using std::vector;
using std::visit;

// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <typename T, typename V>
auto convert(V interatable, function<T(T, Types::Expression)> fn, T init_val) {
  return [interatable, init_val, fn](T) -> Types::Expression {
    T result = init_val;
    for (Types::Expression v : interatable) {
      result = fn(result, v);
    }

    return result;
  };
}

template <typename T, typename V>
auto convert(V interatable, function<T(T, Types::Expression)> fn) {
  return [interatable, fn](T) -> Types::Expression {
    T result = get<T>(*interatable.begin());
    for (auto it = ++interatable.begin(); it != interatable.end(); ++it) {
      result = fn(result, *it);
    }

    return result;
  };
}

namespace MiniLisp {

class Interpreter {
  const string red = "\033[1;31m";
  const string blue = "\033[1;36m";
  const string norm = "\033[0m";

  ostream out = ostream(cout.rdbuf());

  Types::Environment global_env = {
      {"+", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             convert<Types::Number>(val, [](auto x, auto y) { return x + std::get<Types::Number>(y); }),
         };

         return visit(resolve, val[0]);
       }},
      {"-", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             convert<Types::Number>(val, [](auto x, auto y) { return x - std::get<Types::Number>(y); }),
         };

         return visit(resolve, val[0]);
       }},
      {"*", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             convert<Types::Number>(val, [](auto x, auto y) { return x * std::get<Types::Number>(y); }),
         };

         return visit(resolve, val[0]);
       }},
      {"/", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             convert<Types::Number>(val, [](auto x, auto y) { return x / std::get<Types::Number>(y); }),
         };

         return visit(resolve, val[0]);
       }},
      {"mod", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             convert<Types::Bool>(val, [](auto x, auto y) { return x % std::get<Types::Bool>(y); }),
         };

         return visit(resolve, val[0]);
       }},
      {">", [&](Types::List val) -> Types::Expression { return std::is_sorted(val.begin(), val.end(), std::greater_equal<>()); }},
      {"<", [&](Types::List val) -> Types::Expression { return std::is_sorted(val.begin(), val.end(), std::less_equal<>()); }},
      {"=", [&](Types::List val) -> Types::Expression { return std::equal(val.begin() + 1, val.end(), val.begin()); }},
      {"and", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             convert<Types::Bool>(val, [](auto x, auto y) { return x && std::get<Types::Bool>(y); }),
         };

         return visit(resolve, val[0]);
       }},
      {"or", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             convert<Types::Bool>(val, [](auto x, auto y) { return x || std::get<Types::Bool>(y); }),
         };

         return visit(resolve, val[0]);
       }},
      {"not", [&](Types::List val) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { throw "not implemented"; },
             [&](Types::Bool val) -> Types::Expression { return !val; },
         };

         return visit(resolve, val[0]);
       }},
      {"print_num", [](Types::List val) { return get<Types::Number>(val[0]); }},
      {"print_bool", [](Types::List val) { return get<Types::Bool>(val[0]); }},
  };

public:
  Interpreter() = default;

  /**
    * parse - parse from a file
    * @param filename - valid string with input file
    */
  bool parse(const string filename);

  /**
    * parse - parse from a c++ input stream
    * @param is - std::istream&, valid input stream
    */
  bool parse(istream &iss);

  int32_t eval(int32_t x) { return x; }
  template <typename... Args>
  Types::Expression eval(string variable, Args... args) { return eval(variable, Types::List{args...}); }
  Types::Expression eval(string variable, Types::List args) {
    cout << variable << endl;
    return global_env[variable](args);
  }

  friend ostream &operator<<(Interpreter &os, const Types::Expression &val) {
    visit(overloaded{
              [](auto val) { cout << val; },
              [](Types::Bool val) { cout << (val ? "#t" : "#f"); },
              [&os](Types::List val) { for (auto x : val) { os << x << ',';} },
          },
          val);

    return os.out << endl;
  };
};

}; // namespace MiniLisp
#endif
