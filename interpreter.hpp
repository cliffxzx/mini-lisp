#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__ 1

#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <variant>
#include <vector>

#include "parser.tab.hh"
#include "scanner.hpp"

#include "types.hpp"

using MiniLisp::Types;
using std::accumulate;
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

// helper type for the visitor #4
template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace MiniLisp {

class Interpreter {
  const string red = "\033[1;31m";
  const string blue = "\033[1;36m";
  const string norm = "\033[0m";

  ostream out = ostream(cout.rdbuf());

  Types::Environment global_env = {
      // {"+", [&](List val) { return accumulate(val.begin(), val.end(), 0, plus<int32_t>()); }},
      // {"-", [&](List val) { return val[0] - val[1]; }},
      // {"*", [&](List val) { return accumulate(val.begin(), val.end(), 0, multiplies<int32_t>()); }},
      // {"/", [&](List val) { return val[0] / val[1]; }},
      // {">", [](List val) { return val[0] > val[1]; }},
      // {"<", [](List val) { return val[0] < val[1]; }},
      // {"=", [&](List val) { return accumulate(val.begin(), val.end(), 0, equal<int32_t>()); }},
      // {"mod", [&](List val) { return val[0] % val[1]; }},
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
    cout << variable;
    return global_env[variable](args);
  }

  friend ostream &operator<<(Interpreter &os, const Types::Expression &val) {
    std::visit(overloaded{
                   [](Types::Symbol val) { cout << val; },
                   [](Types::Number val) { cout << val; },
                   [](Types::Bool val) { cout << (val ? "#t" : "#f"); },
                   [&os](Types::List val) {
                     for (auto x : val) {
                       os << x;
                     }
                   },
               },
               val);

    return os.out << endl;
  };
};

}; // namespace MiniLisp
#endif
