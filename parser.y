%code {
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

#include "interpreter.hpp"

#undef yylex
#define yylex scanner.yylex

}

// insert code verbatim into the output parser source at the default location or at the location specified by qualifier
// https://www.gnu.org/software/bison/manual/bison.html#g_t_0025code-Summary
%code requires {
namespace MiniLisp {
  class Interpreter;
  class Scanner;
}

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
%define parse.assert

// declare that one or more argument-declaration are additional yyparse arguments
%parse-param { Scanner &scanner } { Interpreter &interpreter }

// generate the code processing the locations
%locations

// %token               END    0     "end of file"
%token <int32_t> NUMBER
%token <bool> BOOL
%token <std::string> ID
%token PRINT_NUM PRINT_BOOL MOD AND OR NOT DEFINE FUN IF ANY NEWLINE END

%type <int32_t> expression;
%%

program: statement | statement program;
statement: expression | define_statement | print_statement;
   print_statement: PRINT_NUM expression { interpreter.out << $2; }
   print_statement: PRINT_BOOL expression { interpreter.out << $2; };
   expressions: | expression expressions;
   expression: BOOL | NUMBER | variable | num_operator | logical_operator | function_expression | function_call | if_expression;
      num_operator: plus | minus | multiply | divide | modulus | greater | smaller | equal;
         plus: '(' '+' expression expression expressions ')';
         minus: '(' '-' expression expression ')';
         multiply: '(' '*' expression expression expressions ')';
         divide: '(' '/' expression expression ')';
         modulus: '(' MOD expression expression ')';
         greater: '(' '>' expression expression ')';
         smaller: '(' '<' expression expression ')';
         equal: '(' '=' expression expression expressions ')';
      logical_operator: and_operator | or_operator | not_operator;
         and_operator: '('AND expression expression expressions')';
         or_operator: '('OR expression expression expressions')';
         not_operator: '('NOT expression')';
      define_statement: '(' DEFINE variable expression ')';
      variable: ID;
      function_expression: '(' FUN function_ids function_body ')';
         ids: | ID ids;
         function_ids: '(' ids ')';
         function_body: expression;
         function_call: '(' function_expression parameters ')';
         function_call: '(' function_name parameters ')';
            parameter: expression;
            parameters: | parameter parameters;
            last_exp: expression;
            function_name: ID;
      if_expression: '(' IF test_exp than_exp else_exp ')';
         test_exp: expression;
         than_exp: expression;
         else_exp: expression;
%%

void MiniLisp::Parser::error(const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
