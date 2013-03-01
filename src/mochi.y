%{
  #include <stdio.h>
  #include "mochi.h"
  extern char yytext[];
  void yyerror(const char *s) { printf("ERROR: %s\n", s); }
  static NODE* call_bin_op(NODE*, ID, NODE*);
  static NODE* node_assign(NODE*, NODE*);
%}

%error-verbose

%union {
  NODE* node;
  int token;
  char* string;
}

%token <node> TNUMBER TNAME
%token <string> TNEWLINE TOP

%type <node> literal expression value stmt stmts program

%start program

%%

program: stmts terms { printf("%d\n", $1->value1); }
       ;

stmts: stmts terms stmt { $$ = $3; }
     | stmt { $$ = $1; }
     ;

stmt: expression
    ;

expression: value '+' value { $$ = call_bin_op($1, '+', $3); }
          | TNAME '=' expression { $$ = node_assign($1, $3); }
          ;

value: TNAME
     | literal

literal: TNUMBER
       ;

terms: TNEWLINE
     ;

%%

static NODE* call_bin_op(NODE* receiver, ID id, NODE* argument) {
  return NEW_CALL(receiver, id, NEW_ARRAY(argument));
}

static NODE* node_assign(NODE* lhs, NODE* rhs) {
  return mochi_node_newnode(NODE_LIT, 1);
}
