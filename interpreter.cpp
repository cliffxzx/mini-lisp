#include <cassert>
#include <cctype>
#include <fstream>
#include <memory>

#include "interpreter.hpp"

using MiniLisp::Interpreter;
using std::cerr;
using std::ifstream;
using std::make_unique;
using std::ostream;

namespace MiniLisp {
lisp_ostream::lisp_ostream(streambuf *buf) : std::ostream(buf){};

// template <typename T>
// inline lisp_ostream &operator<<(lisp_ostream &os, const T &val) {
//   ostream &stdos = static_cast<ostream &>(os);
//   stdos << val;
//   return os;
// }

inline lisp_ostream &operator<<(lisp_ostream &os, const bool &val) { return os << (val ? "#t" : "#f"); }
inline lisp_ostream &operator<<(lisp_ostream &os, const int32_t &val) { return os << val; }
} // namespace MiniLisp

namespace MiniLisp {

bool Interpreter::parse(const string filename) {
  ifstream in_file(filename);
  if (!in_file.good()) {
    return false;
  }

  return Interpreter::parse(in_file);
}

bool Interpreter::parse(istream &stream) {
  if (!stream.good() && stream.eof()) {
    return false;
  }

  auto scanner = make_unique<Scanner>(&stream);
  auto parser = make_unique<Parser>(*scanner, *this);

  if (!parser->parse()) {
    cerr << "Parse failed!!\n";
    return false;
  }

  return true;
}

template <typename T>
inline lisp_ostream &operator<<(Interpreter &os, const T &val) { return os.out << val; }
} // namespace MiniLisp