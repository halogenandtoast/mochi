%{
  #include <stdio.h>
  extern char yytext[];
  void yyerror(const char *s) { printf("ERROR: %s\n", s); }
%}

%union {
  int token;
}

%token <token> TPLUS

%start program

%%

program: stmts
       ;

stmts: stmt stmts
     | stmt
     ;

stmt: TPLUS { printf("+"); }
    ;

%%
