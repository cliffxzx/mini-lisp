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

template <typename T, typename Val>
void assign(Parser::semantic_type * &yylval, Val val) {
   yylval->destroy<T>();
   yylval->emplace<T>(val);
}

#define ASSIGN(T, val) do { yylval->destroy<T>(); yylval->emplace<T>(yytext); } while(0);

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

separator ([\t\n\r ]*)
letter ([a-z])
digit ([0-9])
braces (\)|\()

number (0|-?[1-9]{digit}*)
symbol ([-+*/><=])
id ({letter}({letter}|{digit})*)|{symbol}
bool_val ((#t)|(#f))
print_num (print_num)
print_bool (print_bool)
mod (mod)
and (and)
or (or)
not (not)
define (define)
fun (fun)
if (if)

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
{separator} { }
{braces} { assign<std::string>(yylval, yytext); return yytext[0]; }
{number} { assign<int32_t>(yylval, stoi(yytext)); return token::NUMBER; }
{id} { assign<std::string>(yylval, yytext); return token::ID; }
{bool_val} { assign<bool>(yylval, strcmp(yytext, "#f")); return token::BOOL; }
{print_num} { return token::PRINT_NUM; }
{print_bool} { return token::PRINT_BOOL; }
{mod} { return token::MOD; }
{and} { return token::AND; }
{or} { return token::OR; }
{not} { return token::NOT; }
{define} { return token::DEFINE; }
{fun} { return token::FUN; }
{if} { return token::IF; }

%{
   // update line number
%}
{newline} { loc->lines(); return token::NEWLINE; }

{any} { }

%%


