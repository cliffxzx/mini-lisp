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

  try {
    if (!parser->parse()) {
      cerr << "Parse failed!!\n";
      return false;
    }
  } catch(const string &msg) {
    cerr << msg << endl;
  }

  return true;
}
} // namespace MiniLisp