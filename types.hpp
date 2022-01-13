#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

using std::function;
using std::get;
using std::map;
using std::string;
using std::variant;
using std::vector;

template <class T, class New>
class flattern_variant;

template <class... Old, class New>
class flattern_variant<variant<Old...>, New> : variant<Old..., New> {};

namespace MiniLisp {
class Interpreter;
class Types {
public:
  class Lambda;
  class Expression;
  using Symbol = string;
  using Number = int32_t;
  using Bool = bool;
  using Atom = variant<Symbol, Number, Bool>;
  using List = vector<Expression>;
  using Function = function<Expression(List, Interpreter *)>;
  using Environment = map<Symbol, Expression>;
  class Expression : public flattern_variant<Atom, List, Function> {
  public:
    template <typename... Ts>
    Expression(Ts &&...xs) : variant<Symbol, Number, Bool, List, Function>{std::forward<Ts>(xs)...} {}

    auto operator+(Expression &val) {
    }
  };
};

} // namespace MiniLisp
#endif
