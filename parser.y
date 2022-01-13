%code {
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <variant>
#include <vector>

#include "interpreter.hpp"

#undef yylex
#define yylex scanner.yylex
}

// insert code verbatim into the output parser source at the default location or at the location specified by qualifier
// https://www.gnu.org/software/bison/manual/bison.html#g_t_0025code-Summary
%code requires {
#include "types.hpp"
namespace MiniLisp {
  class Interpreter;
  class Scanner;
};

}

// specify the skeleton to use
%skeleton "lalr1.cc"

// require version version or higher of Bison
%require "3.0"

// specify the namespace for the parser class
%define api.namespace {MiniLisp}

// the name of the parser class
%define api.parser.class {Parser}

// bison provides a variant based implementation of semantic values for C++
// https://www.gnu.org/software/bison/manual/bison.html#C_002b_002b-Variants
%define api.value.type variant

// in C++, when variants are used (see C++ Variants)
// symbols must be constructed and destroyed properly
// This option checks these constraints using runtime type information (RTTI)
// %define parse.assert

// declare that one or more argument-declaration are additional yyparse arguments
%parse-param { Scanner &scanner } { Interpreter &interpreter }

// generate the code processing the locations
%locations


// %token               END    0     "end of file"
%token <int32_t> NUMBER;
%token <bool> BOOL;
%token <std::string> ID, VARIABLE;
%token DEFINE FUN ANY NEWLINE END SEPARATOR;

%type <std::string> function_name;
%type <Types::Expression> expression, print_statement, function_expression, function_call, if_expression, parameter, function_body;
%type <Types::List> ids, function_ids, parameters, define_statement;

%%

program: statement | statement program;
statement: expression { interpreter << interpreter.eval($1); } | define_statement { interpreter.eval($1); };
   define_statement: '(' DEFINE ID expression ')' { $$ = Types::List{"define", $3, $4}; };
   expression: ID { $$ = $1; } | BOOL { $$ = $1;} | NUMBER { $$ = $1;};

   expression: function_expression { $$ = $1; };
   function_expression: '(' FUN function_ids function_body ')' { $$ = Types::List{"lambda", $3, $4}; }
      ids: | ID ids { $2.insert($2.begin(), $1); $$ = $2; };
      function_ids: '(' ids ')' { $$ = $2; };
      function_body: expression { $$ = $1; };

   expression: function_call { $$ = $1; };
   function_call: '(' function_name parameters ')' { $3.insert($3.begin(), $2); $$ = $3; };
   function_call: '(' function_expression parameters ')' { $3.insert($3.begin(), $2); $$ = $3; };
      parameter: expression { $$ = $1; };
      parameters: | parameter parameters { $2.insert($2.begin(), $1); $$ = $2; };
      function_name: ID { $$ = $1; };
%%

void MiniLisp::Parser::error(const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
