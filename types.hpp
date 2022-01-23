#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <functional>
#include <list>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

using namespace std::string_literals;

using std::forward;
using std::function;
using std::get;
using std::list;
using std::ostream;
using std::string;
using std::unordered_map;
using std::variant;

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace MiniLisp {

class Interpreter;
namespace Type {

class Lambda;
class Expression;

using Symbol = string;
using Number = int;
using Bool = bool;
using List = list<Expression>;
using Function = function<Expression(Interpreter *, List)>;
using Environment = unordered_map<Symbol, Expression>;
class Expression : public variant<Bool, Number, Symbol, List, Function> {
public:
  template <typename... Ts>
  Expression(Ts &&...xs) : variant<Bool, Number, Symbol, List, Function>{forward<Ts>(xs)...} {}

  friend ostream &operator<<(ostream &os, const Expression &expression) {
    return visit(overloaded{
                     [&os](Type::Function) -> ostream & { return os; },
                     [&os](Type::Symbol val) -> ostream & { return os << val; },
                     [&os](Type::Number val) -> ostream & { return os << val; },
                     [&os](Type::Bool val) -> ostream & { return os << (val ? "#t" : "#f"); },
                     [&os](Type::List val) -> ostream & {
                       os << "[";
                       for (auto it = val.begin(); it != --val.end(); ++it)
                         os << *it << ", ";
                       return os << val.back() << "]";
                     },
                 },
                 expression);
  }
};

class Lambda : public Function {
  List parameters, body;
  Environment env;

public:
  Lambda() = default;
  Lambda(List t_parameters, List t_body, Environment t_env) : parameters(t_parameters), body(t_body), env(t_env) {}

  template <typename T>
  Expression operator()(T *it, List args) {
    if (args.size() != parameters.size())
      throw Error::parameter_error("too "s + (args.size() < parameters.size() ? "few" : "many") + "parameters to pass");

    auto pit = args.begin();
    auto ait = parameters.begin();
    while (pit != args.end()) {
      env[get<Symbol>(*ait)] = *pit;
      ++ait;
      ++pit;
    }

    for (auto &body_experssion : body)
      body_experssion = it->eval(body_experssion, env);

    return body.back();
  }
};

template <typename T>
T parse(const string &val) {
  if (val == "#t" || val == "#f")
    return val == "#t";

  try {
    return stoi(val);
  } catch (std::out_of_range &e) {
    throw e;
  } catch (...) {
  }

  return val;
};

}; // namespace Type
} // namespace MiniLisp

#endif
