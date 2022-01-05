#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include "location.hh"
#include "parser.tab.hh"

using MiniLisp::Parser;
using std::istream;

namespace MiniLisp {

class Scanner : public yyFlexLexer {
  Parser::semantic_type *yylval = nullptr;

public:
  using FlexLexer::yylex;

  Scanner() = default;
  Scanner(istream *in) : yyFlexLexer(in){};

  virtual int yylex(Parser::semantic_type *const lval, Parser::location_type *location);
};

} // namespace MiniLisp

#endif
