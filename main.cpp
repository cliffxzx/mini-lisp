#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "interpreter.hpp"

using MiniLisp::Interpreter;
using std::cin;
using std::cout;
using std::ifstream;
using std::strcmp;

const string HELP_INFOMATION = R"(
use -i for command line interpreter
or directly give a filename.
ex:
  ./run -i
  ./run ./mini.lsp
  ./run -h
    To get this infomation
)";

int main(const int argc, const char **argv) {
  if (argc == 2) {
    Interpreter interpreter;
    if (!strcmp(argv[1], "-i")) {
      return interpreter.parse(cin);
    }

    if (!strcmp(argv[1], "-h")) {
      cout << HELP_INFOMATION;
      return EXIT_SUCCESS;
    }

    if (!ifstream(argv[1]).good()) {
      cout << "File error\n";
      return EXIT_FAILURE;
    }

    return interpreter.parse(argv[1]);
  }

  cout << HELP_INFOMATION;
  return EXIT_FAILURE;
}
