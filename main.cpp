#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "interpreter.cpp"

using namespace std::string_literals;

using MiniLisp::Interpreter;
using MiniLisp::Type::Expression;
using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::ifstream;
using std::quoted;
using std::stringstream;

const string RED = "\033[1;31m";
const string BLUE = "\033[1;36m";
const string GREEN = "\033[1;32m";
const string NORM = "\033[0m";

const string HELP_INFOMATION = R"(use -i for command line interpreter
use -r for repl mode
or directly give a filename.
ex:
  ./run -i
  ./run -r
  ./run ./mini.lsp
  ./run -h
    To get this infomation
)";

void eval(Interpreter &interpreter, istream &in) {
  Expression result;
  try {
    result = interpreter.parse(in);
    interpreter << ":=" << GREEN << result << NORM << endl;
  } catch (const std::exception &exc) {
    interpreter << RED << exc.what() << NORM << endl;
  } catch (...) {
    interpreter << RED << "unexpected unknown error" << NORM << endl;
  }
}

int main(const int argc, const char **argv) {
  Interpreter interpreter(cout);
  if (argc != 2) {
    interpreter << RED << HELP_INFOMATION << NORM;
    return EXIT_FAILURE;
  }

  if (argv[1] == "-h"s) {
    interpreter << RED << HELP_INFOMATION << NORM;
    return EXIT_SUCCESS;
  }

  if (argv[1] == "-i"s) {
    interpreter << BLUE << "<<<" << NORM << "  " << flush;
    eval(interpreter, cin);
    return EXIT_SUCCESS;
  }

  if (argv[1] == "-r"s) {
    string inputs;
    while (interpreter << BLUE << "<<<" << NORM << "  " << flush && getline(cin, inputs)) {
      stringstream ss(inputs);
      eval(interpreter, ss);
    }

    interpreter << GREEN << "BYE! BYE!" << NORM << flush;
    return EXIT_SUCCESS;
  }

  ifstream file_in(argv[1]);
  if (!file_in.good()) {
    interpreter << RED << "Can't open file: " << NORM << quoted(argv[1]) << endl;
    return EXIT_FAILURE;
  }

  interpreter << interpreter.parse(file_in) << endl;
  return EXIT_SUCCESS;
}
