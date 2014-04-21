%{
#include <stdio.h>
#include "node.h"
#include "mochi.h"
#include "string.h"

int yylex();
void yyerror(const char *error) { fprintf(stderr, "%s\n", error); }
%}

%token <integer> NUMBER
%token <string> ID STRING
%token PLUS DOT

%union {
  int integer;
  char *string;
  Node *node;
  VALUE value;
}

%left PLUS
%left DOT

%type <integer> program
%type <node> expressions expression

%start program

%%

program: expressions { $$ = mochi_run(append_node($1, new_node(MOCHI_DONE_NODE, 0))); }

expressions: expressions expression { $$ = append_node($1, new_node(MOCHI_EXPRESSION_NODE, 1, $2)); }
           | expression { $$ = new_node(MOCHI_EXPRESSION_NODE, 1, $1); }

expression: expression PLUS expression { $$ = new_node(MOCHI_FUNCTION_CALL_NODE, 3, "+", $1, $3); }
          | expression DOT ID { $$ = new_node(MOCHI_FUNCTION_CALL_NODE, 3, $3, $1, NULL); }
          | NUMBER { $$ = new_node(MOCHI_LITERAL_NODE, 1, INT2FIX($1)); }
          | STRING { $$ = new_node(MOCHI_STRING_NODE, 1, $1); }
