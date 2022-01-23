#ifndef __BUILTIN_HPP__
#define __BUILTIN_HPP__

#include <iostream>

#include "errors.hpp"
#include "types.hpp"

using std::endl;

namespace MiniLisp {

template <typename T, bool exact_args_n = false, int args_n = 2, typename Ret = T>
auto builtin(Type::Symbol op, auto fn) -> std::pair<Type::Symbol, Type::Expression> {
  return {op, [&fn, op](Interpreter *it, Type::List args) {
            if ((exact_args_n && args.size() != args_n) || (!exact_args_n && args.size() < args_n))
              throw Error::parameter_error("too "s + (args.size() < args_n ? "few" : "many") + " parameters to pass");

            try {
              if constexpr (args_n == 1) {
                return fn(it, get<T>(args.front()));
              } else {
                T result = get<T>(args.front());
                for (auto it = ++args.begin(); it != args.end(); ++it)
                  result = fn(it, result, get<T>(*it));

                return (Ret)result;
              }
            } catch (std::bad_variant_access) {
              throw Error::type_error("the '"s + op + "' operator only support " + typeid(T).name() + " type");
            }
          }};
}

template <typename T, class... Types>
inline bool operator==(const T &t, const std::variant<Types...> &v) {
  const T *c = get_if<T>(&v);
  return (c ? *c == t : false);
}

template <typename T, class... Types>
inline bool operator==(const std::variant<Types...> &v, const T &t) { return t == v; }

template <typename T>
Type::Environment global_env() {
  Type::Environment env{
      builtin<Type::Number>("+", [](auto, auto x, auto y) { return x + y; }),
      builtin<Type::Number>("-", [](auto, auto x, auto y) { return x - y; }),
      builtin<Type::Number>("*", [](auto, auto x, auto y) { return x * y; }),
      builtin<Type::Number>("/", [](auto, auto x, auto y) { return x / y; }),
      builtin<Type::Number>("mod", [](auto, auto x, auto y) { return x % y; }),
      builtin<Type::Bool>("and", [](auto, auto x, auto y) { return x && y; }),
      builtin<Type::Bool>("or", [](auto, auto x, auto y) { return x || y; }),
      builtin<Type::Bool, true, 1>("not", [](auto, auto x) { return !x; }),
      builtin<Type::Number, true, 2, Type::Bool>(">", [](auto, auto x, auto y) { return x > y; }),
      builtin<Type::Number, true, 2, Type::Bool>("<", [](auto, auto x, auto y) { return x < y; }),
  };

  env["="] = [](Interpreter *it, Type::List args) {
    if (args.size() < 2)
      throw Error::parameter_error("too few parameters to pass"s);

    return visit(
        overloaded{
            [&args](auto x) -> Type::Bool { return find_if(args.begin(), args.end(), [x](auto y) { return x != y; }) == args.end(); },
            [](Type::List x) -> Type::Bool { throw Error::type_error("the '=' operator only support Atom type"); },
            [](Type::Function x) -> Type::Bool { throw Error::type_error("the '=' operator only support Atom type"); }},
        args.front());
  };

  return env;
};

}; // namespace MiniLisp

#endif