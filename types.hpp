#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

using std::function;
using std::map;
using std::string;
using std::variant;
using std::vector;

namespace MiniLisp {

class Types {
  public:

  using Symbol = string;
  using Number = int32_t;
  using Bool = bool;
  using Atom = variant<Symbol, Number, Bool>;

  class ExpressionWrapper;
  using List = vector<ExpressionWrapper>;
  using Expression = variant<Symbol, Number, Bool, List>;

  class ExpressionWrapper : public Expression {
  public:
    template <typename... Ts>
    ExpressionWrapper(Ts &&...xs) : Expression{std::forward<Ts>(xs)...} {}
  };

  using Function = function<Expression(List)>;
  using Environment = map<Symbol, Function>;
};

} // namespace MiniLisp
#endif
