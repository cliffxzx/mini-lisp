#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__ 1

#include <cstddef>
#include <iostream>
#include <string>

#include "parser.tab.hh"
#include "scanner.hpp"

using std::cout;
using std::istream;
using std::ostream;
using std::streambuf;
using std::string;

namespace MiniLisp {
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

class Interpreter {
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

  // template <typename T>
  // friend lisp_ostream &operator<<(Interpreter &os, const T &val);

  lisp_ostream out = cout.rdbuf();

private:
  const string red = "\033[1;31m";
  const string blue = "\033[1;36m";
  const string norm = "\033[0m";
};

} // namespace MiniLisp
#endif
