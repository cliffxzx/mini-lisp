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
%token <std::string> ID;
%token PRINT_NUM PRINT_BOOL MOD AND OR NOT DEFINE FUN IF ANY NEWLINE END SEPARATOR;

%type <std::string> variable, function_name;
%type <Types::Expression> expression, print_statement, function_expression, function_body, function_call, if_expression, parameter;
%type <Types::List> ids, function_ids, parameters, expressions;

%%

program: statement | statement program;
statement: expression { interpreter << $1; } | define_statement | print_statement { interpreter << $1; };
      print_statement: PRINT_NUM expression { $$ = interpreter.eval("print_num", $2); };
      print_statement: PRINT_BOOL expression { $$ = interpreter.eval("print_bool", $2); };
      define_statement: '(' DEFINE variable expression ')';

      expressions: | expression expressions { $$.push_back($1); };
      expression: BOOL { $$ = $1;};
      expression: NUMBER { $$ = $1;};

      expression: variable { $$ = $1; };
      variable: ID { $$ = $1; };

      expression: function_expression { $$ = $1; };
      function_expression: '(' FUN function_ids function_body ')'
         ids: | ID ids { $$.push_back($1); };
         function_ids: '(' ids ')' { $$ = $2; };
         function_body: expression { $$ = $1; };

      expression: function_call { $$ = $1; };
      function_call: '(' function_name parameters ')' { $$ = interpreter.eval($2, $3); };
      /* function_call: '(' function_expression parameters ')' {}; */
         parameter: expression { $$ = $1; };
         parameters: | parameter parameters { $2.insert($2.begin(), $1); $$ = $2; };
         function_name: ID { $$ = $1; };

      expression: if_expression { $$ = $1; };
      if_expression: '(' IF test_exp than_exp else_exp ')';
         test_exp: expression;
         than_exp: expression;
         else_exp: expression;
%%

void MiniLisp::Parser::error(const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
