#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <regex>
#include <string>
#include <utility>

#include "builtin.hpp"
#include "errors.hpp"
#include "types.hpp"

using std::back_inserter;
using std::copy_if;
using std::get;
using std::get_if;
using std::istream;
using std::istreambuf_iterator;
using std::list;
using std::next;
using std::ostream;
using std::regex;
using std::regex_replace;
using std::sregex_token_iterator;
using std::string;
using std::visit;

namespace MiniLisp {

class Interpreter {
  const regex SEPERATOR = regex("[\\r\\n\\s\\t]+");
  Type::Environment global_env = MiniLisp::global_env<void>();
  ostream &out;

  list<string> tokenize(string inputs) {
    inputs = regex_replace(inputs, regex("[)(]"), " $0 ");
    list<string> tokens;
    copy_if(sregex_token_iterator(inputs.begin(), inputs.end(), SEPERATOR, -1),
            sregex_token_iterator(),
            back_inserter(tokens), [](const string &s) { return !s.empty(); });
    return tokens;
  }

  Type::Expression parse_from_token(list<string> &&tokens) { return parse_from_token(tokens); }
  Type::Expression parse_from_token(list<string> &tokens) {
    if (tokens.empty())
      throw Error::syntax_error("unexpected EOF");

    string token = tokens.front();
    tokens.pop_front();

    if (token == "(") {
      Type::List val{};
      while (tokens.front() != ")")
        val.emplace_back(parse_from_token(tokens));

      tokens.pop_front();
      return val;
    }

    if (token == ")")
      throw Error::syntax_error("unexpected ')'");

    return Type::parse<Type::Expression>(token);
  }

public:
  Interpreter(ostream &t_out) : out(t_out){};

  /**
    * parse - parse from a c++ input stream
    * @param iss - valid input stream
    */
  Type::Expression parse(istream &iss) {
    if (!iss.good())
      return false;

    string inputs(istreambuf_iterator<char>(iss), {});
    return eval(parse_from_token(tokenize(inputs)));
  }

  Type::Expression eval(Type::Expression x) { return eval(x, global_env); }
  Type::Expression eval(Type::Expression x, Type::Environment &env) {
    return visit(
        overloaded{
            [](auto x) -> Type::Expression { return x; },
            [&env, this](Type::Symbol x) -> Type::Expression {
              if (env.contains(x))
                return env[x];

              if (global_env.contains(x))
                return global_env[x];

              throw Error::undefined_error("undefined symbol '"s + x + "'");
            },
            [&env, this](Type::List x) -> Type::Expression {
              if (auto pval = get_if<Type::Symbol>(&x.front()); pval) {
                Type::Symbol op = *pval;

                if (op == "quote")
                  return *next(x.begin());

                if (op == "if") {
                  auto test = get<Type::Bool>(eval(*next(x.begin()), env));
                  return eval((test ? *next(x.begin(), 2) : *next(x.begin(), 3)), env);
                }

                if (op == "define") {
                  auto symbol = get<Type::Symbol>(*next(x.begin()));
                  return env[symbol] = eval(*next(x.begin(), 2), env);
                }

                if (op == "lambda") {
                  Type::List params = get<Type::List>(*next(x.begin()));
                  return Type::Lambda(params, Type::List(next(x.begin(), 2), x.end()), env);
                }
              }

              Type::Function proc = get<Type::Function>(eval(x.front(), env));
              Type::List parameters(++x.begin(), x.end());
              for (auto &param : parameters)
                param = eval(param, env);

              return proc(this, parameters);
            },
        },
        x);
  }

  template <typename T>
  ostream &operator<<(const T &val) { return out << val; }
};

}; // namespace MiniLisp
