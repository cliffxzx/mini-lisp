#ifndef __ERRORS_HPP__
#define __ERRORS_HPP__

#include <stdexcept>

using std::logic_error;
using std::runtime_error;

namespace MiniLisp::Error {
class syntax_error : public runtime_error {
  using runtime_error::runtime_error;
};

class undefined_error : public runtime_error {
  using runtime_error::runtime_error;
};

class parameter_error : public logic_error {
  using logic_error::logic_error;
};

class type_error : public runtime_error {
  using runtime_error::runtime_error;
};
}; // namespace MiniLisp::Error

#endif