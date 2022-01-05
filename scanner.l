%{
#include <string>

#include "scanner.hpp"

#undef  YY_DECL
// tell Flex the expected prototype of yylex
// MiniLisp::Parser::semantic_type * const - const pointer to MiniLisp::Parser::semantic_type
#define YY_DECL int MiniLisp::Scanner::yylex(MiniLisp::Parser::semantic_type * const lval, MiniLisp::Parser::location_type *loc)

/* define yyterminate as this instead of NULL */
#define yyterminate() return token::END

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

using token = MiniLisp::Parser::token;
using std::string;
using std::stoi;
%}

/* debug mode */
/* https://www.twblogs.net/a/5c67a336bd9eee01cc9e04d6 */
%option debug

/* turn off default behavior */
/* https://zhuanlan.zhihu.com/p/120819122 */
%option nodefault

/* informs flex that you have derived foo as a subclass of yyFlexLexer */
/* so flex will place your actions in the member function foo::yylex() instead of yyFlexLexer::yylex() */
/* it also generates a yyFlexLexer::yylex() member function that emits a run-time error */
/* (by invoking yyFlexLexer::LexerError()) if called */
/* https://nxmnpg.lemoda.net/1/lex */
%option yyclass="MiniLisp::Scanner"

/* makes the scanner not call yywrap() upon an end-of-file */
/* but simply assume that there are no more files to scan */
/* (until the user points yyin at a new file and calls yylex() again). */
%option noyywrap

/* automatically specified if the name of the flex executable ends in a '+', such as flex++ */
/* when using this option, flex defaults to generating the scanner to the file lex.yy.cc instead of lex.yy.c */
%option c++

separator ([\t\n\r ])
letter ([a-z])
digit ([0-9])

number (0|-?[1-9][0-9]*)
id ([a-z]([a-z]|[0-9]|-)*)
bool (#t|#f)

newline (\r?\n)
any (.)

%%

%{
   // code executed at the beginning of yylex
   yylval = lval;
%}

%{
   // https://www.gnu.org/software/bison/manual/html_node/Split-Symbols.html
%}
{number} { yylval->emplace<int32_t>(stoi(yytext)); return token::NUMBER; }
{id} { yylval->emplace<string>(yytext); return token::ID; }
{bool} { yylval->emplace<bool>(strcmp(yytext, "#f")); return token::BOOL; }

%{
   // update line number
%}
{newline} { loc->lines(); return( token::NEWLINE ); }

{any} { return( token::ANY ); }

%%


