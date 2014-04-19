%{
  #include <stdio.h>
  #include "node.h"
  #include "y.tab.h"
%}

%option noyywrap

%%

"\""([^\"|\\.])+"\"" {
  char *tmp = (char *)malloc(sizeof(char) * strlen(yytext) - 1);
  strncpy(tmp, yytext+1, strlen(yytext) - 2);
  tmp[strlen(yytext) - 2] = '\0';
  yylval.string = tmp;
  return STRING;
}
[0-9]+ { yylval.integer = atoi(yytext); return NUMBER; }
[a-z]+ { yylval.string = strdup(yytext); return ID; }
"." { return DOT; }
"+" { return PLUS; }
[ \t\n] {}
. { fprintf(stderr, "Invalid token: %s\n", yytext); }
