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
using std::get;
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
using std::string_literals::s;

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
    T result = std::get<T>(*interatable.begin());
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
      {"+", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                 return x + std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               }
             }),
         };

         return visit(resolve, val[0]);
       }},
      {"-", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                  return x - std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return visit(resolve, val[0]);
       }},
      {"*", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                  return x * std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return visit(resolve, val[0]);
       }},
      {"/", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                  return x / std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return visit(resolve, val[0]);
       }},
      {"mod", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                  return x % std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return visit(resolve, val[0]);
       }},
      {">", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                  return x > std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return Types::Expression(Types::Bool(std::get<Types::Number>(visit(resolve, val[0]))));
       }},
      {"<", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                  return x < std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return Types::Expression(Types::Bool(std::get<Types::Number>(visit(resolve, val[0]))));
       }},
      {"=", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Number>(val, [](auto x, auto y) {
               try {
                  return x == std::get<Types::Number>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return Types::Expression(Types::Bool(std::get<Types::Number>(visit(resolve, val[0]))));
       }},
      {"and", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Bool>(val, [](auto x, auto y) {
               try {
                  return x && std::get<Types::Bool>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return visit(resolve, val[0]);
       }},
      {"or", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             convert<Types::Bool>(val, [](auto x, auto y) {
               try {
                  return x || std::get<Types::Bool>(y);
               } catch (...) {
                 std::cerr << "Type error!" << endl;
                 exit(0);
               } }),
         };

         return visit(resolve, val[0]);
       }},
      {"not", [&](Types::List val, Interpreter *) -> Types::Expression {
         auto resolve = overloaded{
             [&](auto val) -> Types::Expression { std::cerr << "Type error!" << endl; exit(0); },
             [&](Types::Bool val) -> Types::Expression { try {
               return !val;
               } catch (...) {
           std::cerr << "Type error!" << endl;
           exit(0);
         } },
         };

         return visit(resolve, val[0]);
       }},
      {"print-num", [](Types::List val, Interpreter *) {
         try {
           return std::get<Types::Number>(val[0]);

         } catch (...) {
           std::cerr << "Type error!" << endl;
           exit(0);
         }
       }},
      {"print-bool", [](Types::List val, Interpreter *) {
         try {
           return std::get<Types::Bool>(val[0]);

         } catch (...) {
           std::cerr << "Type error!" << endl;
           exit(0);
         }
       }},
  };

  class Lambda : public Types::Function {
    Types::List parameters;
    Types::Expression body;
    Types::Environment env;

  public:
    Lambda() = default;
    Lambda(Types::List t_parameters,
           Types::Expression t_body,
           Types::Environment t_env) : parameters(t_parameters), body(t_body), env(t_env) {}

    Types::Expression operator()(Types::List args, Interpreter *it) {
      assert(args.size() == parameters.size());
      for (int w = 0; w < args.size(); ++w) {
        Types::Expression t = args[w];
        Types::Expression t2 = parameters[w];
        env[std::get<Types::Symbol>(parameters[w])] = args[w];
      }

      for (auto p : it->global_env) {
        if (env.find(p.first) == env.end())
          env[p.first] = p.second;
      }

      return it->eval(body, env);
    }
  };

public:
  Interpreter() = default;

  vector<string> tokenize(istream in) {
    char ch;
    while(in >> ch) {

    };
  }

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

  Types::Expression eval(Types::Expression token) { return eval(token, global_env); }
  Types::Expression eval(Types::Expression token, Types::Environment &env) {
    if (auto pval = get_if<Types::Symbol>(&token)) {
      Types::Symbol val = *pval;
      return env[val];
    }
    if (auto pval = get_if<Types::Number>(&token)) {
      Types::Number val = *pval;
      return val;
    }
    if (auto pval = get_if<Types::Bool>(&token)) {
      Types::Bool val = *pval;
      return val;
    }
    // if (auto pval = get_if<Types::Function>(&token)) {
    // }
    if (auto pval = get_if<Types::List>(&token)) {
      Types::List val = *pval;
      try {
        auto op = get<Types::Symbol>(val[0]);
        if (op == "if") {
          auto teste = eval(val[1], env);
          if (auto pval = get_if<Types::Bool>(&teste)) {
            Types::Bool test = *pval;
            return eval((test ? val[2] : val[3]), env);
          }
          if (auto pval = get_if<Types::Number>(&teste)) {
            Types::Bool test = *pval;
            return eval((test ? val[2] : val[3]), env);
          }
          std::cerr << "Type error!" << endl;
          exit(0);
        }
        if (op == "define") {
          Types::Symbol symbol = get<Types::Symbol>(val[1]);
          return env[symbol] = eval(val[2], env);
        }
        if (op == "lambda") {
          Types::List params = get<Types::List>(val[1]);
          return Lambda(params, val[2], env);
        }
      } catch (...) {
      }

      auto proc = get<Types::Function>(eval(val[0], env));
      Types::List args = Types::List{};
      for (int w = 1; w < val.size(); ++w) {
        args.push_back(eval(val[w], env));
      }

      return proc(args, this);
    }

    throw "Syntax error";
  }

  friend ostream &operator<<(Interpreter &os, const Types::Expression &val) {
    visit(overloaded{
              [](auto val) { cout << typeid(val).name(); },
              [](Types::Symbol val) { cout << val; },
              [](Types::Number val) { cout << val; },
              [](Types::Bool val) { cout << (val ? "#t" : "#f"); },
              [&os](Types::List val) { for (auto x : val) { os << x << ',';} },
          },
          val);

    return os.out << endl;
  };
};
}; // namespace MiniLisp
#endif
